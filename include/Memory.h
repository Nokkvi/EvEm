#ifndef MEMORY_H
#define MEMORY_H

#include "Definitions.h"

class Memory
{
    private:
        uint8_t storage[65535];
    public:
        Memory();
        virtual ~Memory();

        inline uint8_t GetByte(uint16_t index)
        {
            return storage[index];
        };

        inline uint16_t GetWord(uint16_t index)
        {
            return (((uint16_t)storage[index]) << 8) | storage[index + 1];
        };

        inline void SetByte(uint16_t index, uint8_t value)
        {
            storage[index] = value;
        };

        inline void SetWord(uint16_t index, uint16_t value)
        {
            storage[index] = ((uint8_t)value);
            storage[index + 1] = ((uint8_t)(value >> 8));
        };
};

#endif // MEMORY_H
