#include "../include/Processor.h"

Processor::Processor(GPU* gpu, Memory* memory)
{
    this->gpu = gpu;
    this->memory = memory;
    //pMemory->SetProcessor(this);
    InitOpcodes();

    this->_halt = 0;
    this->_ime = 0;
    this->_stop = 0;

    this->A = new Register(1);
    this->B = new Register(1);
    this->C = new Register(1);
    this->D = new Register(1);
    this->E = new Register(1);
    this->H = new Register(1);
    this->L = new Register(1);
    this->F = new Register(1);

    this->PC = new Register(2);
    this->SP = new Register(2);

    this->M = new Register(1);
    this->T = new Register(1);
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
    this->L->SetByte(0, 0x00);
    this->F->SetByte(0, 0x00);
    this->PC->SetByte(0, 0x00);
    this->SP->SetByte(0, 0x00);
    this->M->SetByte(0, 0x00);
    this->T->SetByte(0, 0x00);

    this->_halt = 0;
    this->_ime = 1;
    this->_stop = 0;
}

void Processor::Render()
{
    gpu->Step(this->T->GetByte(0));
}

// Loading into register r the immediate n.
void Processor::Load(Register* r, uint8_t n) {
    r->SetByte(0, n);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

// Loads into register r the 16 bit immediate nn
void Processor::Load(Register* r, uint16_t nn) {
    r->SetWord(0, nn);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into 8 bit register r the contents of 8 bit register X.
void Processor::Load(Register* r, Register* X) {
    r->SetByte(0, X->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into register pair XY the immediate nn.
void Processor::Load(Register* X, Register* Y, uint16_t nn) {
    Y->SetByte(0, (uint8_t)(nn >> 8));
    X->SetByte(0, (uint8_t)nn);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into 16 bit register r the values in register pair XY.
void Processor::Load(Register* r, Register* X, Register* Y){
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    r->SetWord(0, temp);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into 16 bit register r the values in register pair XY, simultaneously increment the contents of register pair XY.
void Processor::Load_Increment(Register* r, Register* X, Register* Y) {
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    r->SetWord(0, temp);
    temp++;
    Y->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    X->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into 16 bit register r the values in register pair XY, simultaneously decrement the contents of register pair XY.
void Processor::Load_Decrement(Register* r, Register* X, Register* Y) {
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    r->SetWord(0, temp);
    temp--;
    Y->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    X->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of 8 bit register r at address nn.
void Processor::Store(Register* r, uint16_t nn) {
    this->memory->SetByte(nn, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of 8 bit register r at address specified by 16 bit register X.
void Processor::Store(Register* r, Register* X){
    this->memory->SetByte(X->GetWord(0), r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair XY.
void Processor::Store(Register* r, Register* X, Register* Y) {
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);

}

// Stores the contents of register r in memory specified by register pair XY, simultaneously increment the contents of XY.
void Processor::Store_Increment(Register* r, Register* X, Register* Y){
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    if (X->Increment())
        Y->Increment();
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair XY, simultaneously decrement the contents of XY.
void Processor::Store_Decrement(Register* r, Register* X, Register* Y) {
    uint16_t temp = Y->GetByte(0) << 8;
    temp |= X->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    if (X->Decrement())
        Y->Decrement();
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Immediate Value*/

// Loads 8-bit immediate data n into memory specified by register pair xy.%
void Processor::Store(Register* X, Register* Y, uint8_t n) {
    uint16_t temp = Y->GetByte(0) << 8;
    X->GetByte(0);
    this->memory->SetByte(temp, n);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}
/*
// Load two bytes of immediate data to register pair xy%.
void Processor::Load(Register* X, Register* Y, uint16_t nn){
    X->SetByte(0, nn >> 8);
    Y->SetByte(0, nn & 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

// The 8-bit operand e is added to SP and the result is stored in HL.%
void Processor::Load(Register* SP, Register* X, Register* Y, uint8_t e) {
    uint16_t temp = SP->GetByte(1) << 8;
    temp = SP->GetByte(0);
    temp += e;
    X->SetByte(0, temp >> 8);
    Y->SetByte(0, temp & 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

// Stores the lower byte of SP at address nn specified by the 16-bit immediate
// operand nn and the upper byte of SP at address nn + 1.
void Processor::Store_SP(Register* SP, uint16_t nn){
    this->memory->SetByte(nn, SP->GetByte(0));
    this->memory->SetByte(nn+1,SP->GetByte(1));
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

// Load the contents of register pair HL(not the memory location) in stack pointer SP.
void Processor::Load_HL(Register* X, Register* Y, Register* SP) {
    X->SetByte(0, (SP->GetByte(1)));
    Y->SetByte(0, (SP->GetByte(0)));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}
*/

/*Not sure how to classify push and pop....*/

/*Pushes the contents of register pair qq onto the memory stack. First 1 is
subtracted from SP and the contents of the higherportion of qq are placed on
the stack. The contents of the lower portion of qq are then placed on the
stack. The contents of SP are automatically decremented by 2.*/
void Processor::Push(Register* SP, Register* X, Register* Y){
    uint16_t temp = SP->GetByte(1) << 8;
    temp = SP->GetByte(0);
    temp--;
    this->memory->SetByte(temp, X->GetByte(0));
    temp--;
    this->memory->SetByte(temp, Y->GetByte(0));
    SP->SetByte(1, (uint8_t)temp >> 8);
    SP->SetByte(0, (uint8_t)temp && 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

/*Pops the contents from the memory stack and into register pair qq. First
the contents of memory, specified by the contents of SP are loaded in the
lower portion of qq. Next, the contents of SP are incremented by 1 and the
contents of the memory they specify are loaded in the upper portion of qq.
The contents of SP are automatically incremented by 2.*/
void Processor::Pop(Register* SP, Register* X, Register* Y) {
    uint16_t temp = SP->GetByte(1) << 8;
    temp = SP->GetByte(0);
    X->SetByte(0, this->memory->GetByte(SP->GetByte(0)));
    temp++;
    Y->SetByte(0, this->memory->GetByte(SP->GetByte(0)));
    temp++;
    SP->SetByte(1, (uint8_t)temp >> 8);
    SP->SetByte(0, (uint8_t)temp && 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}


void Processor::ADD(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::ADD(Register* X, Register* Y, Register* Z, Register* W) {
    uint16_t xy = ((uint16_t)X->GetByte(0)) << 8;
    xy |= ((uint16_t)Y->GetByte(0));
    uint16_t zw = ((uint16_t)Z->GetByte(0)) << 8;
    zw |= ((uint16_t)W->GetByte(0));
    uint32_t sum = (uint32_t)xy + (uint32_t)zw;
    if(sum>65535)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (sum & 0xFF00) >> 8);
    Y->SetByte(0, sum & 0x00FF);

    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
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

    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x012);
}

void Processor::ADD(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) + n;
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::ADDSIGNED(Register* SP, int8_t n){
    if(n>127)
        n=-((~n+1)&255);
    uint16_t temp = SP->GetByte(1) << 8;
    temp = SP->GetByte(0);
    temp += n;
    if(temp > 255)
        this->F->SetHex(1, 0x1);
    SP->SetByte(1, (uint8_t)temp >> 8);
    SP->SetByte(0, (uint8_t)temp && 0x00FF);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

//adds the content of register X with the contents of memory location (HL)
void Processor::ADDHL(Register* X, Register* H, Register* L){
    uint16_t temp = H->GetByte(0) << 8;
    temp = L->GetByte(0);
    uint16_t sum = X->GetByte(0);
    sum += this->memory->GetByte(temp);
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::ADC(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::ADC(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) + n;
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::ADC(Register* X, Register* H, Register* L){
    uint16_t temp = H->GetByte(0) << 8;
    temp = L->GetByte(0);
    uint16_t sum = X->GetByte(0);
    sum += this->memory->GetByte(temp);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SUB(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) - (uint16_t)Y->GetByte(0);
    this->FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::SUB(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) - n;
    this->FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SUB(Register* A, Register* H, Register* L){
    uint16_t temp = H->GetByte(0) << 8;
    temp = L->GetByte(0);
    uint16_t sum = A->GetByte(0);
    sum -= this->memory->GetByte(temp);
    this->FlagHelper(sum, 0);
    if(sum > 65535)
        this->F->SetHex(0, 0x1);
    A->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SBC(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) - (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::SBC(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) - n;
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SBC(Register* X, Register* H, Register* L){
    uint16_t temp = H->GetByte(0) << 8;
    temp = L->GetByte(0);
    uint16_t sum = X->GetByte(0);
    sum -= this->memory->GetByte(temp);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this->FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::AND(Register* X){
    uint16_t result = this->A->GetByte(0)&X->GetByte(0);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::AND(uint8_t n){
    uint16_t result = this->A->GetByte(0)&n;
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::AND(Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    uint16_t result = (uint16_t)this->A->GetByte(0)&this->memory->GetByte(n);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::OR(Register* X){
    uint16_t result = this->A->GetByte(0)|X->GetByte(0);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::OR(uint8_t n){
    uint16_t result = this->A->GetByte(0)|n;
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::OR(Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    uint16_t result = (uint16_t)this->A->GetByte(0)|this->memory->GetByte(n);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::XOR(Register* X){
    uint16_t result = this->A->GetByte(0)^X->GetByte(0);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::XOR(uint8_t n){
    uint16_t result = this->A->GetByte(0)^n;
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::XOR(Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    uint16_t result = (uint16_t)this->A->GetByte(0)^this->memory->GetByte(n);
    this->FlagHelper(result, 0);
    this->A->SetByte(0, (uint8_t)result & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::CP(Register* X, Register* Y){
    uint16_t n = X->GetByte(0);
    n -= Y->GetByte(0);
    this->FlagHelper(n, 1);
    if(n>255)
        this->F->SetByte(0, 0x10);
    n&=(0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}
void Processor::CP(Register* X, uint8_t n){
    uint16_t x = X->GetByte(0);
    x -= n;
    this->FlagHelper(n, 1);
    if(n>255)
        this->F->SetByte(0, 0x10);
    n&=(0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::CP(Register* X, Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    uint16_t x = X->GetByte(0);
    x -= this->memory->GetByte(n);
    this->FlagHelper((uint16_t)this->memory->GetByte(x), 1);
    if(n>255)
        this->F->SetByte(0, 0x10);
    n&=(0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::INC(Register* X){
    this->ADD(X, 1);
}

void Processor::INC(Register* X, Register* Y){
    Y->SetByte(0, Y->GetByte(0)+1);
    if(Y->GetByte(0)==0)
        X->SetByte(0, X->GetByte(0)+1);
    if((X->GetByte(0)==0)&(Y->GetByte(0)==0))
        this->F->SetByte(0, 0x10);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::INCHL(Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    this->memory->SetByte(n, this->memory->GetByte(n)+1);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

void Processor::DEC(Register* X){
    this->SUB(X, 1);
}

void Processor::DEC(Register* X, Register* Y){
    Y->SetByte(0, Y->GetByte(0)-1);
    if(Y->GetByte(0)==255)
        X->SetByte(0, X->GetByte(0)-1);
    if((X->GetByte(0)==255)&(Y->GetByte(0)==255))
        this->F->SetByte(0, 0x10);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::DECHL(Register* H, Register* L){
    uint16_t n = H->GetByte(0) << 8;
    n += L->GetByte(0);
    this->memory->SetByte(n, this->memory->GetByte(n)-1);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

void Processor::RLCA(){
    uint8_t ci = this->A->GetByte(0) & 0x80 ? 1 : 0;
    uint8_t co = this->A->GetByte(0) & 0x80 ? 0x10 : 0;
    this->A->SetByte(0, (this->A->GetByte(0) << 1)+ci);
    this->A->SetByte(0, this->A->GetByte(0)&255);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::RLC(Register* X){
    uint8_t ci = X->GetByte(0)&0x80 ? 1 : 0;
    uint8_t co = X->GetByte(0)&0x80 ? 0x10 : 0;
    X->SetByte(0, (X->GetByte(0) << 1)+ci);
    X->SetByte(0, X->GetByte(0)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::RLCHL(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint8_t ci = this->F->GetByte(0)&0x80 ? 1 : 0;
    uint8_t co = HLmem&0x80 ? 0x10 : 0;
    uint16_t HLtemp = (HLmem << 1)+ci;
    HLtemp&=255;
    this->FlagHelper(HLtemp, 0);
    this->memory->SetByte(HL, (uint8_t)HLtemp);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

void Processor::RLA(){
    uint8_t ci = this->F->GetByte(0)&0x10 ? 1 : 0;
    uint8_t co = this->A->GetByte(0)&0x80 ? 0x10 : 0;
    this->A->SetByte(0, (this->A->GetByte(0) << 1)+ci);
    this->A->SetByte(0, this->A->GetByte(0)&255);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::RL(Register* X){
    uint8_t ci = this->F->GetByte(0)&0x10 ? 1 : 0;
    uint8_t co = X->GetByte(0)&0x80 ? 1 : 0;
    X->SetByte(0, (X->GetByte(0) << 1)+ci);
    X->SetByte(0, X->GetByte(0)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::RLHL(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint8_t ci = this->F->GetByte(0)&0x10 ? 1 : 0;
    uint8_t co = HLmem&0x80 ? 0x10 : 0;
    uint16_t HLtemp = (HLmem << 1)+ci;
    HLtemp&=255;
    this->FlagHelper(HLtemp, 0);
    this->memory->SetByte(HL, (uint8_t)HLtemp);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

void Processor::RRCA(){
    uint8_t ci = this->A->GetByte(0) & 1 ? 0x80 : 0;
    uint8_t co = this->A->GetByte(0) & 1 ? 0x10 : 0;
    this->A->SetByte(0, (this->A->GetByte(0) >> 1)+ci);
    this->A->SetByte(0, this->A->GetByte(0)&255);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::RRC(Register* X){
    uint8_t ci = X->GetByte(0)&1 ? 0x80 : 0;
    uint8_t co = X->GetByte(0)&1 ? 0x10 : 0;
    X->SetByte(0, (X->GetByte(0) >> 1)+ci);
    X->SetByte(0, X->GetByte(0)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::RRCHL(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint8_t ci = this->F->GetByte(0)&1 ? 0x80 : 0;
    uint8_t co = HLmem&1 ? 0x10 : 0;
    uint16_t HLtemp = (HLmem >> 1)+ci;
    HLtemp&=255;
    this->FlagHelper(HLtemp, 0);
    this->memory->SetByte(HL, (uint8_t)HLtemp);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

void Processor::RRA(){
    uint8_t ci = this->F->GetByte(0)&0x10 ? 0x80 : 0;
    uint8_t co = this->A->GetByte(0)&1 ? 0x10 : 0;
    this->A->SetByte(0, (this->A->GetByte(0) >> 1)+ci);
    this->A->SetByte(0, this->A->GetByte(0)&255);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::RR(Register* X){
    uint8_t ci = this->F->GetByte(0)&0x10 ? 0x80 : 0;
    uint8_t co = X->GetByte(0)&1 ? 0x10 : 0;
    X->SetByte(0, (X->GetByte(0) >> 1)+ci);
    X->SetByte(0, X->GetByte(0)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::RRHL(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint8_t ci = this->F->GetByte(0)&1 ? 0x10 : 0;
    uint8_t co = HLmem&0x10 ? 0x80 : 0;
    uint16_t HLtemp = (HLmem >> 1)+ci;
    HLtemp&=255;
    this->FlagHelper(HLtemp, 0);
    this->memory->SetByte(HL, (uint8_t)HLtemp);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

void Processor::SLA(Register* X){
    uint16_t co = X->GetByte(0)&0x80 ? 0x10 : 0;
    X->SetByte(0, (X->GetByte(0)<<1)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SLA(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint16_t co = HLmem&0x80 ? 0x10 : 0;
    uint16_t HLtemp = (HLmem<<1)&255;
    this->FlagHelper(HLtemp, 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

void Processor::SRA(Register* X){
    uint16_t ci = X->GetByte(0)&0x80;
    uint16_t co = X->GetByte(0)&1 ? 0x10 : 0;
    X->SetByte(0, ((X->GetByte(0)>>1)+ci)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SRA(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint16_t ci = HLmem&0x80;
    uint16_t co = HLmem&1 ? 0x10 : 0;
    uint16_t HLtemp = ((HL>>1)+ci)&255;
    this->FlagHelper(HLtemp, 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

void Processor::SRL(Register* X){
    uint16_t co = X->GetByte(0)&1 ? 0x10 : 0;
    X->SetByte(0, (X->GetByte(0) >> 1)&255);
    this->FlagHelper((uint16_t)X->GetByte(0), 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SRL(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    uint16_t co = HLmem & 1 ? 0x10 : 0;
    uint16_t HLtemp = (HLmem>>1)&255;
    this->FlagHelper(HLtemp, 0);
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+co);
    this->M->SetByte(0, 0x04);
    this->T->SetByte(0, 0x16);
}

void Processor::BIT(uint8_t b, Register* X){
    this->FlagHelper(X->GetByte(0)&b, 0);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}
void Processor::BIT(uint8_t b){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    this->FlagHelper(this->memory->GetByte(HL)&b, 0);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}
void Processor::SET(uint8_t b, Register* X){
    X->SetByte(0, X->GetByte(0)|b);
    this->M->SetByte(0, 0x02);
    this->M->SetByte(0, 0x08);
}
void Processor::SET(uint8_t b){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLbyte = this->memory->GetByte(HL);
    this->memory->SetByte(HL, HLbyte|b);
    this->M->SetByte(0, 0x03);
    this->M->SetByte(0, 0x12);
}
void Processor::RES(uint8_t b, Register* X){
    uint8_t value = X->GetByte(0);
    value &= 0xFF - (1 << b);
    X->SetByte(0, value);
    this->M->SetByte(0, 0x02);
    this->M->SetByte(0, 0x08);
}
void Processor::RES(uint8_t b){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t value = this->memory->GetByte(HL);
    value &= 0xFF - (1 << b);
    this->memory->SetByte(HL, value);
    this->M->SetByte(0, 0x04);
    this->M->SetByte(0, 0x16);
}

void Processor::DAA(){
    uint16_t a = this->A->GetByte(0);
    if ((this->F->GetByte(0) & 0x20) || ((this->A->GetByte(0) & 15) > 9))
        this->A->SetByte(0, this->A->GetByte(0)+6);
    this->F->SetByte(0, this->F->GetByte(0) & 0xEF);
    if((this->F->GetByte(0)&0x20)||(a>0x99)){
        this->A->SetByte(0, this->A->GetByte(0) + 0x60);
        this->F->SetByte(0, this->F->GetByte(0)|0x10);
    }
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::CPL(){
    this->A->SetByte(0, ~(this->A->GetByte(0))&255);
    this->FlagHelper((uint16_t)this->A->GetByte(0), 1);
    this->M->SetByte(0, 0x01);
    this->M->SetByte(0, 0x04);
}

void Processor::SWAP(Register* X){
    uint16_t swapper = X->GetByte(0);
    X->SetByte(0, (((swapper&0xF)<<4)|((swapper&0xF0)>>4)));
    this->F->SetByte(0, X->GetByte(0)?0:0x80);
    this->M->SetByte(0, 0x01);
    this->M->SetByte(0, 0x04);
}

void Processor::SWAP(){
    uint16_t HL = this->H->GetByte(0) << 8;
    HL += this->L->GetByte(0);
    uint8_t HLmem = this->memory->GetByte(HL);
    this->memory->SetByte(HL, (((HLmem&0xF)<<4)|((HLmem&0xF0)>>4)));
    this->F->SetByte(0, this->memory->GetByte(HL)?0:0x80);
    this->M->SetByte(0, 0x02);
    this->M->SetByte(0, 0x08);
}

void Processor::CCF(){
    uint8_t ci = this->F->GetByte(0)&0x10 ? 0 : 0x10;
    this->F->SetByte(0, (this->F->GetByte(0)&0xEF)+ci);
    this->M->SetByte(0, 0x01);
    this->M->SetByte(0, 0x04);
}

void Processor::SCF(){
    this->F->SetByte(0, this->F->GetByte(0)|0x10);
    this->M->SetByte(0, 0x01);
    this->M->SetByte(0, 0x04);
}

void Processor::JR(int8_t value)
{

}

void Processor::NOP(){
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::HALT(){
    this->_halt = 1;
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::STOP(){
    this->_stop = 1;
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::DI(){
    this->_ime = 0;
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::EI(){
    this->_ime = 1;
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::FlagHelper(uint16_t n, int as){
    this->F->SetByte(0, 0x0);
    if(!(n&255))
        this->F->SetByte(0, this->F->GetByte(0) | 128);
    this->F->SetByte(0, this->F->GetByte(0) | as ? 0x40 : 0);
}
