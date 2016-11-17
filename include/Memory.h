#ifndef MEMORY_H
#define MEMORY_H

#include "Definitions.h"
#include "GPU.h"
#include <string>

class Memory
{
    private:
        GPU* gpu;
        bool inBIOS, ramOn, mode;
        uint8_t bios[0xFF], wram[0x2000], eram[0x8000], rom[128 * 0x1FFF], zram[0x800];
        unsigned int romOffset, ramOffset, cartType, ie, interruptFlags, romBank, ramBank;
    public:
        Memory(GPU* gpu);
        virtual ~Memory();

        inline void SetBIOS(bool on)
        {
            inBIOS = on;
        };

        void Reset();

        void LoadBIOS(std::string path);
        void LoadROM(std::string path);

        uint8_t GetByte(uint16_t index);
        uint16_t GetWord(uint16_t index);
        void SetByte(uint16_t index, uint8_t value);
        void SetWord(uint16_t index, uint16_t value);
    private:
        uint16_t Indexer(uint16_t index);
};

#endif // MEMORY_H
