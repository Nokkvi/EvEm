#include "../include/Processor.h"
#include "../include/Register.h"

Processor::Processor(Memory* pMemory)
{
    //m_pMemory = pMemory;
    //pMemory->SetProcessor(this);
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
}

void Processor::LoadFrom(Register* X){
}

void Processor::LoadFrom(){

}

void Processor::LoadTo(Register* X){

}

void Processor::LoadTo(){

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

}

void Processor::ADD(Register* X, Register* Y, Register* ZW){
    uint16_t xy = ((uint16_t)X->GetByte(0)) << 8;
    xy |= ((uint16_t)Y->GetByte(0));
    uint16_t zw = ((uint16_t)ZW->GetHex(0));
    uint32_t sum = (uint32_t)xy +(uint32_t)zw;
    if(sum>65536)
        this->F->SetHex(1, 0x1);
}

void Processor::ADD(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) + n;
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum / 256));
}

void Processor::ADD(int8_t n){

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
}

void Processor::HALT(){

}

void Processor::STOP(){

}

void Processor::DI(){

}

void Processor::EI(){

}
