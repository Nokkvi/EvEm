#include "../include/GPU.h"

GPU::GPU()
{
    screen = new Screen(160, 144);
    scx = new Register(1);
    scy = new Register(1);
    tileset = new uint8_t**[384];

    for (int i = 0; i < 384; i++)
    {
        tileset[i] = new uint8_t*[8];
        for (int j = 0; j < 8; j++)
        {
            tileset[i][j] = new uint8_t[8];
        }
    }

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
    delete scx;
    delete scy;
    delete screen;
}

uint8_t GPU::GetVram(uint16_t addr)
{
    return this->vram[addr];
}

void GPU::SetVram(uint16_t addr, uint8_t value)
{
    this->vram[addr] = value;
}

uint8_t GPU::GetOAM(uint16_t addr)
{
    return this->oam[addr];
}

void GPU::SetOAM(uint16_t addr, uint8_t value)
{
    this->oam[addr] = value;
}

uint8_t GPU::GetData(uint16_t addr)
{
    uint16_t gaddr = addr - 0xFF40;
    switch(gaddr)
    {
        case 0:
            return (this->switchLCD ? 0x80 : 0) |
                ((this->bgTile) ? 0x10 : 0) |
                ((this->bgMap) ? 0x08 : 0) |
                (this->objSize ? 0x04 : 0) |
                (this->switchObj ? 0x02 : 0) |
                (this->switchBG ? 0x01 : 0);
        case 1:
            return (this->line == this->raster ? 4 : 0) | this->mode;
        case 2:
            return this->scy->GetByte(0);
        case 3:
            return this->scx->GetByte(0);
        case 4:
            return line;
        case 5:
            return this->raster;
        default:
            return this->reg[gaddr];
    }
}

void GPU::SetData(uint16_t addr, uint8_t value)
{
    uint16_t gaddr = addr - 0xFF40;
    switch(gaddr)
    {
        case 0:
            this->switchLCD = (value & 0x80) != 0;
            this->bgTile = (value & 0x10) != 0;
            this->bgMap = (value & 0x08) != 0;
            this->objSize = (value & 0x04) != 0;
            this->switchObj = (value & 0x2) != 0;
            this->switchBG = (value & 0x01) != 0;
        case 2:
            this->scy->SetByte(0, value);
        case 3:
            this->scx->SetByte(0, value);
        case 5:
            this->raster = value;
        case 6:
            int v;
            for(int i = 0; i < 160; i++)
            {
                v = ((value << 8) + i);
                this->oam[i] = v;
                this->UpdateOAM(0xFE00 + i);
            }
            break;
        case 7:
            for(int i = 0; i < 4; i++)
            {
                switch((value >> (i * 2)) & 3)
                {
                    case 0:
                        this->paletteBG[i] = 255;
                        break;
                    case 1:
                        this->paletteBG[i] = 192;
                        break;
                    case 2:
                        this->paletteBG[i] = 96;
                        break;
                    case 3:
                        this->paletteBG[i] = 0;
                        break;
                }
            }
            break;
        case 8:
            for(int i = 0; i < 4; i++)
            {
                switch((value >> (i * 2)) & 3)
                {
                case 0:
                    this->paletteObj0[i] = 255;
                    break;
                case 1:
                    this->paletteObj0[i] = 192;
                    break;
                case 2:
                    this->paletteObj0[i] = 96;
                    break;
                case 3:
                    this->paletteObj0[i] = 0;
                    break;
                }
            }
            break;
        case 9:
            for(int i = 0; i < 4; i++)
            {
                switch((value >> (i * 2)) & 3)
                {
                    case 0:
                        this->paletteObj1[i] = 255;
                        break;
                    case 1:
                        this->paletteObj1[i] = 192;
                        break;
                    case 2:
                        this->paletteObj1[i] = 96;
                        break;
                    case 3:
                        this->paletteObj1[i] = 0;
                        break;
                }
            }
            break;
    }
}

void GPU::UpdateTile(uint16_t addr)
{

}

void GPU::UpdateOAM(uint16_t addr)
{

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
        this->sprites[i].y = -16;
        this->sprites[i].x = -8;
        this->sprites[i].tile = 0;
        this->sprites[i].palette = 0;
        this->sprites[i].xflip = 0;
        this->sprites[i].yflip = 0;
        this->sprites[i].prio = 0;
        this->sprites[i].num = i;
    }
}

void GPU::buildobjdata(uint16_t address, uint8_t val){
    int obj = address >> 2;
    if(obj < 40){
        switch(address & 3){
            //Y-coord
            case 0:
                this->sprites[obj].y = val-16;
                break;
            //X-coord
            case 1:
                this->sprites[obj].x = val-8;
                break;
            //Data tile
            case 2:
                this->sprites[obj].tile = val;
                break;
            //Options
            case 3:
                this->sprites[obj].palette = (val & 0x10) ? 1 : 0;
                this->sprites[obj].xflip = (val & 0x20) ? 1 : 0;
                this->sprites[obj].yflip = (val & 0x40) ? 1 : 0;
                this->sprites[obj].prio = (val & 0x80) ? 1 : 0;
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
        color = (((uint16_t)this->paletteBG[cindex * 2 + 1]) << 8) | this->paletteBG[cindex * 2];
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
