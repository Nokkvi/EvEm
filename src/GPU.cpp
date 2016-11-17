#include "../include/GPU.h"

GPU::GPU()
{
    screen = new Screen(160, 144);
    pal = new Register(2);
    scx = new Register(1);
    scy = new Register(1);
    tileset = new uint8_t**[384];

    oam = new uint8_t[160];
    objectdata = new objdata[40];

    for (int i = 0; i < 384; i++)
    {
        tileset[i] = new uint8_t*[8];
        for (int j = 0; j < 8; j++)
        {
            tileset[i][j] = new uint8_t[8];
        }
    }

    vram = new uint8_t[0xFFFF];
    Reset();
}

GPU::~GPU()
{
    for (int i = 0; i < 384; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            delete[] tileset[i][j];
        }
        delete[] tileset[i];
    }
    delete[] tileset;
    delete[] vram;
    delete pal;
    delete scx;
    delete scy;
    delete screen;
}

uint8_t GPU::GetVram(uint16_t addr)
{
    switch (addr)
    {
        case 0xFF40:
            return
                this->switchBG ? 0x01 : 0x00 |
                this->bgMap ? 0x08 : 0x00 |
                this->bgTile ? 0x10 : 0x00 |
                this->switchLCD ? 0x80 : 0x00;
        case 0xFF42:
            return this->scy->GetByte(0);
        case 0xFF43:
            return this->scx->GetByte(0);
        default:
            return line;
    }
}

void GPU::SetVram(uint16_t addr, uint8_t value)
{
    switch (addr)
    {
        case 0xFF40:
            this->switchBG = (value & 0x01) != 0;
            this->bgMap = (value & 0x08) != 0;
            this->bgTile = (value & 0x10) != 0;
            this->switchLCD = (value & 0x85) != 0;
            break;
        case 0xFF42:
            this->scy->SetByte(0, value);
            break;
        case 0xFF43:
            this->scx->SetByte(0, value);
            break;
        case 0xFF44:
            uint8_t val = 0;
            for (int i = 0; i < 4; i++)
            {
                // Adds the 2 bits required to say the color
                // 0 = 255, 1 = 192, 2 = 96, 3 = 0
                val = (val << 2) | (val >> (i * 2) & 3);
                if (i % 2 == 1)
                    this->pal->SetByte(i / 2, val);
            }
            break;
    }
}

void GPU::Render()
{
    this->screen->Render();
}

void GPU::Reset()
{
    for (int i = 0; i < 384; i++)
    {
         for(int j = 0; j < 8; j++)
         {
             for(int h = 0; h < 8; h++)
             {
                this->tileset[i][j][h] = 0;
             }
         }
    }
    for (int i = 0, n = 0; i<40; i++, n+=4){
        this->oam[n+0] = 0;
        this->oam[n+1] = 0;
        this->oam[n+2] = 0;
        this->oam[n+3] = 0;
        this->objectdata[i].y = -16;
        this->objectdata[i].x = -8;
        this->objectdata[i].tile = 0;
        this->objectdata[i].palette = 0;
        this->objectdata[i].xflip = 0;
        this->objectdata[i].yflip = 0;
        this->objectdata[i].prio = 0;
        this->objectdata[i].num = 1;
    }
}

void GPU::buildobjdata(uint16_t address, uint8_t val){
    int obj = address >> 2;
    if(obj < 40){
        switch(address & 3){
            //Y-coord
            case 0: this->objectdata[obj].y = val-16; break;
            //X-coord
            case 1: this->objectdata[obj].x = val-8; break;
            //Data tile
            case 2: this->objectdata[obj].tile = val; break;
            //Options
            case 3:
                this->objectdata[obj].palette = (val & 0x10) ? 1 : 0;
                this->objectdata[obj].xflip = (val & 0x20) ? 1 : 0;
                this->objectdata[obj].yflip = (val & 0x40) ? 1 : 0;
                this->objectdata[obj].prio = (val & 0x80) ? 1 : 0;
                break;
        }
    }
}

void GPU::Scanline()
{
    uint8_t x = scx->GetByte(0);
    uint8_t y = scy->GetByte(0);
    uint16_t mapoffs = true ? 0x1C00 : 0x1800;
    mapoffs += ((this->line + y) & 0xFF) >> 3;
    uint8_t lineoffs = x >> 3;
    y = (this->line + y) & 0x7;
    x = x & 0x7;
    int canvasoffs = this->line * 160;
    uint16_t color;
    uint8_t tile = this->vram[mapoffs + lineoffs];
    if (this->bgTile == true && tile < 0xFF)
        tile += 0x01FF;

    for (int i = 0; i < 160; i++)
    {
        uint8_t cindex = this->tileset[tile][y][x];
        color = (this->pal->GetByte(cindex * 2) << 8) | this->pal->GetByte((cindex * 2) + 1);
        canvasoffs++;
        x++;

        this->screen->SetPixels(canvasoffs, color);
        if (x == 8)
        {
            x = 0;
            lineoffs = (lineoffs + 1) & 0x3F;
            tile = this->vram[mapoffs + lineoffs];
            if (this->bgTile == true && tile < 0xFF)
                tile += 0x01FF;
        }
    }
}

void GPU::Step(unsigned int clock)
{
    this->clock += clock;
    switch (this->mode)
    {
        case 3:
            if (this->clock >= 172)
            {
                this->clock = 0;
                this->mode = 0;

                this->Scanline();
            }
            break;
        case 2:
            if (this->clock >= 80)
            {
                this->clock = 0;
                this->mode = 3;
            }
            break;
        case 1:
            if (this->clock >= 456)
            {
                this->clock = 0;
                this->line++;
                if (this->line > 153)
                {
                    this->mode = 2;
                    this->line = 0;
                }
            }
            break;
        case 0:
            if (this->clock >= 204)
            {
                this->clock = 0;
                this->line++;

                if (this->line == 143)
                {
                    this->mode = 1;
                    // Update screen data.
                }
                else
                {
                    this->mode = 2;
                }
            }
            break;
    }
}
