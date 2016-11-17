#ifndef MEMORY_H
#define MEMORY_H

#include "Definitions.h"
#include "GPU.h"
#include <string>

class Memory
{
    private:
        GPU* gpu;
        bool inBIOS;
        uint8_t storage[65535];
    public:
        Memory(GPU* gpu);
        virtual ~Memory();

        inline void SetBIOS(bool on)
        {
            inBIOS = on;
        };

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
