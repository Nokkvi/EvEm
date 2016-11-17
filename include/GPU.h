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
        uint8_t * vram;
        uint8_t mode, line;
        Register *pal, *scx, *scy;
        unsigned int clock;
        bool switchBG, bgMap, bgTile, switchLCD;

        uint8_t* oam;
        objdata* objectdata;
    public:
        GPU();
        virtual ~GPU();

        uint8_t GetVram(uint16_t addr);
        void SetVram(uint16_t addr, uint8_t value);
        void UpdateTile(uint16_t addr);
        void Reset();
        void Step(unsigned int clock);
        void Render();
        void buildobjdata(uint16_t address, uint8_t val);
    private:
        void Scanline();
};

#endif // GPU_H
