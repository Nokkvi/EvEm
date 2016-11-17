#include "../include/Memory.h"

#include <iostream>
#include <fstream>
#include <cstddef>

Memory::Memory(GPU* gpu)
{
    this->gpu = gpu;
    Reset();
}

Memory::~Memory()
{

}

void Memory::Reset()
{
    inBIOS = true;
    romOffset = 0x4000;
    ramOffset = 0x0000;
    cartType = 0;
    ie = 0;
    interruptFlags = 0;
    ramBank = 0;
    romBank = 0;
    mode = false;
    ramOn = false;
}

void Memory::LoadBIOS(std::string path)
{
    std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);
	if (!file)
        throw -1;

	uint8_t *pointer = &bios[0];
	while (!file.eof())
	{
        file >> *pointer;
        pointer++;
	}
}

void Memory::LoadROM(std::string path)
{
    std::ifstream file;
	file.open(path, std::ios::in | std::ios::binary);
	if (!file)
        throw -1;

	uint8_t *pointer = &rom[0];
	while (!file.eof())
	{
        file >> *pointer;
        pointer++;
	}

	this->cartType = bios[0x0147];
}

uint8_t Memory::GetByte(uint16_t index)
{

    switch(index & 0xF000)
    {
        case 0x0000:
            if(this->inBIOS)
            {
                if(index < 0x0100)
                    return this->bios[index];
                else if(index == 0x0100)
                    this->inBIOS = 0;
            }
            else
                return this->rom[index];
        case 0x1000:
        case 0x2000:
        case 0x3000:
            return this->rom[index];
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return this->rom[romOffset + (index & 0x3FFF)];
        case 0x8000:
        case 0x9000:
            return this->gpu->GetVram(index & 0x1FFF);
        case 0xA000:
        case 0xB000:
            return this->eram[ramOffset + (index & 0x1FFF)];
        case 0xC000:
        case 0xD000:
        case 0xE000:
            return this->wram[index & 0x1FFF];
        case 0xF000:
            switch(index & 0x0F00)
            {
                case 0xF00:
                    if(index == 0xFFFF)
                        return this->ie;
                    else if(index > 0xFF7F)
                        return this->zram[index & 0x7F];
                    else
                        switch(index & 0xF0)
                        {
                            case 0x00:
                                switch(index & 0xF)
                                {
                                    case 0:
                                        // return KEY.rb();
                                    case 4:
                                    case 5:
                                    case 6:
                                    case 7:
                                        // return TIMER.rb(index);
                                    case 15:
                                        return this->interruptFlags;
                                    default:
                                        return 0;
                                }
                            case 0x10:
                            case 0x20:
                            case 0x30:
                                return 0;
                            case 0x40:
                            case 0x50:
                            case 0x60:
                            case 0x70:
                                return this->gpu->GetData(index);
                        }
                    break;
                case 0xE00:
                    return ((index & 0xFF) < 0xA0) ? this->gpu->GetOAM(index & 0xFF) : 0;
                default:
                    return this->wram[index & 0x1FFF];
            }
            break;
    }
    return 0;
};

uint16_t Memory::GetWord(uint16_t index)
{
    return (((uint16_t)this->GetByte(index + 1)) << 8) + this->GetByte(index);
};

void Memory::SetByte(uint16_t index, uint8_t value)
{
    switch(index & 0xF000)
    {
        case 0x0000:
        case 0x1000:
            switch(this->cartType)
            {
                case 1:
                    this->ramOn = (value & 0xF) == 0xA ? 1 : 0;
                break;
            }
            break;
        case 0x2000:
        case 0x3000:
            switch(this->cartType)
            {
                case 1:
                    this->romBank &= 0x60;
                    value &= 0x1F;
                    if(!value)
                        value = 1;

                    this->romBank |= value;
                    this->romOffset = this->romBank * 0x4000;
                    break;
            }
            break;
        case 0x4000:
        case 0x5000:
            switch(this->cartType)
            {
                case 1:
                    if(this->mode)
                    {
                        this->ramBank = value & 3;
                        this->ramOffset = this->ramBank * 0x2000;
                    }
                    else
                    {
                        this->romBank &= 0x1F;
                        this->romBank |= ((value & 3) << 5);
                        this->romOffset = this->romBank * 0x4000;
                    }
                    break;
            }
            break;
        case 0x6000:
        case 0x7000:
            switch(this->cartType)
            {
                case 1:
                    this->mode = (value & 1) == 1;
                break;
            }
            break;
        case 0x8000:
        case 0x9000:
            this->gpu->SetVram(index & 0x1FFF, value);
            this->gpu->UpdateTile(index & 0x1FFF);
            break;
        case 0xA000:
        case 0xB000:
            this->eram[this->ramOffset + (index & 0x1FFF)] = value;
            break;
        case 0xC000:
        case 0xD000:
        case 0xE000:
            this->wram[index & 0x1FFF] = value;
            break;
        case 0xF000:
            switch(index & 0x0F00)
            {
                case 0xF00:
                    if(index == 0xFFFF)
                        this->ie = value;
                    else if(index > 0xFF7F)
                        this->zram[index & 0x7F] = value;
                    else
                        switch(index & 0xF0)
                        {
                            case 0x00:
                                switch(index & 0xF)
                                {
                                    case 0:
                                        //KEY.wb(val);
                                        break;
                                    case 4:
                                    case 5:
                                    case 6:
                                    case 7:
                                        //TIMER.wb(index, val);
                                        break;
                                    case 15:
                                        this->interruptFlags = value;
                                        break;
                                }
                                break;
                            case 0x40:
                            case 0x50:
                            case 0x60:
                            case 0x70:
                                this->gpu->SetData(index, value);
                                break;
                        }
                    break;
                case 0xE00:
                    index = index & 0xFF;
                    if (index < 0xA0)
                        this->gpu->SetOAM(index, value);

                    this->gpu->UpdateOAM(index);
                    break;
                default:
                    this->wram[index & 0x1FFF] = value;
            }
            break;
    }
};

void Memory::SetWord(uint16_t index, uint16_t value)
{
    this->SetByte(index, value & 0xFF);
    this->SetByte(index + 1, value >> 8);
};
