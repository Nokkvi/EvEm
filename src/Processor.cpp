#include "../include/Processor.h"

Processor::Processor(GPU* gpu, Memory* memory)
{
	this->gpu = gpu;
	this->memory = memory;
	//pMemory->SetProcessor(this);
	//    InitOpcodes();

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

    Reset();

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

	delete Trash8;
	delete Trash16;
}

void Processor::Reset(){
    this->A->SetByte(0, 0);
    this->B->SetByte(0, 0);
    this->C->SetByte(0, 0);
    this->D->SetByte(0, 0);
    this->E->SetByte(0, 0);
    this->H->SetByte(0, 0);
    this->L->SetByte(0, 0);
    this->F->SetByte(0, 0);
    this->PC->SetWord(0, 0);
    this->SP->SetWord(0, 0);
    this->M->SetByte(0, 0);
    this->T->SetByte(0, 0);

    this->memory->SetBIOS(true);

    cbInstruction = false;

    this->_halt = 0;
    this->_ime = 1;
    this->_stop = 0;

	this->Trash8->SetByte(0, 0x00);
	this->Trash16->SetWord(0, 0x0000);
}

void Processor::Render()
{
	gpu->Step(this->T->GetByte(0));
}

//Helper function for getting byte from memory.
uint8_t Processor::FromMemory(Register* X, Register* Y) {
	uint16_t temp = X->GetByte(0) << 8;
	temp |= Y->GetByte(0);
	return (this->memory->GetByte(temp));
}

void Processor::ToMemory(Register* X, Register* Y, uint8_t n) {
	uint16_t temp = X->GetByte(0) << 8;
	temp |= Y->GetByte(0);
	this->memory->SetByte(temp, n);
}

//----------------------------------------------------------------

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

    X->SetByte(0, (uint8_t)(nn >> 8));
    Y->SetByte(0, (uint8_t)nn);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);

}

// Loads into 16 bit register r the values in register pair XY.

void Processor::Load(Register* r, Register* X, Register* Y){
    uint16_t temp = X->GetByte(0) << 8;
    temp |= Y->GetByte(0);
    r->SetWord(0, temp);
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads into 16 bit register r the values in register pair XY, simultaneously increment the contents of register pair XY.
void Processor::Load_Increment(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) << 8;
    temp |= Y->GetByte(0);
    r->SetWord(0, temp);
    if (Y->Increment())
        X->Increment();
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}


