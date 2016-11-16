#ifndef REGISTER_H
#define REGISTER_H

#include "Definitions.h"

class Register
{
    private:
        int size;
        uint8_t* storage;
    public:
        Register(int sizeInBytes);
        virtual ~Register();
        bool Increment();
        bool Decrement();
        void SetBit(int index, bool bit);
        void SetHex(int index, uint8_t hex);
        void SetByte(int index, uint8_t byte);
        void SetWord(int index, uint16_t word);
        bool GetBit(int index);
        uint8_t GetHex(int index);
        uint8_t GetByte(int index);
        uint16_t GetWord(int index);
        int GetSize();

    protected:

    private:
};

#endif // REGISTER_H
