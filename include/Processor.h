#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Definitions.h"
#include "Memory.h"
#include "Register.h"
#include <unordered_map>
#include <functional>

class Processor
{
public:
    Register* A = new Register(1);
    Register* B = new Register(1);
    Register* C = new Register(1);
    Register* D = new Register(1);
    Register* E = new Register(1);
    Register* H = new Register(1);
    Register* L = new Register(1);
    Register* F = new Register(1);
    Register* PC = new Register(2);
    Register* SP = new Register(2);

    //Clock registers
    Register* M = new Register(1);
    Register* T = new Register(1);

private:
    std::unordered_map<uint8_t, std::function<void(Processor* p, unsigned int* m, unsigned int* t)>>* operations;
    unsigned int mClock;
    unsigned int tClock;

    Memory* memory;
public:
    Processor(Memory* memory);
    ~Processor();

    inline bool IsRunning() { return true; };

    void InitOpcodes();
    inline uint8_t FetchInstruction() { return memory->GetByte((((uint16_t)PC->GetByte(1)) << 8) & PC->GetByte(0)); };
    void ProcessOpcode(uint8_t code);

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

            // Loading into register r the contents of memory address (0xFFn).
            void Load(Register* r, uint8_t n);

            /*Loads into register r the contents of the internal RAM or register specified
            by the 16-bit immediate operand nn.*/
            void Load(Register* r, uint16_t nn);

            /* Loads into register r the contents of internal RAM, port register, or mode
            register at the address in the range 0xFF00-0xFFFF specified by register X.
            Loads r <-- ($FF00+X).*/
            void Load(Register* r, Register* X);

            /*Loads into register r the contents of memory specified by the contents of
            register pair xy, simultaneously incrememnt the contents of HL.*/
            void Load_Increment(Register* r, Register* X, Register* Y);

            /*Loads into register r the contents of memory specified by the contents of
            register pair xy, simultaneously decrementing the contents of HL.*/
            void Load_Decrement(Register* r, Register* X, Register* Y);

        /*From Register*/

            // Loads into register r the contents of register X.
            void Load_Register(Register* r, Register* X);

        /*Immediate Value*/

            // Loads into register r the immediate value of n.
            void Load_Immediate(Register* r, uint8_t n);

    /*To Memory*/

        /*From Register*/

            /*Loading from register r into memory address (0xFF00-0xFFFF) determined by
            the value of n.*/
            void Store(Register* r, uint8_t n);

            /*Stores the contents of register A at the internal RAM or register specified
            by the 16-bit immediate operand nn.*/
            void Store(Register* r, uint16_t nn);

            // Loads r -- > ($FF00+X)
            void Store(Register* r, Register* X);

            // Stores the contents of register r in memory specified by register pair xy.
            void Store(Register* r, Register* X, Register* Y);

            /*Store the contents of register r in the memory specified by register pair xy,
            simultaneously increment the contents of xy.*/
            void Store_Increment(Register* r, Register* X, Register* Y);

            /*Store the contents of register r in the memory specified by register pair xy,
            simultaneously decrement the contents of xy.*/
            void Store_Decrement(Register* r, Register* X, Register* Y);

        /*Immediate Value*/

            // Loads 8-bit immediate data n into memory specified by register pair xy.
            void Store(Register* X, Register* Y, uint8_t n);

/*
/   16-bit loads
*/

    /*To Register*/

        /*Immediate Data*/

            // Load two bytes of immediate data to register pair xy.
			void Load(uint8_t n, uint8_t m, Register* X, Register* Y);

        /*From Stack Pointer*/

            // The 8-bit operand e is added to SP and the result is stored in HL.
			void Load(Register* SP, Register* X, Register* Y, uint8_t e);

    /*To Memory*/

        /*From Stack Pointer*/

            /*Stores the lower byte of SP at address nn specified by the 16-bit immediate
            operand nn and the upper byte of SP at address nn + 1.*/
			void Store_SP(Register* SP, uint16_t nn);

    /*To Stack Pointer*/

        /*From Registers*/

            // Load the contents of register pair HL(not the memory location) in stack pointer SP.
			void Load(Register* X, Register* Y, Register* SP);

    /*Not sure how to classify push and pop....*/

        /*Pushes the contents of register pair qq onto the memory stack. First 1 is
        subtracted from SP and the contents of the higherportion of qq are placed on
        the stack. The contents of the lower portion of qq are then placed on the
        stack. The contents of SP are automatically decremented by 2.*/
        void Push(Register* SP, Register* X, Register* Y);

        /*Pops the contents from the memory stack and into register pair qq. First
        the contents of memory, specified by the contents of SP are loaded in the
        lower portion of qq. Next, the contents of SP are incremented by 1 and the
        contents of the memory they specify are loaded in the upper portion of qq.
        The contents of SP are automatically incremented by 2.*/
        void Pop(Register* SP, Register* X, Register* Y);

    /*
    /   Arithmetic and Logical Instructions
    */

        /*
        /   8-bit
        */

        void ADD(Register* X, Register* Y);
        void ADD(Register* X, Register* Y, Register* Z, Register* W);
        void ADD(Register* X, Register* Y, Register* ZW);
        void ADD(Register* X, uint8_t n);
        void ADD(Register* SP, int8_t n);
        void ADDHL(Register* X, Register* H, Register* L);

        void ADC(Register* X);
        void ADC(uint8_t n);
        void ADC();

        void SUB(Register* X, Register* Y);
        void SUB(Register* X, uint8_t n);
        void SUB(Register* A, Register* H, Register* L);

        void SBC(Register* X);
        void SBC(uint8_t n);
        void SBC();

        void AND(Register* X);
        void AND(uint8_t n);
        void AND();

        void OR(Register* X);
        void OR(uint8_t n);
        void OR();

        void XOR(Register* X);
        void XOR(uint8_t n);
        void XOR();

        void CP(Register* X);
        void CP(uint8_t n);
        void CP();

        void INC(Register* X);
        void INC(Register* X, Register* Y);
        void INCHL(Register* H, Register* L);

        void DEC(Register* X);
        void DEC(Register* X, Register* Y);
        void DECHL(Register* H, Register* L);

        /*
        /   16-bit
        */

        //void ADD(Register* X, Register* Y);

        //void INC(Register* X, Register* Y);

        //void DEC(Register* X, Register* Y);

    /*
    /   Rotate and Shift Operations
    */

    void RLCA();

    void RLC(Register* X);
    void RLC();

    void RLA();

    void RL(Register* X);
    void RL();

    //------------------------------------------------

    void RRCA();

    void RRC(Register* X);
    void RRC();

    void RRA();

    void RR(Register* X);
    void RR();

    //--------------------------------------------------

    void SLA(Register* X);
    void SLA();

    void SRA(Register* X);
    void SRA();

    void SRL(Register* X);
    void SRL();

    /*
    /   Bit manipulation
    */

    //void BIT(Bit b, Register* X);
    //void BIT(Bit b);

    //void SET(Bit b, Register* X);
    //void SET(Bit b);

    //void RES(Bit b, Register* X);
    //void RES(Bit b);

    /*
    /   General purpose Arithmetic and CPU control
    */

    void DAA();

    void CPL();

    void SWAP(Register* X);
    void SWAP();

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

    //Helper Functions
    void FlagHelper(uint16_t n, int as);
};

#endif	/* PROCESSOR_H */