// Loads into 16 bit register r the values in register pair XY, simultaneously decrement the contents of register pair XY.
void Processor::Load_Decrement(Register* r, Register* X, Register* Y) {
	uint16_t temp = X->GetByte(0) << 8;
	temp |= Y->GetByte(0);
	r->SetWord(0, temp);
	if (Y->Decrement())
		X->Decrement();
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
void Processor::Store(Register* r, Register* X) {
	this->memory->SetByte(X->GetWord(0), r->GetByte(0));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair XY.
void Processor::Store(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) << 8;
    temp |= Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair XY, simultaneously increment the contents of XY.
void Processor::Store_Increment(Register* r, Register* X, Register* Y){
    uint16_t temp = X->GetByte(0) << 8;
    temp |= Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    if (X->Increment())
        Y->Increment();
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Stores the contents of register r in memory specified by register pair XY, simultaneously decrement the contents of XY.
void Processor::Store_Decrement(Register* r, Register* X, Register* Y) {
    uint16_t temp = X->GetByte(0) << 8;
    temp |= Y->GetByte(0);
    this->memory->SetByte(temp, r->GetByte(0));
    if (X->Decrement())
        Y->Decrement();
    this->M->SetByte(0, 0x02);
    this->T->SetByte(0, 0x08);
}

// Loads 8-bit immediate data n into memory specified by register pair xy.%
void Processor::Store(Register* X, Register* Y, uint8_t n) {
    uint16_t temp = X->GetByte(0) << 8;
    temp|= Y->GetByte(0);
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
//Incorrect, we should be loading from registers to stack pointer not reverse.
X->SetByte(0, (SP->GetByte(1)));
Y->SetByte(0, (SP->GetByte(0)));
this->M->SetByte(0, 0x02);
this->T->SetByte(0, 0x08);
}

/*Pushes the contents of register pair xy onto the memory stack. First 1 is
subtracted from SP and the contents of the higherportion of xy are placed on
the stack. The contents of the lower portion of qq are then placed on the
stack. The contents of SP are automatically decremented by 2.*/
void Processor::Push(Register* SP, Register* X, Register* Y) {
	uint16_t temp = SP->GetWord(0);
	temp--;
	this->memory->SetByte(temp, X->GetByte(0));
	temp--;
	this->memory->SetByte(temp, Y->GetByte(0));
	SP->SetWord(0, temp);
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x12);
}

/*Pops the contents from the memory stack and into register pair qq. First
the contents of memory, specified by the contents of SP are loaded in the
lower portion of qq. Next, the contents of SP are incremented by 1 and the
contents of the memory they specify are loaded in the upper portion of qq.
The contents of SP are automatically incremented by 2.*/
void Processor::Pop(Register* SP, Register* X, Register* Y) {
	uint16_t temp = SP->GetWord(0);
	Y->SetByte(0, this->memory->GetByte(temp));
	temp++;
	X->SetByte(0, this->memory->GetByte(temp));
	temp++;
	SP->SetWord(0, temp);
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x12);
}

uint8_t Processor::WordToByte(bool higher, uint16_t n) {
	if (higher) {
		return uint8_t(n >> 8);
	}
	else {
		return uint8_t(n) & 0xFF;
	}
}

uint16_t Processor::ByteToWord(uint8_t n) {
	return (0x00FF & (uint16_t)n);
}

uint16_t Processor::RegisterPairContents(Register* X, Register* Y) {
	uint16_t temp = (X->GetByte(0) << 8);
	temp |= Y->GetByte(0);
	return temp;
}

void Processor::ToRegisterPair(Register* X, Register* Y, uint16_t n) {
	uint16_t temp = (X->GetByte(0) << 8);
	temp |= Y->GetByte(0);
	X->SetByte(0, this->WordToByte(1, n));
	Y->SetByte(0, this->WordToByte(0, n));
}

void Processor::FlagSetter(bool zeroFlag, bool subtractionFlag, bool halfCarryFlag, bool carryFlag) {
	this->ZeroFlag = zeroFlag;
	this->SubtractionFlag = subtractionFlag;
	this->HalfCarryFlag = halfCarryFlag;
	this->CarryFlag = carryFlag;
}

bool Processor::carry(bool cIn, bool X, bool Y) {
	if (cIn) {
		if (X || Y) return true;
	}
	else {
		if (X && Y) return true;
	}
	return false;
}

bool Processor::borrow(bool cin, bool X, bool Y) {
	if (X) {
		if (cin && Y) return true;
	}
	else {
		if (cin || Y) return true;
	}
	return false;
}

uint8_t Processor::ADDHelper(bool cin, uint8_t firstByte, uint8_t secondByte) {
	uint8_t sum = firstByte + secondByte;
	this->Trash8->SetByte(0, firstByte);
	this->Trash8_2->SetByte(0, secondByte);
	bool a = cin, b = 0, c = 0, d = 0;
	for (int i = 0; i < 8; i++) {
		a = this->carry(a, this->Trash16->GetBit(i), this->Trash8->GetBit(i));
		if (i == 3) {
			c = a;
		}	
		else if (i == 7) {
			d = a;
		}
	}
	a = (sum == 0);
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint16_t Processor::ADDHelper16(bool cin, uint16_t firstWord, uint16_t secondWord) {
	uint16_t sum = firstWord + secondWord;
	this->Trash16->SetWord(0, firstWord);
	this->Trash16_2->SetWord(0, secondWord);
	bool a = cin, b = 0, c = 0, d = 0;
	for (int i = 0; i < 16; i++) {
		a = this->carry(a, this->Trash16->GetBit(i), this->Trash16_2->GetBit(i));
		if (i == 11) {
			c = a;
		}
		else if (i == 15) {
			d = a;
		}
	}
	a = (sum == 0);
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint8_t Processor::SUBHelper(bool cin, uint8_t firstByte, uint8_t secondByte) {
	uint8_t sum = firstByte - secondByte;
	this->Trash8->SetByte(0, firstByte);
	this->Trash8_2->SetByte(0, secondByte);
	bool a = cin, b = 1, c = 0, d = 0;
	for (int i = 0; i < 8; i++) {
		c = this->borrow(c, this->Trash8->GetBit(i), this->Trash8_2->GetBit(i));
		if (i == 3) {
			c = a;
		}
		else if (i == 7) {
			d = a;
		}
	}
	a = (sum == 0);
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint16_t Processor::SUBHelper16(bool cin, uint16_t firstWord, uint16_t secondWord) {
	uint16_t sum = firstWord - secondWord;
	this->Trash16->SetWord(0, firstWord);
	this->Trash16_2->SetWord(0, secondWord);
	bool a = cin, b = 1, c = 0, d = 0;
	for (int i = 0; i < 16; i++) {
		c = this->borrow(c, this->Trash16->GetBit(i), this->Trash16_2->GetBit(i));
		if (i == 11) {
			this->HalfCarryFlag = c;
		}
		else if (i == 15) {
			this->CarryFlag = c;
		}
	}
	a = (sum == 0);
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint8_t Processor::ANDHelper(uint8_t firstByte, uint8_t secondByte) {
	uint8_t sum = firstByte & secondByte;
	bool a = (sum == 0), b = 1, c = 0, d = 0;
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint8_t Processor::ORHelper(uint8_t firstByte, uint8_t secondByte) {
	uint8_t sum = firstByte | secondByte;
	bool a = (sum == 0), b = 0, c = 0, d = 0;
	this->FlagSetter(a, b, c, d);
	return sum;
}

uint8_t Processor::XORHelper(uint8_t firstByte, uint8_t secondByte) {
	this->Trash8->SetByte(0, firstByte);
	this->Trash8_2->SetByte(0, secondByte);
	for (int i = 0; i < 8; i++) {
		this->Trash8->SetBit(i, (this->Trash8->GetBit(i) != this->Trash8_2->GetBit(i)));
	}
	uint8_t sum = this->Trash8->GetByte(0);
	bool a = (sum == 0), b = 0, c = 0, d = 0;
	this->FlagSetter(a, b, c, d);
	return sum;
}

void Processor::CPHelper(uint8_t firstByte, uint8_t secondByte) {
	uint8_t sum = firstByte - secondByte;
	this->Trash16->SetWord(0, firstByte);
	this->Trash16_2->SetWord(0, secondByte);
	bool a = 0, b = 1, c = 0, d = 0;
	for (int i = 0; i < 16; i++) {
		a = this->borrow(a, this->Trash16->GetBit(i), this->Trash16_2->GetBit(i));
		if (i == 3) {
			c = a;
		}
		else if (i == 7) {
			d = a;
		}
	}
	a = (sum == 0);
	this->FlagSetter(a, b, c, d);
}

//8-bit
//----------------------------------------------------------

void Processor::ADD(Register* X, Register* Y) {
	X->SetByte(0, this->ADDHelper(false, X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::ADD(Register* X, uint8_t n) {
	X->SetByte(0, this->ADDHelper(false, X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::ADD(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->ADDHelper(false, X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::ADC(Register* X, Register* Y) {
	X->SetByte(0, this->ADDHelper(true, X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::ADC(Register* X, uint8_t n) {
	X->SetByte(0, this->ADDHelper(true, X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::ADC(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->ADDHelper(true, X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//----------------------------------------------------

void Processor::SUB(Register* X, Register* Y) {
	X->SetByte(0, this->SUBHelper(false, X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}


void Processor::SUB(Register* X, uint8_t n) {
	X->SetByte(0, this->SUBHelper(false, X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SUB(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->SUBHelper(false, X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//------------------------------------------------------------

void Processor::SBC(Register* X, Register* Y) {
	X->SetByte(0, this->SUBHelper(true, X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}


void Processor::SBC(Register* X, uint8_t n) {
	X->SetByte(0, this->SUBHelper(true, X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SBC(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->SUBHelper(true, X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//------------------------------------------------------------

void Processor::AND(Register* X, Register* Y) {
	X->SetByte(0, this->ANDHelper(X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::AND(Register* X, uint8_t n) {
	X->SetByte(0, this->ANDHelper(X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::AND(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->ANDHelper(X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//-----------------------------------------------------------------

void Processor::OR(Register* X, Register* Y) {
	X->SetByte(0, this->ORHelper(X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::OR(Register* X, uint8_t n) {
	X->SetByte(0, this->ORHelper(X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::OR(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->ORHelper(X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//----------------------------------------------------------------

void Processor::XOR(Register* X, Register* Y) {
	X->SetByte(0, this->XORHelper(X->GetByte(0), Y->GetByte(0)));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::XOR(Register* X, uint8_t n) {
	X->SetByte(0, this->XORHelper(X->GetByte(0), n));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::XOR(Register* X, Register* H, Register* L) {
	X->SetByte(0, this->XORHelper(X->GetByte(0), this->FromMemory(H, L)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//----------------------------------------------------------------

void Processor::CP(Register* X, Register* Y) {
	this->CPHelper(X->GetByte(0), Y->GetByte(0));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::CP(Register* X, uint8_t n) {
	this->CPHelper(X->GetByte(0), n);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::CP(Register* X, Register* H, Register* L) {
	this->CPHelper(X->GetByte(0), this->FromMemory(H, L));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//----------------------------------------------------------------

void Processor::INC(Register* X) {
	X->SetByte(0, this->ADDHelper(false, X->GetByte(0), 0x01));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::INC(Register* H, Register* L) {
	this->ToMemory(H, L, this->ADDHelper(false, this->FromMemory(H, L), 0x01));
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x0C);
}

void Processor::DEC(Register* X) {
	X->SetByte(0, this->SUBHelper(false, X->GetByte(0), 0x01));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::DEC(Register* X, Register* Y) {
	this->ToMemory(H, L, this->SUBHelper(false, this->FromMemory(H, L), 0x01));
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x0C);
}

/*
	16-bit
*/

void Processor::ADDPairs(Register* X, Register* Y, Register* Z, Register* W) {
	uint16_t n = this->ADDHelper16(	false, this->RegisterPairContents(X, Y), this->RegisterPairContents(Z, W));
	this->ToRegisterPair(X, Y, n);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::ADDSP(Register* SP, uint8_t n) {
	SP->SetWord(0, this->ADDHelper16(false, SP->GetWord(0), this->ByteToWord(n)));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::INCPair(Register* X, Register* Y) {
	this->ToRegisterPair(X, Y, (this->ADDHelper16(false, this->RegisterPairContents(X, Y), 0x0001)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::DECPair(Register* X, Register* Y) {
	this->ToRegisterPair(X, Y, (this->SUBHelper16(false, this->RegisterPairContents(X, Y), 0x0001)));
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

//----------------------------------------------------------------------------

void Processor::RLCA() {
	bool c = this->A->GetBit(7);
	this->CarryFlag = c;
	for (int i = 7; i > 0; i--) {
		this->A->SetBit(i,this->A->GetBit(i-1));
	}
	this->A->SetBit(0, c);
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::RLA() {
	bool c = this->CarryFlag;
	this->CarryFlag = this->A->GetBit(7);
	for (int i = 7; i > 0; i--) {
		this->A->SetBit(i, this->A->GetBit(i - 1));
	}
	this->A->SetBit(0, c);
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::RRCA() {
	bool c = this->A->GetBit(0);
	this->CarryFlag = c;
	for (int i = 0; i < 7; i++) {
		this->A->SetBit(i, this->A->GetBit(i + 1));
	}
	this->A->SetBit(7, c);
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::RRA() {
	bool c = this->CarryFlag;
	this->CarryFlag = this->A->GetBit(0);
	for (int i = 0; i < 7; i++) {
		this->A->SetBit(i, this->A->GetBit(i + 1));
	}
	this->A->SetBit(7, c);
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::RLC(Register* X) {
	bool c = X->GetBit(7);
	this->CarryFlag = c;
	for (int i = 7; i > 0; i--) {
		X->SetBit(i, X->GetBit(i - 1));
	}
	X->SetBit(0, c);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::RLC(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	bool c = this->Trash8->GetBit(7);
	this->CarryFlag = c;
	for (int i = 7; i > 0; i--) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i - 1));
	}
	this->Trash8->SetBit(0, c);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);

}

void Processor::RL(Register* X) {
	bool c = this->CarryFlag;
	this->CarryFlag = X->GetBit(7);
	for (int i = 7; i > 0; i--){
		X->SetBit(i, X->GetBit(i - 1));
	}
	X->SetBit(0, c);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::RL(Register* H, Register* L) {
	bool c = this->CarryFlag;
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	for (int i = 7; i > 0; i--) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i - 1));
	}
	this->Trash8->SetBit(0, c);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}


void Processor::RRC(Register* X) {
	bool c = X->GetBit(0);
	this->CarryFlag = c;
	for (int i = 0; i < 7; i++) {
		X->SetBit(i, X->GetBit(i + 1));
	}
	X->SetBit(7, c);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}


void Processor::RRC(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	bool c = this->Trash8->GetBit(0);
	this->CarryFlag = c;
	for (int i = 0; i < 7; i++) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i + 1));
	}
	this->Trash8->SetBit(0, c);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::RR(Register* X) {
	bool c = this->CarryFlag;
	this->CarryFlag = X->GetBit(0);
	for (int i = 0; i < 7; i++) {
		X->SetBit(i, X->GetBit(i + 1));
	}
	X->SetBit(7, c);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}


void Processor::RR(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	bool c = this->CarryFlag;
	this->CarryFlag = this->Trash8->GetBit(0);
	for (int i = 0; i < 7; i++) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i + 1));
	}
	this->Trash8->SetBit(0, c);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::SLA(Register* X) {
	this->CarryFlag = X->GetBit(7);
	for (int i = 7; i > 0; i--) {
		X->SetBit(i, X->GetBit(i - 1));
	}
	X->SetBit(0, false);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SLA(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	this->CarryFlag = this->Trash8->GetBit(7);
	for (int i = 7; i > 0; i--) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i - 1));
	}
	this->Trash8->SetBit(0, false);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}


void Processor::SRA(Register* X) {
	this->CarryFlag = X->GetBit(0);
	for (int i = 0; i < 7; i++) {
		X->SetBit(i, X->GetBit(i+1));
	}
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SRA(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	this->CarryFlag = this->Trash8->GetBit(0);
	for (int i = 0; i < 7; i++) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i + 1));
	}
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::SRL(Register* X) {
	this->CarryFlag = X->GetBit(0);
	for (int i = 0; i < 7; i++) {
		X->SetBit(i, X->GetBit(i + 1));
	}
	X->SetBit(7, false);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SRL(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	this->CarryFlag = this->Trash8->GetBit(0);
	for (int i = 0; i < 7; i++) {
		this->Trash8->SetBit(i, this->Trash8->GetBit(i + 1));
	}
	this->Trash8->SetBit(7, false);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::SWAP(Register* X) {
	uint8_t temp1 = (X->GetByte(0) & 0xF0)>>4;
	uint8_t temp2 = (X->GetByte(0) & 0x0F)<<4;
	uint8_t result = temp1 + temp2;
	X->SetByte(0, result);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SWAP(Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	uint8_t temp1 = (this->Trash8->GetByte(0) & 0xF0) >> 4;
	uint8_t temp2 = (this->Trash8->GetByte(0) & 0x0F) << 4;
	uint8_t result = temp1 + temp2;
	this->ToMemory(H, L, result);
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

//Bit Operations

void Processor::BIT(uint8_t n, Register* X) {
	if (!X->GetBit(n & 0b111)) {
		this->ZeroFlag = true;
	}
	this->SubtractionFlag = false;
	this->HalfCarryFlag = true;
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::BIT(uint8_t n, Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	if (this->Trash8->GetBit(n & 0b111)) {
		this->ZeroFlag = true;
	}
	this->SubtractionFlag = false;
	this->HalfCarryFlag = true;
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x0C);
}

void Processor::SET(uint8_t n, Register* X) {
	X->SetBit(n & 0b111, true);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::SET(uint8_t n, Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	this->Trash8->SetBit(n & 0b111, 1);
	this->SubtractionFlag = false;
	this->HalfCarryFlag = true;
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::RES(uint8_t n, Register* X) {
	X->SetBit(n & 0b111, 0);
	this->M->SetByte(0, 0x02);
	this->T->SetByte(0, 0x08);
}

void Processor::RES(uint8_t n, Register* H, Register* L) {
	this->Trash8->SetByte(0, this->FromMemory(H, L));
	this->Trash8->SetBit(n & 0b111, 0);
	this->ToMemory(H, L, this->Trash8->GetByte(0));
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

//-----------------------------------------------------------------------------------------

bool Processor::JumpConditions(uint8_t cc) {
	bool success = false;
	switch (cc)
	{
	case 0b00:
		success = !this->ZeroFlag;
		break;
	case 0b01:
		success = this->ZeroFlag;
		break;
	case 0b10:
		success = !this->CarryFlag;
		break;
	case 0b11:
		success = this->CarryFlag;
		break;
	}
	return success;
}

//------------------------------------------------------------------------------------

void Processor::JP(uint16_t nn) {
	this->PC->SetWord(0, nn);
	this->M->SetByte(0, 0x04);
	this->T->SetByte(0, 0x10);
}

void Processor::JP(uint8_t cc, uint16_t nn) {
	if (this->JumpConditions(cc)) {
		this->PC->SetWord(0, nn);
		this->M->SetByte(0, 0x04);
		this->T->SetByte(0, 0x10);
	}
	else {
		this->M->SetByte(0, 0x03);
		this->T->SetByte(0, 0x0C);
	}
}

void Processor::JR(int8_t n) {
	this->PC->SetWord(0, this->PC->GetWord(0) + n);
	this->M->SetByte(0, 0x03);
	this->T->SetByte(0, 0x0C);
}

void Processor::JP(uint8_t cc, int8_t n) {
	if (this->JumpConditions(cc)) {
		this->PC->SetWord(0, this->PC->GetWord(0) + n);
		this->M->SetByte(0, 0x03);
		this->T->SetByte(0, 0x0C);
	}
	else {
		this->M->SetByte(0, 0x02);
		this->T->SetByte(0, 0x08);
	}
}

void Processor::JP(Register* H, Register* L) {
	this->PC->SetWord(0, this->RegisterPairContents(H, L));
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

//------------------------------------------------------------------------------------
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

void Processor::SCF() {
	this->F->SetByte(0, this->F->GetByte(0) | 0x10);
	this->M->SetByte(0, 0x01);
	this->M->SetByte(0, 0x04);
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

void Processor::EI() {
	this->_ime = 1;
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::NOP() {
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}

void Processor::HALT() {
	this->_halt = 1;
	this->M->SetByte(0, 0x01);
	this->T->SetByte(0, 0x04);
}
