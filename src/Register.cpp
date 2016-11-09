#include "../include/Register.h"

Register::Register(int sizeInBytes)
{
    size = sizeInBytes;
    storage = new uint8_t[sizeInBytes];
}

Register::~Register()
{
    delete[] storage;
}

int Register::GetSize()
{
    return size;
}

void Register::Increment()
{
    for (int i = 0; i < size; i++)
    {
        if (storage[i] == 0xFF)
            storage[i] = 0;
        else
        {
            storage[i]++;
            break;
        }
    }
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

void Register::SetByte(int index, uint8_t byte)
{
    storage[index] = byte;
}

void Register::SetWord(int index, uint16_t word)
{
    int realIndex = index * 2;
    storage[realIndex] = word & 0x00FF;
    storage[realIndex + 1] = word >> 8;
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

uint16_t Register::GetWord(int index)
{
    int realIndex = index * 2;
    return ((uint16_t)storage[realIndex + 1] << 8) | storage[realIndex];
}
