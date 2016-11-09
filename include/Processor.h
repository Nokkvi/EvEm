#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Definitions.h"
#include "GPU.h"
#include "Memory.h"
#include "Register.h"
#include <unordered_map>
#include <functional>

class Processor
{
public:
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

            // Loads into register pair XY the immediate nn.
            void Load(Register* X, Register* Y, uint16_t nn);

            // Loads into 16 bit register r the values in register pair XY.
            void Load(Register* r, Register* X, Register* Y);

            // Loads into 16 bit register r the values in register pair XY, simultaneously increment the contents of register pair XY.
            void Load_Increment(Register* r, Register* X, Register* Y);

            // Loads into 16 bit register r the values in register pair XY, simultaneously decrement the contents of register pair XY.
            void Load_Decrement(Register* r, Register* X, Register* Y);

<<<<<<< HEAD
    // From Immediate

=======
>>>>>>> Rewrote the Load and Store functions in the Processor and fixed some typos.
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
        void ADDSIGNED(Register* SP, int8_t n);
        void ADDHL(Register* X, Register* H, Register* L);

        void ADC(Register* X, Register* H, Register* L);
        void ADC(Register* X, Register* Y);
        void ADC(Register* X, uint8_t n);

        void SUB(Register* X, Register* Y);
        void SUB(Register* X, uint8_t n);
        void SUB(Register* A, Register* H, Register* L);

        void SBC(Register* X, Register* H, Register* L);
        void SBC(Register* X, Register* Y);
        void SBC(Register* X, uint8_t n);

        void AND(Register* X);
        void AND(uint8_t n);
        void AND(Register* H, Register* L);

        void OR(Register* X);
        void OR(uint8_t n);
        void OR(Register* H, Register* L);

        void XOR(Register* X);
        void XOR(uint8_t n);
        void XOR(Register* H, Register* L);

        void CP(Register* X, Register* Y);
        void CP(Register* X,uint8_t n);
        void CP(Register* X, Register* H, Register* L);

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
    void RLCHL();

    void RLA();

    void RL(Register* X);
    void RLHL();

    //------------------------------------------------

    void RRCA();

    void RRC(Register* X);
    void RRCHL();

    void RRA();

    void RR(Register* X);
    void RRHL();

    //--------------------------------------------------

    void SLA(Register* X);
    void SLA();

    void SRA(Register* X);
    void SRA();

    void SRL(Register* X);
    void SRL();


    //   Bit manipulation


    void BIT(uint8_t b, Register* X);
    void BIT(uint8_t b);

    void SET(uint8_t b, Register* X);
    void SET(uint8_t b);

    void RES(uint8_t b, Register* X);
    void RES(uint8_t b);


    //   General purpose Arithmetic and CPU control


    void DAA();

    void CPL();

    void SWAP(Register* X);
    void SWAP();

    void CCF();

    void SCF();

    void JR(int8_t value);

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
