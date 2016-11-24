#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Definitions.h"
#include "GPU.h"
#include "Memory.h"
#include "Register.h"
#include <unordered_map>
#include <functional>
#include "Timer.h"

class Processor
{
public:
    Timer* Timari;
    Register* A;
    Register* B;
    Register* C;
    Register* D;
    Register* E;
    Register* H;
    Register* L;
    Register* F;
    Register* PC;
    Register* SP;

	Register* Trash8 = new Register(1);
	Register* Trash8_2 = new Register(1);
	Register* Trash16 = new Register(2);
	Register* Trash16_2 = new Register(2);

	//Flags

	/*Is set if the result of a math operation is 0 or two values match when
	using CP.*/
	bool ZeroFlag = false;
	/*This bit is set if a subtraction was performed in the
	last math instruction.*/
	bool SubtractionFlag = false;
	/*This bit is set if a carry occurred from the lower
	nibble in the last math operation.*/
	bool HalfCarryFlag = false;
	/*This bit is set if a carry occurred from the last
	math operation or if register A is the smaller value
	when executing the CP instruction.*/
	bool CarryFlag = false;

    //Clock registers
    Register* M;
    Register* T;

    unsigned int mTotal, tTotal;

    bool cbInstruction;

    uint16_t _halt, _stop, _ime;
    Memory* memory;
private:
    std::unordered_map<uint8_t, std::function<void(Processor* p)>>* operations;
    std::unordered_map<uint8_t, std::function<void(Processor* p)>>* cboperations;

    GPU* gpu;
public:
    Processor(GPU* gpu, Memory* memory);
    ~Processor();

    inline bool IsRunning() { return true; };

    void InitOpcodes();
    uint8_t FetchInstruction();
    void ProcessOpcode(uint8_t code);

    uint8_t Get8BitImmediate();
    uint16_t Get16BitImmediate();

    void Render();
/*
/   Instruction Sets
*/

    /*
    /   8 and 16-bit Loads
    */

// --------------------------------------------------------------

/*
/   8-bit loads
*/

    /*To Register*/

        /*From Memory*/

            // Loading into register r the 8 bit immediate n.
            void Load(Register* r, uint8_t n);

            // Loads into register r the 16 bit immediate nn
            void Load(Register* r, uint16_t nn);

            // Loads into 8 bit register r the contents of 8 bit register X.
            void Load(Register* r, Register* X);

            void LDAIOn();

            void LDIOCA();

            void LDmmA();

            void LDHLSPn();

            // Loads into register pair XY the immediate nn.
            void Load(Register* X, Register* Y, uint16_t nn);

            // Loads into 16 bit register r the values in register pair XY.
            void Load(Register* r, Register* X, Register* Y);

            // Loads into 16 bit register r the values in register pair XY, simultaneously increment the contents of register pair XY.
            void Load_Increment(Register* r, Register* X, Register* Y);

            // Loads into 16 bit register r the values in register pair XY, simultaneously decrement the contents of register pair XY.
            void Load_Decrement(Register* r, Register* X, Register* Y);

    /*To Memory*/

        /*From Register*/

            // Stores the contents of 8 bit register r at address nn.
            void Store(Register* r, uint16_t nn);

            // Stores the contents of 8 bit register r at address specified by 16 bit register X.
            void Store(Register* r, Register* X);

            // Stores the contents of register r in memory specified by register pair XY.
            void Store(Register* r, Register* X, Register* Y);

            // Stores the contents of register r in memory specified by register pair XY, simultaneously increment the contents of XY.
            void Store_Increment(Register* r, Register* X, Register* Y);

            // Stores the contents of register r in memory specified by register pair XY, simultaneously decrement the contents of XY.
            void Store_Decrement(Register* r, Register* X, Register* Y);

        /*Immediate Value*/

            // Loads 8-bit immediate data n into memory specified by register pair xy.
            void Store(Register* X, Register* Y, uint8_t n);

/*
/   16-bit loads
*/
/*
    //To Register

        //Immediate Data

            // Load two bytes of immediate data to register pair xy.
			void Load(Register* X, Register* Y, uint16_t nn);

        //From Stack Pointer

            // The 8-bit operand e is added to SP and the result is stored in HL.
			void Load(Register* SP, Register* X, Register* Y, uint8_t e);

    //To Memory

        //From Stack Pointer

            // Stores the lower byte of SP at address nn specified by the 16-bit immediate
            // operand nn and the upper byte of SP at address nn + 1.
			void Store_SP(Register* SP, uint16_t nn);

    //To Stack Pointer

        //From Registers

            // Load the contents of register pair HL(not the memory location) in stack pointer SP.
			void Load_HL(Register* X, Register* Y, Register* SP);
*/
    /*Not sure how to classify push and pop....*/

        /*Pushes the contents of register pair qq onto the memory stack. First 1 is
        subtracted from SP and the contents of the higherportion of qq are placed on
        the stack. The contents of the lower portion of qq are then placed on the
        stack. The contents of SP are automatically decremented by 2.*/
        void Push(Register* X, Register* Y);

        /*Pops the contents from the memory stack and into register pair qq. First
        the contents of memory, specified by the contents of SP are loaded in the
        lower portion of qq. Next, the contents of SP are incremented by 1 and the
        contents of the memory they specify are loaded in the upper portion of qq.
        The contents of SP are automatically incremented by 2.*/
        void Pop(Register* X, Register* Y);

