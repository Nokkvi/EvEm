#include "../include/Processor.h"
#include "../include/Register.h"

Processor::Processor(Memory* memory)
{

    this->memory = memory;
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

// Loading into register r the contents of memory address (0xFFn)%.
void Processor::Load(Register* r, uint8_t n) {
    r->SetByte(0, this->memory->GetByte(0xFF00 | n));
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

/*Loads into register r the contents of the internal RAM or register specified
by the 16-bit immediate operand nn.%*/
void Processor::Load(Register* r, uint16_t nn) {
    r->SetByte(0, this->memory->GetByte(nn));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/* Loads into register r the contents of internal RAM, port register, or mode
register at the address in the range 0xFF00-0xFFFF specified by register X.
Loads r <-- ($FF00+X).%*/
void Processor::Load(Register* r, Register* X) {
    r->SetByte(0, this->memory->GetByte(0xFF00 | X->GetByte(0)));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Loads into register r the contents of memory specified by the contents of
register pair xy %*/
void Processor::Load(Register* r, Register* X, Register* Y){
    uint16_t temp = X->GetByte(0) >> 8;
    temp = Y->GetByte(0);
    r->SetByte(this->memory->GetByte(temp));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Loads into register r the contents of memory specified by the contents of
register pair xy, simultaneously incrememnt the contents of HL.%*/
void Processor::Load_Increment(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) >> 8;
    temp = Y->GetByte(0);
    r->SetByte(this->memory->GetByte(temp));
    temp++;
    X->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    Y->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Loads into register r the contents of memory specified by the contents of
register pair XY, then decrementing the pair XY.%*/
void Processor::Load_Decrement(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) >> 8;
    temp = Y->GetByte(0);
    r->SetByte(this->memory->GetByte(temp));
    temp--;
    X->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    Y->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*From Register*/

// Loads into register r the contents of register X.%
void Processor::Load_Register(Register* r, Register* X) {
    r->SetByte(0, X->GetByte(0));
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

/*Immediate Value*/

// Loads into register r the immediate value of n.%
void Processor::Load_Immediate(Register* r, uint8_t n) {
    r->SetByte(0, n);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*To Memory*/

/*From Register*/

/*Loading from register r into memory address (0xFF00-0xFFFF) determined by
the value of n.%*/
void Processor::Store(Register* r, uint8_t n) {
    this->memory->SetByte(0xFF00 | n, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Stores the contents of register A at the internal RAM or register specified
by the 16-bit immediate operand nn.%*/
void Processor::Store(Register* r, uint16_t nn) {
    this->memory->SetByte(nn, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads r -- > ($FF00+X)%
void Processor::Store(Register* r, Register* X){
    this->memory->SetByte(0xFF00 | X->GetByte(0), X->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair xy.%
void Processor::Store(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) << 8;
    temp = Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);

}

/*Store the contents of register r in the memory specified by register pair xy,
simultaneously increment the contents of xy.%*/
void Processor::Store_Increment(Register* r, Register* X, Register* Y){
    uint16_t temp = X->GetByte(0) << 8;
    temp = Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    temp++;
    X->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    Y->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Store the contents of register r in the memory specified by register pair xy,
simultaneously decrement the contents of xy.%*/
void Processor::Store_Decrement(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) << 8;
    temp = Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    temp--;
    X->SetByte(0, (uint8_t)((temp & 0xFF00) >> 8));
    Y->SetByte(0, (uint8_t)(temp & 0x00FF));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

/*Immediate Value*/

// Loads 8-bit immediate data n into memory specified by register pair xy.%
void Processor::Store(Register* X, Register* Y, uint8_t n) {
    uint16_t temp = X->GetByte(0) << 8;
    Y->GetByte(0);
    this->memory->SetByte(temp, n);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

/*
/   16-bit loads
*/

/*To Register*/

/*Immediate Data*/

// Load two bytes of immediate data to register pair xy%.
void Processor::Load(Register* X, Register* Y, uint16_t nn){
    X->SetByte(0, nn >> 8);
    Y->SetByte(0, nn & 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

/*From Stack Pointer*/

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

/*To Memory*/

/*From Stack Pointer*/

/*Stores the lower byte of SP at address nn specified by the 16-bit immediate
operand nn and the upper byte of SP at address nn + 1.*/
void Processor::Store_SP(Register* SP, uint16_t nn){
    this->memory->SetByte(nn, SP->GetByte(0));
    this->memory->SetByte(nn+1,SP->GetByte(1));
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}

/*To Stack Pointer*/

/*From Registers*/

// Load the contents of register pair HL(not the memory location) in stack pointer SP.%
void Processor::Load(Register* X, Register* Y, Register* SP) {
    X->SetByte(0, (SP->GetByte(1)));
    Y->SetByte(0, (SP->GetByte(0)));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}


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
    temp++
    Y->SetByte(0, this->memory->GetByte(SP->GetByte(0)));
    temp++
    SP->SetByte(1, (uint8_t)temp >> 8);
    SP->SetByte(0, (uint8_t)temp && 0x00FF);
    this->M->SetByte(0, 0x03);
    this->T->SetByte(0, 0x12);
}


void Processor::ADD(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
    this.FlagHelper(sum, 0);
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
    this.FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::ADD(Register* SP, int8_t n){
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
    this.FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::ADC(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this.FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::ADC(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) + (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this.FlagHelper(sum, 0);
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
    this.FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SUB(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) - (uint16_t)Y->GetByte(0);
    this.FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::SUB(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) - n;
    this.FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(1, 0x1);

    X->SetByte(0, (uint8_t)(sum & 0x00FF));

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SUB(Register* A, Register* H, Register* L){
    uint16_t temp = H->GetByte(0) << 8;
    temp = L->GetByte(0);
    uint16_t sum = X->GetByte(0);
    sum -= this->memory->GetByte(temp);
    this.FlagHelper(sum, 0);
    if(sum > 65535)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

void Processor::SBC(Register* X, Register* Y){
    uint16_t sum = (uint16_t)X->GetByte(0) - (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this.FlagHelper(sum, 1);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);
    this->M->SetByte(0, 0x01);
    this->T->SetByte(0, 0x04);
}

void Processor::SBC(Register* X, uint8_t n){
    uint16_t sum = (uint16_t)X->GetByte(0) - (uint16_t)Y->GetByte(0);
    sum+=(uint16_t)(this->F->GetByte(0)&0x10)!=0 ? 1 : 0;
    this.FlagHelper(sum, 1);
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
    this.FlagHelper(sum, 0);
    if(sum > 255)
        this->F->SetHex(0, 0x1);
    X->SetByte(0, (uint8_t)sum & 0x00FF);

    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
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
    this.ADD(X, 1);
}

void Processor::INC(Register* X, Register* Y){

}

void Processor::INC(Register* H, Register* L){
}

void Processor::DEC(Register* X){
    this.SUB(X, 1);
}

void Processor::DEC(Register* X, Register* Y){
    Y->SetByte(0, Y->GetByte(0)+1);
    if(Y->GetByte(0)==0)
        X->SetByte(0, X->GetByte(0)+1);
    if((X->GetByte(0)==0)&(Y->GetByte(0)==0=)
        this->F->SetByte(0, 0x10);
}

void Processor::DECHL(Register* H, Register* L){

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

void Processor::FlagHelper(uint16_t n, int as){
    this->F->SetByte(0, 0x0);
    if(!(n&255))
        this->F->SetByte(0, this->F->GetByte(0)|=128);
    this->F->SetByte(0, this->F->GetByte(0)|= as ? 0x40 : 0);
}
