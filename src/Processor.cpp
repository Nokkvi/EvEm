#include "Processor.h"

Processor::Processor(Memory* pMemory)
{
    m_pMemory = pMemory;
    pMemory->SetProcessor(this);
    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    F = 0;
    H = 0;
    L = 0;
    SP = 0;
    PC = 0;
}

Processor::~Processor()
{

}

void Processor::8Bit_Add(Register B){
    this->A += B; //Addition
    this->F = 0;
    if(!(this->A & 256)) this->F |= 0x80;
    if(this->A > 256) this->F |= 0x10;
    this->M = 1;
    this->T = 4;
}

void Processor::8Bit_Add_Mem(){
    Registers adderAddress(16);
    adderAddress = ((adderAddress | this->H) << 8) | this->L;
}
