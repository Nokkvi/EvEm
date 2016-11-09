#include "../include/Processor.h"

uint8_t Processor::FetchInstruction()
{
    uint16_t pc = this->PC->GetWord(0);
    this->PC->Increment();
    if (pc == 0x1000)
    {
        memory->SetBIOS(false);
    }
    return memory->GetByte(pc);
};

void Processor::ProcessOpcode(uint8_t code)
{
    (*operations)[code](this);
    if (this->cbInstruction)
    {
        this->cbInstruction = false;
        (*cboperations)[FetchInstruction()](this);
    }

    this->mTotal += this->M->GetByte(0);
    this->tTotal += this->T->GetByte(0);
}

void Processor::InitOpcodes()
{
    operations = new std::unordered_map<uint8_t, std::function<void(Processor* p)>>();
    (*operations)[0x00] = [](Processor* p) {
        p->NOP();
    };
    (*operations)[0x01] = [](Processor* p) {
        uint16_t immediate = ((uint16_t)p->PC->GetByte(1) << 8) | p->PC->GetByte(0);
        p->Load(p->B, p->C, p->memory->GetByte(p->PC->GetWord(0)));
    };
    (*operations)[0x02] = [](Processor* p) {
        p->Store(p->A, p->B, p->C);
    };
    (*operations)[0x03] = [](Processor* p) {
        p->INC(p->B, p->C);
    };
    (*operations)[0x04] = [](Processor* p) {
        p->INC(p->B);
    };
    (*operations)[0x05] = [](Processor* p) {
        p->DEC(p->B);
    };
    (*operations)[0x06] = [](Processor* p) {
        p->Load(p->B, p->memory->GetByte(p->PC->GetWord(0)));
        p->PC->Increment();
    };
    (*operations)[0x07] = [](Processor* p) {
        p->RLC(p->A);
    };
    (*operations)[0x08] = [](Processor* p) {
        uint16_t word = p->PC->GetWord(0);
        p->Store(p->SP, p->memory->GetWord(word));
        p->PC->SetWord(0, word + 2);
    };
    (*operations)[0x09] = [](Processor* p) {
        p->ADD(p->B, p->C, p->H, p->L);
    };
    (*operations)[0x0A] = [](Processor* p) {
        p->Load(p->A, p->B, p->C);
    };
    (*operations)[0x0B] = [](Processor* p) {
        p->DEC(p->B, p->C);
    };
    (*operations)[0x0C] = [](Processor* p) {
        p->INC(p->C);
    };
    (*operations)[0x0D] = [](Processor* p) {
        p->DEC(p->C);
    };
    (*operations)[0x0E] = [](Processor* p) {
        p->Load(p->C, p->memory->GetByte(p->PC->GetWord(0)));
        p->PC->Increment();
    };
    (*operations)[0x0F] = [](Processor* p) {
        p->RRC(p->A);
    };
    (*operations)[0x10] = [](Processor* p) {
        p->STOP();
    };
    (*operations)[0x11] = [](Processor* p) {
        uint16_t immediate = ((uint16_t)p->PC->GetByte(1) << 8) | p->PC->GetByte(0);
        p->Load(p->D, p->E, immediate);
    };
    (*operations)[0x12] = [](Processor* p) {

    };
    (*operations)[0x13] = [](Processor* p) {

    };
    (*operations)[0x14] = [](Processor* p) {

    };
    (*operations)[0x15] = [](Processor* p) {

    };
    (*operations)[0x16] = [](Processor* p) {

    };
    (*operations)[0x17] = [](Processor* p) {

    };
    (*operations)[0x18] = [](Processor* p) {

    };
    (*operations)[0x19] = [](Processor* p) {

    };
    (*operations)[0x20] = [](Processor* p) {

    };
    (*operations)[0x21] = [](Processor* p) {

    };
    (*operations)[0x80] = [](Processor* p) {
        p->ADD(p->A, p->B);
    };

    cboperations = new std::unordered_map<uint8_t, std::function<void(Processor* p)>>();
    (*cboperations)[0x00] = [](Processor* p) {
        p->RLC(p->B);
    };
    (*cboperations)[0x01] = [](Processor* p) {
        p->RLC(p->C);
    };
    (*cboperations)[0x02] = [](Processor* p) {
        p->RLC(p->D);
    };
    (*cboperations)[0x03] = [](Processor* p) {
        p->RLC(p->E);
    };
    (*cboperations)[0x04] = [](Processor* p) {
        p->RLC(p->H);
    };
    (*cboperations)[0x05] = [](Processor* p) {
        p->RLC(p->L);
    };
    (*cboperations)[0x06] = [](Processor* p) {
        p->RLCHL();
    };
    (*cboperations)[0x07] = [](Processor* p) {
        p->RLC(p->A);
    };
    (*cboperations)[0x08] = [](Processor* p) {
        p->RRC(p->B);
    };
    (*cboperations)[0x09] = [](Processor* p) {
        p->RRC(p->C);
    };
    (*cboperations)[0x0A] = [](Processor* p) {
        p->RRC(p->D);
    };
    (*cboperations)[0x0B] = [](Processor* p) {
        p->RRC(p->D);
    };
    (*cboperations)[0x0C] = [](Processor* p) {
        p->RRC(p->H);
    };
    (*cboperations)[0x0D] = [](Processor* p) {
        p->RRC(p->L);
    };
    (*cboperations)[0x0E] = [](Processor* p) {
        p->RRCHL();
    };
    (*cboperations)[0x0F] = [](Processor* p) {
        p->RRC(p->A);
    };
    (*cboperations)[0x10] = [](Processor* p) {
        p->RL(p->B);
    };
    (*cboperations)[0x11] = [](Processor* p) {
        p->RL(p->C);
    };
    (*cboperations)[0x12] = [](Processor* p) {
        p->RL(p->D);
    };
    (*cboperations)[0x13] = [](Processor* p) {
        p->RL(p->E);
    };
    (*cboperations)[0x14] = [](Processor* p) {
        p->RL(p->H);
    };
    (*cboperations)[0x15] = [](Processor* p) {
        p->RL(p->L);
    };
    (*cboperations)[0x16] = [](Processor* p) {
        p->RLHL();
    };
    (*cboperations)[0x17] = [](Processor* p) {
        p->RL(p->A);
    };
    (*cboperations)[0x18] = [](Processor* p) {
        p->RR(p->B);
    };
    (*cboperations)[0x19] = [](Processor* p) {
        p->RR(p->C);
    };
    (*cboperations)[0x1A] = [](Processor* p) {
        p->RR(p->D);
    };
    (*cboperations)[0x1B] = [](Processor* p) {
        p->RR(p->E);
    };
    (*cboperations)[0x1C] = [](Processor* p) {
        p->RR(p->H);
    };
    (*cboperations)[0x1D] = [](Processor* p) {
        p->RR(p->L);
    };
    (*cboperations)[0x1E] = [](Processor* p) {
        p->RRHL();
    };
    (*cboperations)[0x1F] = [](Processor* p) {
        p->RR(p->A);
    };
    (*cboperations)[0x20] = [](Processor* p) {
        p->SLA(p->B);
    };
    (*cboperations)[0x21] = [](Processor* p) {
        p->SLA(p->C);
    };
    (*cboperations)[0x22] = [](Processor* p) {
        p->SLA(p->D);
    };
    (*cboperations)[0x23] = [](Processor* p) {
        p->SLA(p->E);
    };
    (*cboperations)[0x24] = [](Processor* p) {
        p->SLA(p->H);
    };
    (*cboperations)[0x25] = [](Processor* p) {
        p->SLA(p->L);
    };
    (*cboperations)[0x26] = [](Processor* p) {
        p->SLA();
    };
    (*cboperations)[0x27] = [](Processor* p) {
        p->SLA(p->A);
    };
    (*cboperations)[0x28] = [](Processor* p) {
        p->SRA(p->B);
    };
    (*cboperations)[0x29] = [](Processor* p) {
        p->SRA(p->C);
    };
    (*cboperations)[0x2A] = [](Processor* p) {
        p->SRA(p->D);
    };
    (*cboperations)[0x2B] = [](Processor* p) {
        p->SRA(p->E);
    };
    (*cboperations)[0x2C] = [](Processor* p) {
        p->SRA(p->H);
    };
    (*cboperations)[0x2D] = [](Processor* p) {
        p->SRA(p->L);
    };
    (*cboperations)[0x2E] = [](Processor* p) {
        p->SRA();
    };
    (*cboperations)[0x2F] = [](Processor* p) {
        p->SRA(p->A);
    };
    (*cboperations)[0x30] = [](Processor* p) {
        p->SWAP(p->B);
    };
    (*cboperations)[0x31] = [](Processor* p) {
        p->SWAP(p->C);
    };
    (*cboperations)[0x32] = [](Processor* p) {
        p->SWAP(p->D);
    };
    (*cboperations)[0x33] = [](Processor* p) {
        p->SWAP(p->E);
    };
    (*cboperations)[0x34] = [](Processor* p) {
        p->SWAP(p->H);
    };
    (*cboperations)[0x35] = [](Processor* p) {
        p->SWAP(p->L);
    };
    (*cboperations)[0x36] = [](Processor* p) {
        p->SWAP();
    };
    (*cboperations)[0x37] = [](Processor* p) {
        p->SWAP(p->A);
    };
    (*cboperations)[0x38] = [](Processor* p) {
        p->SRL(p->B);
    };
    (*cboperations)[0x39] = [](Processor* p) {
        p->SRL(p->C);
    };
    (*cboperations)[0x3A] = [](Processor* p) {
        p->SRL(p->D);
    };
    (*cboperations)[0x3B] = [](Processor* p) {
        p->SRL(p->E);
    };
    (*cboperations)[0x3C] = [](Processor* p) {
        p->SRL(p->H);
    };
    (*cboperations)[0x3D] = [](Processor* p) {
        p->SRL(p->L);
    };
    (*cboperations)[0x3E] = [](Processor* p) {
        p->SRL();
    };
    (*cboperations)[0x3F] = [](Processor* p) {
        p->SRL(p->A);
    };
    (*cboperations)[0x40] = [](Processor* p) {
        p->BIT(0, p->B);
    };
    (*cboperations)[0x41] = [](Processor* p) {
        p->BIT(0, p->C);
    };
    (*cboperations)[0x42] = [](Processor* p) {
        p->BIT(0, p->D);
    };
    (*cboperations)[0x43] = [](Processor* p) {
        p->BIT(0, p->E);
    };
    (*cboperations)[0x44] = [](Processor* p) {
        p->BIT(0, p->H);
    };
    (*cboperations)[0x45] = [](Processor* p) {
        p->BIT(0, p->L);
    };
    (*cboperations)[0x46] = [](Processor* p) {
        p->BIT(0);
    };
    (*cboperations)[0x47] = [](Processor* p) {
        p->BIT(0, p->A);
    };
    (*cboperations)[0x48] = [](Processor* p) {
        p->BIT(1, p->B);
    };
    (*cboperations)[0x49] = [](Processor* p) {
        p->BIT(1, p->C);
    };
    (*cboperations)[0x4A] = [](Processor* p) {
        p->BIT(1, p->D);
    };
    (*cboperations)[0x4B] = [](Processor* p) {
        p->BIT(1, p->E);
    };
    (*cboperations)[0x4C] = [](Processor* p) {
        p->BIT(1, p->H);
    };
    (*cboperations)[0x4D] = [](Processor* p) {
        p->BIT(1, p->L);
    };
    (*cboperations)[0x4E] = [](Processor* p) {
        p->BIT(1);
    };
    (*cboperations)[0x4F] = [](Processor* p) {
        p->BIT(1, p->A);
    };
    (*cboperations)[0x50] = [](Processor* p) {
        p->BIT(2, p->B);
    };
    (*cboperations)[0x51] = [](Processor* p) {
        p->BIT(2, p->C);
    };
    (*cboperations)[0x52] = [](Processor* p) {
        p->BIT(2, p->D);
    };
    (*cboperations)[0x53] = [](Processor* p) {
        p->BIT(2, p->E);
    };
    (*cboperations)[0x54] = [](Processor* p) {
        p->BIT(2, p->H);
    };
    (*cboperations)[0x55] = [](Processor* p) {
        p->BIT(2, p->L);
    };
    (*cboperations)[0x56] = [](Processor* p) {
        p->BIT(2);
    };
    (*cboperations)[0x57] = [](Processor* p) {
        p->BIT(2, p->A);
    };
    (*cboperations)[0x58] = [](Processor* p) {
        p->BIT(3, p->B);
    };
    (*cboperations)[0x59] = [](Processor* p) {
        p->BIT(3, p->C);
    };
    (*cboperations)[0x5A] = [](Processor* p) {
        p->BIT(3, p->D);
    };
    (*cboperations)[0x5B] = [](Processor* p) {
        p->BIT(3, p->E);
    };
    (*cboperations)[0x5C] = [](Processor* p) {
        p->BIT(3, p->H);
    };
    (*cboperations)[0x5D] = [](Processor* p) {
        p->BIT(3, p->L);
    };
    (*cboperations)[0x5E] = [](Processor* p) {
        p->BIT(3);
    };
    (*cboperations)[0x5F] = [](Processor* p) {
        p->BIT(3, p->A);
    };

    (*cboperations)[0x60] = [](Processor* p) {
        p->BIT(4, p->B);
    };
    (*cboperations)[0x61] = [](Processor* p) {
        p->BIT(4, p->C);
    };
    (*cboperations)[0x62] = [](Processor* p) {
        p->BIT(4, p->D);
    };
    (*cboperations)[0x63] = [](Processor* p) {
        p->BIT(4, p->E);
    };
    (*cboperations)[0x64] = [](Processor* p) {
        p->BIT(4, p->H);
    };
    (*cboperations)[0x65] = [](Processor* p) {
        p->BIT(4, p->L);
    };
    (*cboperations)[0x66] = [](Processor* p) {
        p->BIT(4);
    };
    (*cboperations)[0x67] = [](Processor* p) {
        p->BIT(4, p->A);
    };
    (*cboperations)[0x68] = [](Processor* p) {
        p->BIT(5, p->B);
    };
    (*cboperations)[0x69] = [](Processor* p) {
        p->BIT(5, p->C);
    };
    (*cboperations)[0x6A] = [](Processor* p) {
        p->BIT(5, p->D);
    };
    (*cboperations)[0x6B] = [](Processor* p) {
        p->BIT(5, p->E);
    };
    (*cboperations)[0x6C] = [](Processor* p) {
        p->BIT(5, p->H);
    };
    (*cboperations)[0x6D] = [](Processor* p) {
        p->BIT(5, p->L);
    };
    (*cboperations)[0x6E] = [](Processor* p) {
        p->BIT(5);
    };
    (*cboperations)[0x6F] = [](Processor* p) {
        p->BIT(5, p->A);
    };
    (*cboperations)[0x70] = [](Processor* p) {
        p->BIT(6, p->B);
    };
    (*cboperations)[0x71] = [](Processor* p) {
        p->BIT(6, p->C);
    };
    (*cboperations)[0x72] = [](Processor* p) {
        p->BIT(6, p->D);
    };
    (*cboperations)[0x73] = [](Processor* p) {
        p->BIT(6, p->E);
    };
    (*cboperations)[0x74] = [](Processor* p) {
        p->BIT(6, p->H);
    };
    (*cboperations)[0x75] = [](Processor* p) {
        p->BIT(6, p->L);
    };
    (*cboperations)[0x76] = [](Processor* p) {
        p->BIT(6);
    };
    (*cboperations)[0x77] = [](Processor* p) {
        p->BIT(6, p->A);
    };
    (*cboperations)[0x78] = [](Processor* p) {
        p->BIT(7, p->B);
    };
    (*cboperations)[0x79] = [](Processor* p) {
        p->BIT(7, p->C);
    };
    (*cboperations)[0x7A] = [](Processor* p) {
        p->BIT(7, p->D);
    };
    (*cboperations)[0x7B] = [](Processor* p) {
        p->BIT(7, p->E);
    };
    (*cboperations)[0x7C] = [](Processor* p) {
        p->BIT(7, p->H);
    };
    (*cboperations)[0x7D] = [](Processor* p) {
        p->BIT(7, p->L);
    };
    (*cboperations)[0x7E] = [](Processor* p) {
        p->BIT(7);
    };
    (*cboperations)[0x7F] = [](Processor* p) {
        p->BIT(7, p->A);
    };
    (*cboperations)[0x80] = [](Processor* p) {
        p->RES(0, p->B);
    };
    (*cboperations)[0x81] = [](Processor* p) {
        p->RES(0, p->C);
    };
    (*cboperations)[0x82] = [](Processor* p) {
        p->RES(0, p->D);
    };
    (*cboperations)[0x83] = [](Processor* p) {
        p->RES(0, p->E);
    };
    (*cboperations)[0x84] = [](Processor* p) {
        p->RES(0, p->H);
    };
    (*cboperations)[0x85] = [](Processor* p) {
        p->RES(0, p->L);
    };
    (*cboperations)[0x86] = [](Processor* p) {
        p->RES(0);
    };
    (*cboperations)[0x87] = [](Processor* p) {
        p->RES(0, p->A);
    };
    (*cboperations)[0x88] = [](Processor* p) {
        p->RES(1, p->B);
    };
    (*cboperations)[0x89] = [](Processor* p) {
        p->RES(1, p->C);
    };
    (*cboperations)[0x8A] = [](Processor* p) {
        p->RES(1, p->D);
    };
    (*cboperations)[0x8B] = [](Processor* p) {
        p->RES(1, p->E);
    };
    (*cboperations)[0x8C] = [](Processor* p) {
        p->RES(1, p->H);
    };
    (*cboperations)[0x8D] = [](Processor* p) {
        p->RES(1, p->L);
    };
    (*cboperations)[0x8E] = [](Processor* p) {
        p->RES(1);
    };
    (*cboperations)[0x8F] = [](Processor* p) {
        p->RES(1, p->A);
    };
    (*cboperations)[0x90] = [](Processor* p) {
        p->RES(2, p->B);
    };
    (*cboperations)[0x91] = [](Processor* p) {
        p->RES(2, p->C);
    };
    (*cboperations)[0x92] = [](Processor* p) {
        p->RES(2, p->D);
    };
    (*cboperations)[0x93] = [](Processor* p) {
        p->RES(2, p->E);
    };
    (*cboperations)[0x94] = [](Processor* p) {
        p->RES(2, p->H);
    };
    (*cboperations)[0x95] = [](Processor* p) {
        p->RES(2, p->L);
    };
    (*cboperations)[0x96] = [](Processor* p) {
        p->RES(2);
    };
    (*cboperations)[0x97] = [](Processor* p) {
        p->RES(2, p->A);
    };
    (*cboperations)[0x98] = [](Processor* p) {
        p->RES(3, p->B);
    };
    (*cboperations)[0x99] = [](Processor* p) {
        p->RES(3, p->C);
    };
    (*cboperations)[0x9A] = [](Processor* p) {
        p->RES(3, p->D);
    };
    (*cboperations)[0x9B] = [](Processor* p) {
        p->RES(3, p->E);
    };
    (*cboperations)[0x9C] = [](Processor* p) {
        p->RES(3, p->H);
    };
    (*cboperations)[0x9D] = [](Processor* p) {
        p->RES(3, p->L);
    };
    (*cboperations)[0x9E] = [](Processor* p) {
        p->RES(3);
    };
    (*cboperations)[0x9F] = [](Processor* p) {
        p->RES(3, p->A);
    };
    (*cboperations)[0xA0] = [](Processor* p) {
        p->RES(4, p->B);
    };
    (*cboperations)[0xA1] = [](Processor* p) {
        p->RES(4, p->C);
    };
    (*cboperations)[0xA2] = [](Processor* p) {
        p->RES(4, p->D);
    };
    (*cboperations)[0xA3] = [](Processor* p) {
        p->RES(4, p->E);
    };
    (*cboperations)[0xA4] = [](Processor* p) {
        p->RES(4, p->H);
    };
    (*cboperations)[0xA5] = [](Processor* p) {
        p->RES(4, p->L);
    };
    (*cboperations)[0xA6] = [](Processor* p) {
        p->RES(4);
    };
    (*cboperations)[0xA7] = [](Processor* p) {
        p->RES(4, p->A);
    };
    (*cboperations)[0xA8] = [](Processor* p) {
        p->RES(5, p->B);
    };
    (*cboperations)[0xA9] = [](Processor* p) {
        p->RES(5, p->C);
    };
    (*cboperations)[0xAA] = [](Processor* p) {
        p->RES(5, p->D);
    };
    (*cboperations)[0xAB] = [](Processor* p) {
        p->RES(5, p->E);
    };
    (*cboperations)[0xAC] = [](Processor* p) {
        p->RES(5, p->H);
    };
    (*cboperations)[0xAD] = [](Processor* p) {
        p->RES(5, p->L);
    };
    (*cboperations)[0xAE] = [](Processor* p) {
        p->RES(5);
    };
    (*cboperations)[0xAF] = [](Processor* p) {
        p->RES(5, p->A);
    };
    (*cboperations)[0xB0] = [](Processor* p) {
        p->RES(6, p->B);
    };
    (*cboperations)[0xB1] = [](Processor* p) {
        p->RES(6, p->C);
    };
    (*cboperations)[0xB2] = [](Processor* p) {
        p->RES(6, p->D);
    };
    (*cboperations)[0xB3] = [](Processor* p) {
        p->RES(6, p->E);
    };
    (*cboperations)[0xB4] = [](Processor* p) {
        p->RES(6, p->H);
    };
    (*cboperations)[0xB5] = [](Processor* p) {
        p->RES(6, p->L);
    };
    (*cboperations)[0xB6] = [](Processor* p) {
        p->RES(6);
    };
    (*cboperations)[0xB7] = [](Processor* p) {
        p->RES(6, p->A);
    };
    (*cboperations)[0xB8] = [](Processor* p) {
        p->RES(7, p->B);
    };
    (*cboperations)[0xB9] = [](Processor* p) {
        p->RES(7, p->C);
    };
    (*cboperations)[0xBA] = [](Processor* p) {
        p->RES(7, p->D);
    };
    (*cboperations)[0xBB] = [](Processor* p) {
        p->RES(7, p->E);
    };
    (*cboperations)[0xBC] = [](Processor* p) {
        p->RES(7, p->H);
    };
    (*cboperations)[0xBD] = [](Processor* p) {
        p->RES(7, p->L);
    };
    (*cboperations)[0xBE] = [](Processor* p) {
        p->RES(7);
    };
    (*cboperations)[0xBF] = [](Processor* p) {
        p->RES(7, p->A);
    };
    (*cboperations)[0xC0] = [](Processor* p) {
        p->SET(0, p->B);
    };
    (*cboperations)[0xC1] = [](Processor* p) {
        p->SET(0, p->C);
    };
    (*cboperations)[0xC2] = [](Processor* p) {
        p->SET(0, p->D);
    };
    (*cboperations)[0xC3] = [](Processor* p) {
        p->SET(0, p->E);
    };
    (*cboperations)[0xC4] = [](Processor* p) {
        p->SET(0, p->H);
    };
    (*cboperations)[0xC5] = [](Processor* p) {
        p->SET(0, p->L);
    };
    (*cboperations)[0xC6] = [](Processor* p) {
        p->SET(0);
    };
    (*cboperations)[0xC7] = [](Processor* p) {
        p->SET(0, p->A);
    };
    (*cboperations)[0xC8] = [](Processor* p) {
        p->SET(1, p->B);
    };
    (*cboperations)[0xC9] = [](Processor* p) {
        p->SET(1, p->C);
    };
    (*cboperations)[0xCA] = [](Processor* p) {
        p->SET(1, p->D);
    };
    (*cboperations)[0xCB] = [](Processor* p) {
        p->SET(1, p->E);
    };
    (*cboperations)[0xCC] = [](Processor* p) {
        p->SET(1, p->H);
    };
    (*cboperations)[0xCD] = [](Processor* p) {
        p->SET(1, p->L);
    };
    (*cboperations)[0xCE] = [](Processor* p) {
        p->SET(1);
    };
    (*cboperations)[0xCF] = [](Processor* p) {
        p->SET(1, p->A);
    };
    (*cboperations)[0xD0] = [](Processor* p) {
        p->SET(2, p->B);
    };
    (*cboperations)[0xD1] = [](Processor* p) {
        p->SET(2, p->C);
    };
    (*cboperations)[0xD2] = [](Processor* p) {
        p->SET(2, p->D);
    };
    (*cboperations)[0xD3] = [](Processor* p) {
        p->SET(2, p->E);
    };
    (*cboperations)[0xD4] = [](Processor* p) {
        p->SET(2, p->H);
    };
    (*cboperations)[0xD5] = [](Processor* p) {
        p->SET(2, p->L);
    };
    (*cboperations)[0xD6] = [](Processor* p) {
        p->SET(2);
    };
    (*cboperations)[0xD7] = [](Processor* p) {
        p->SET(2, p->A);
    };
    (*cboperations)[0xD8] = [](Processor* p) {
        p->SET(3, p->B);
    };
    (*cboperations)[0xD9] = [](Processor* p) {
        p->SET(3, p->C);
    };
    (*cboperations)[0xDA] = [](Processor* p) {
        p->SET(3, p->D);
    };
    (*cboperations)[0xDB] = [](Processor* p) {
        p->SET(3, p->E);
    };
    (*cboperations)[0xDC] = [](Processor* p) {
        p->SET(3, p->H);
    };
    (*cboperations)[0xDD] = [](Processor* p) {
        p->SET(3, p->L);
    };
    (*cboperations)[0xDE] = [](Processor* p) {
        p->SET(3);
    };
    (*cboperations)[0xDF] = [](Processor* p) {
        p->SET(3, p->A);
    };
    (*cboperations)[0xE0] = [](Processor* p) {
        p->SET(4, p->B);
    };
    (*cboperations)[0xE1] = [](Processor* p) {
        p->SET(4, p->C);
    };
    (*cboperations)[0xE2] = [](Processor* p) {
        p->SET(4, p->D);
    };
    (*cboperations)[0xE3] = [](Processor* p) {
        p->SET(4, p->E);
    };
    (*cboperations)[0xE4] = [](Processor* p) {
        p->SET(4, p->H);
    };
    (*cboperations)[0xE5] = [](Processor* p) {
        p->SET(4, p->L);
    };
    (*cboperations)[0xE6] = [](Processor* p) {
        p->SET(4);
    };
    (*cboperations)[0xE7] = [](Processor* p) {
        p->SET(4, p->A);
    };
    (*cboperations)[0xE8] = [](Processor* p) {
        p->SET(5, p->B);
    };
    (*cboperations)[0xE9] = [](Processor* p) {
        p->SET(5, p->C);
    };
    (*cboperations)[0xEA] = [](Processor* p) {
        p->SET(5, p->D);
    };
    (*cboperations)[0xEB] = [](Processor* p) {
        p->SET(5, p->E);
    };
    (*cboperations)[0xEC] = [](Processor* p) {
        p->SET(5, p->H);
    };
    (*cboperations)[0xED] = [](Processor* p) {
        p->SET(5, p->L);
    };
    (*cboperations)[0xEE] = [](Processor* p) {
        p->SET(5);
    };
    (*cboperations)[0xEF] = [](Processor* p) {
        p->SET(5, p->A);
    };
    (*cboperations)[0xF0] = [](Processor* p) {
        p->SET(6, p->B);
    };
    (*cboperations)[0xF1] = [](Processor* p) {
        p->SET(6, p->C);
    };
    (*cboperations)[0xF2] = [](Processor* p) {
        p->SET(6, p->D);
    };
    (*cboperations)[0xF3] = [](Processor* p) {
        p->SET(6, p->E);
    };
    (*cboperations)[0xF4] = [](Processor* p) {
        p->SET(6, p->H);
    };
    (*cboperations)[0xF5] = [](Processor* p) {
        p->SET(6, p->L);
    };
    (*cboperations)[0xF6] = [](Processor* p) {
        p->SET(6);
    };
    (*cboperations)[0xF7] = [](Processor* p) {
        p->SET(6, p->A);
    };
    (*cboperations)[0xF8] = [](Processor* p) {
        p->SET(7, p->B);
    };
    (*cboperations)[0xF9] = [](Processor* p) {
        p->SET(7, p->C);
    };
    (*cboperations)[0xFA] = [](Processor* p) {
        p->SET(7, p->D);
    };
    (*cboperations)[0xFB] = [](Processor* p) {
        p->SET(7, p->E);
    };
    (*cboperations)[0xFC] = [](Processor* p) {
        p->SET(7, p->H);
    };
    (*cboperations)[0xFD] = [](Processor* p) {
        p->SET(7, p->L);
    };
    (*cboperations)[0xFE] = [](Processor* p) {
        p->SET(7);
    };
    (*cboperations)[0xFF] = [](Processor* p) {
        p->SET(7, p->A);
    };
}
