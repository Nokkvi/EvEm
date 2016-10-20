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
        void SetHex(int index, uint8_t hex);
        void SetByte(int index, uint8_t byte);
        uint8_t GetHex(int index);
        uint8_t GetByte(int index);
        int GetSize();

    protected:

    private:
};

#endif // REGISTER_H
