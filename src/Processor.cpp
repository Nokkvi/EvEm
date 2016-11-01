#include "../include/Processor.h"
#include "../include/Register.h"
#include "../include/Memory.h"

Processor::Processor(Memory* pMemory)
{
    m_pMemory = pMemory;
    pMemory->SetProcessor(this);
    InitOpcodes();
}

Processor::~Processor()
{
    delete operations;
    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete H;
    delete L;
    delete F;
    delete PC;
    delete SP;
    delete M;
    delete T;
}

void Processor::Reset(){
    this->A->SetByte(0, 0x00);
    this->B->SetByte(0, 0x00);
    this->C->SetByte(0, 0x00);
    this->D->SetByte(0, 0x00);
    this->E->SetByte(0, 0x00);
    this->H->SetByte(0, 0x00);
    this->K->SetByte(0, 0x00);
    this->F->SetByte(0, 0x00);
    this->PC->SetByte(0, 0x00);
    this->SP->SetByte(0, 0x00);
    this->M->SetByte(0, 0x00);
    this->T->SetByte(0, 0x00);
}

void Processor::LoadFrom(Register* X){
    X->SetByte(this->A->GetByte(0));
    this->T->SetByte(0, 0x04);
    this->M->SetByte(0, 0x01);
}

void Processor::LoadFrom(){
    uint16_t P;
    P = (this->H->GetByte(0)) << 8;
    P = this->L->GetByte(0);
    Memory.writeByte(this->A, P);
    this->T->SetByte(0, 0x08);
    this->M->SetByte(0, 0x02);
}

void Processor::LoadTo(Register* X){
    this->A->SetByte(X->GetByte(0));
    this->T->SetByte(0, 0x04);
    this->M->SetByte(0, 0x01);
}

void Processor::LoadTo(){
    uint16_t P;
    P = (this->H->GetByte(0)) << 8;
    P = this->L->GetByte(0);
    Memory.writeByte(P, this->A.GetByte(0));
    this->T->SetByte(0, 0x08);
    this->M->SetByte(0, 0x02);
}

void Processor::DirectLoad(uint16_t n){

}

void Processor::DirectLoad(Register* X, Register* Y, uint16_t n){

}

void Processor::DirectLoadSP(uint16_t n){

}

void Processor::ImmediateLoad(Register* X, uint8_t n){

}

void Processor::ImmediateLoad(Register* X, Register* Y, uint16_t n){

}

void Processor::PopXY(Register* X, Register* Y){

}

void Processor::Store(uint16_t loc, Register* X){

}

void Processor::Store(uint16_t loc){

}

void Processor::Store(Register* X){

}

void Processor::Store(){

}

void Processor::StoreValue(uint8_t n){

}

void Processor::ADD(Register* X, Register* Y){
    if (X->GetSize() == 1 && Y->GetSize() == 1)
    {
        uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
        if(sum > 255)
            this->F->SetHex(1, 0x1);

        X->SetByte(0, (uint8_t)(sum & 0x00FF));
    }
    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::ADD(Register* X, Register* Y, Register* Z, Register* W) {
    uint16_t xy = ((uint16_t)X->GetByte(0)) << 8;
    xy |= ((uint16_t)Y->GetByte(0));
    uint16_t zw = ((uint16_t)Z->GetByte(0)) << 8;
    zw |= ((uint16_t)W->GetByte(0));
    uint32_t sum = (uint32_t)xy + (uint32_t)zw;
    if(sum>65536)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (sum & 0xFF00) >> 8);
    Y->SetByte(0, sum & 0x00FF);

    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::ADD(Register* X, Register* Y, Register* ZW){
    uint16_t xy = ((uint16_t)X->GetByte(0)) << 8;
    xy |= ((uint16_t)Y->GetByte(0));
    uint16_t zw = ((uint16_t)ZW->GetHex(0));
    uint32_t sum = (uint32_t)xy +(uint32_t)zw;
    if(sum>65536)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (sum & 0xFF00) >> 8);
    Y->SetByte(0, sum & 0x00FF);

    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::ADD(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) + n;
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t) (sum & 0x00FF));

    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::ADD(Register* SP, int8_t n){
    uint8_t temp = memory->ReadByte();

    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::ADC(Register* X){

}

void Processor::ADC(uint8_t n){

}

void Processor::ADC(){

}

void Processor::SUB(Register* X){

}

void Processor::SUB(uint8_t n){

}

void Processor::SUB(){

}

void Processor::SBC(Register* X){

}

void Processor::SBC(uint8_t n){

}

void Processor::SBC(){

}

void Processor::AND(Register* X){

}

void Processor::AND(uint8_t n){

}

void Processor::AND(){

}

void Processor::OR(Register* X){

}

void Processor::OR(uint8_t n){

}

void Processor::OR(){

}

void Processor::XOR(Register* X){

}

void Processor::XOR(uint8_t n){

}

void Processor::XOR(){

}

void Processor::CP(Register* X){

}
void Processor::CP(uint8_t n){

}

void Processor::CP(){

}

void Processor::INC(Register* X){

}

void Processor::INC(){

}

void Processor::DEC(Register* X){

}

void Processor::DEC(){

}

void Processor::RLCA(){

}

void Processor::RLC(Register* X){

}

void Processor::RLC(){

}

void Processor::RLA(){

}

void Processor::RL(Register* X){

}

void Processor::RL(){

}

void Processor::RRCA(){

}

void Processor::RRC(Register* X){

}

void Processor::RRC(){

}

void Processor::RRA(){

}

void Processor::RR(Register* X){

}

void Processor::RR(){

}

void Processor::SLA(Register* X){

}

void Processor::SLA(){

}

void Processor::SRA(Register* X){

}

void Processor::SRA(){

}

void Processor::SRL(Register* X){

}

void Processor::SRL(){

}

/*void Processor::BIT(Bit b, Register* X){

}

void Processor::BIT(Bit b){

}

void Processor::SET(Bit b, Register* X){

}

void Processor::SET(Bit b){

}

void Processor::RES(Bit b, Register* X){

}

void Processor::RES(Bit b){

}*/

void Processor::DAA(){

}

void Processor::CPL(){

}

void Processor::SWAP(Register* X){

}

void Processor::SWAP(){

}

void Processor::CCF(){

}

void Processor::SCF(){

}

void Processor::NOP(){
    //No operation
    T->SetByte(0, 0x04);
    M->SetByte(0, 0x01);
}

void Processor::HALT(){

}

void Processor::STOP(){

}

void Processor::DI(){

}

void Processor::EI(){

}
