#include "Registers.h"

Registers::Registers(int sizeInBytes)
{
    size = sizeInHex;
    storage = new uint8_t[sizeInBytes];
}

Registers::~Registers()
{
    delete[] storage;
}

int Register::GetSize()
{
    return size;
}

void Register::SetHex(int index, uint8_t hex)
{
    // Get the index of the byte the hex is in.
    int realIndex = index / 2;
    // 1 is later half of that byte while 0 is the first one.
    int mask = index % 2;
    // The number of bytes we should shift once we are done using the mask.
    int offset = mask == 0 ? 4 : 0;
    // Set the bitmask we are going to use according to the old value in mask.
    mask = mask == 0 ? 0x0F : 0xF0;

    storage[realIndex] = (storage[realIndex] & (0xFF - mask)) | ((hex & 0x0F) << offset);
}

void RegisterSetByte(int index, uint8_t byte)
{
    storage[index] = byte;
}

uint8_t Register::GetHex(int index)
{
    int realIndex = index / 2;
    int mask = index % 2;
    int offset = mask == 0 ? 4 : 0;
    mask = mask == 0 ? 0x0F : 0xF0;

    return (storage[realIndex] & mask) >> offset;
}

uint8_t Register::GetByte(int index)
{
    return storage[index];
}
