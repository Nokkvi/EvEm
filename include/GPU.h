#ifndef GPU_H
#define GPU_H

#include "Definitions.h"
#include "Register.h"
#include "Screen.h"
#include "objdata.h"

class GPU
{
    private:
        Screen* screen;

        uint8_t ***tileset;
        uint8_t vram[0xFFFF], oam[0x100], reg[0xFFFF], paletteBG[4], paletteObj0[4], paletteObj1[4];
        uint8_t mode, line, raster;
        Register *scx, *scy;
        unsigned int clock;
        bool switchBG, bgMap, bgTile, switchLCD, objSize, switchObj;
    public:
        GPU();
        virtual ~GPU();

        uint8_t GetVram(uint16_t addr);
        void SetVram(uint16_t addr, uint8_t value);

        uint8_t GetOAM(uint16_t addr);
        void SetOAM(uint16_t addr, uint8_t value);

        uint8_t GetData(uint16_t addr);
        void SetData(uint16_t addr, uint8_t value);

        uint8_t Read(uint16_t addr);

        void UpdateTile(uint16_t addr);
        void UpdateOAM(uint16_t addr);
        void Reset();
        void Step(unsigned int clock);
        void Render();
        void buildobjdata(uint16_t address, uint8_t val);
    private:
        void Scanline();
};

#endif // GPU_H
