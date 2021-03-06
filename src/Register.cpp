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
bool Register::Increment()
{
    for (int i = 0; i < size; i++)
    {
        if (storage[i] == 0xFF)
            storage[i] = 0x00;
        else
        {
            storage[i]++;
            return false;
        }
    }

    return true;
}

bool Register::Decrement()
{
    for (int i = 0; i < size; i++)
    {
        if (storage[i] == 0x00)
            storage[i] = 0xFF;
        else
        {
            storage[i]--;
            return false;
        }
    }

    return true;
}

void Register::SetBit(int index, bool bit)
{
    int realIndex = index / 8;
    int shift = index % 8;
    if (bit)
        storage[realIndex] |= 0x01 << shift;
    else
        storage[realIndex] &= 0xFF - (0x01 << shift);
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
    storage[index] = word & 0x00FF;
    storage[index + 1] = word >> 8;
}

bool Register::GetBit(int index)
{
    int realIndex = index / 8;
    int shift = index % 8;
    return (storage[realIndex] & (0x01 << shift)) != 0;
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
    return ((uint16_t)storage[index + 1] << 8) | storage[index];
}