    /*
    /   Arithmetic and Logical Instructions
    */

        /*
        /   8-bit
        */

		void FlagSetter(bool zeroFlag, bool subtractionFlag, bool halfCarryFlag, bool carryFlag);
		bool carry(bool cIn, bool X, bool Y);
		bool borrow(bool cIn, bool X, bool Y);
		uint8_t WordToByte(bool index, uint16_t n);
		uint16_t ByteToWord(uint8_t n);
		uint16_t RegisterPairContents(Register* X, Register* Y);
		void ToRegisterPair(Register* X, Register* Y, uint16_t n);

		/*---------------------------------*/
		uint8_t ADDHelper(bool cin, uint8_t firstByte, uint8_t secondByte);
		uint16_t ADDHelper16(bool cin, uint16_t firstWord, uint16_t secondWord);
		uint8_t SUBHelper(bool cin, uint8_t firstByte, uint8_t secondByte);
		uint16_t SUBHelper16(bool cin, uint16_t firstWord, uint16_t secondWord);
		uint8_t ANDHelper(uint8_t firstByte, uint8_t secondByte);
		uint8_t ORHelper(uint8_t firstByte, uint8_t secondByte);
		uint8_t XORHelper(uint8_t firstByte, uint8_t secondByte);
		void CPHelper(uint8_t firstByte, uint8_t secondByte);
		bool JumpConditions(uint8_t cc);

		/*-----------------------------------*/
		uint8_t FromMemory(Register* X, Register* Y);
		void ToMemory(Register* X, Register* Y, uint8_t n);

		/*-----------------------------------*/
		/*Pass in Registers X and Y and add their contents together into X.*/
        void ADD(Register* X, Register* Y);
		void ADD(Register* X, uint8_t n);
		void ADD(Register* X, Register* H, Register* L);

		void ADC(Register* X, Register* Y);
		void ADC(Register* X, uint8_t n);
		void ADC(Register* X, Register* H, Register* L);

		void SUB(Register* X, Register* Y);
		void SUB(Register* X, uint8_t n);
		void SUB(Register* X, Register* H, Register* L);

		void SBC(Register* X, Register* Y);
		void SBC(Register* X, uint8_t n);
		void SBC(Register* X, Register* H, Register* L);

		void AND(Register* X, Register* Y);
		void AND(Register* X, uint8_t n);
		void AND(Register* X, Register* H, Register* L);

		void OR(Register* X, Register* Y);
		void OR(Register* X, uint8_t n);
		void OR(Register* X, Register* H, Register* L);

		void XOR(Register* X, Register* Y);
		void XOR(Register* X, uint8_t n);
		void XOR(Register* X, Register* H, Register* L);

		void CP(Register* X, Register* Y);
		void CP(Register* X, uint8_t n);
		void CP(Register* X, Register* H, Register* L);

		void INC(Register* X);
		void INC(Register* H, Register* L);

		void DEC(Register* X);
		void DEC(Register* H, Register* L);

		//16-bit

		void ADDPairs(Register* X, Register* Y, Register* Z, Register* W);
		void ADDSP(int8_t n);
		void INCPair(Register* X, Register* Y);
		void DECPair(Register* X, Register* Y);

		//-------------------------------------------------------
		//Rotate Shift Instructions

		void RLCA();
		void RLA();
		void RRCA();
		void RRA();

		void RLC(Register* X);
		void RLC(Register* H, Register* L);

		void RL(Register* X);
		void RL(Register* H, Register* L);

		void RRC(Register* X);
		void RRC(Register* H, Register* L);

		void RR(Register* X);
		void RR(Register* H, Register* L);

		void SLA(Register* X);
		void SLA(Register* H, Register* L);

		void SRA(Register* X);
		void SRA(Register* H, Register* L);

		void SRL(Register* X);
		void SRL(Register* H, Register* L);

		void SWAP(Register* X);
		void SWAP(Register* H, Register* L);

		void JP(uint16_t nn);
		bool JP(uint8_t cc, uint16_t nn);

		void JR(int8_t n);
		bool JR(uint8_t cc, int8_t n);

		void JP(Register* H, Register* L);





    //   Bit manipulation


    void BIT(uint8_t n, Register* X);
    void BIT(uint8_t n, Register* H, Register* L);

    void SET(uint8_t n, Register* X);
	void SET(uint8_t n, Register* H, Register* L);

    void RES(uint8_t n, Register* X);
    void RES(uint8_t n, Register* H, Register* L);


    //   General purpose Arithmetic and CPU control

	void CALL(uint16_t nn);
	void CALL(uint8_t cc, uint16_t nn);

	//Pop two bytes from stack and jump to that address.
	void RET();
	void RET(uint8_t cc);
	void RETI();
	void RST(uint8_t t);

	/////////////////////////////////////////////////////////

	void DAA();
	void CPL();
    void CCF();
    void SCF();

    void NOP();

    void HALT();

    void STOP();

    void DI();

    void EI();

    //Compare(Register* A, Register* B);
    //Push(Register* SP);
    //Pop(Register* SP);
    //ClearCarryFlag(Register* F);
    void Reset();

};

#endif	/* PROCESSOR_H */
