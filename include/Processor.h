#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "definitions.h"
#include "SixteenBitRegister.h"
#include "boot_roms.h"

class Memory;

class Processor
{
public:
    Processor(Memory* pMemory);
    ~Processor();

/*
/   Instruction Sets
*/

    /*
    /   8 and 16-bit Loads
    */

        /*
        /   Register to register
        */

        void LoadFrom(Register X);                              //Loads the contents of register A into register X.
        void LoadFrom();                                        //Loads the contents of memory address (HL) into register A.

        void LoadTo(Register X);                                //Loads the contents of register X into register A.
        void LoadTo();                                          //Loads the contents of register A into memory address (HL).

        /*
        /   Direct loading of registers
        */

        void DirectLoad(uint16_t n);                               //Loads from memory location ($uint16_t) into register A.
        void DirectLoad(Register X, Register Y, uint16_t n);      //Loads from memory locations ($uint16_t) and
                                                                    //($uint16_t+4) into registers X and Y.
        void DirectLoadSP(uint16_t n);                              //Loads from memory location ($uint16_t) into Stack Pointer.

        /*
        /   Immediate loading of registers
        /   Can be used to load any specific register or register pair with a specific fixed value.
        */

        void ImmediateLoad(Register X, uint8_t n);                 //Loads register X with the value $uint8_t.
        void ImmediateLoad(Register X, Register Y, uint16_t n);   //Loads registers X and Y with the value $uint16_t.

        /*
        /   Stack loading of registers
        */

        void PopXY(Register X, Register Y);              //Loads a register pair from the top of the stack, incrementing SP.

        /*
        /   Storing registers in memory
        */

        void Store(Memory Loc, Register X);     //Loads the contents of register X into memory location ($Loc).
        void Store(Memory Loc);                 //Loads the contents of register A into memory location ($Loc).
        void Store(Register X);                     //Loads the contents of register X into memory location (HL).
        void Store();                               //Loads the contents of register A into memory location (HL).
        void StoreValue(uint8_t n);                  //Loads the value n into memory location (HL).

    /*
    /   Arithmetic and Logical Instructions
    */

        /*
        /   8-bit
        */

        void ADD(Register X);
        void ADD(uint8_t n);
        void ADD();

        void ADC(Register X);
        void ADC(uint8_t n);
        void ADC();

        void SUB(Register X);
        void SUB(uint8_t n);
        void SUB();

        void SBC(Register X);
        void SBC(uint8_t n);
        void SBC();

        void AND(Register X);
        void AND(uint8_t n);
        void AND();

        void OR(Register X);
        void OR(uint8_t n);
        void OR();

        void XOR(Register X);
        void XOR(uint8_t n);
        void XOR();

        void CP(Register X);
        void CP(uint8_t n);
        void CP();

        void INC(Register X);
        void INC();

        void DEC(Register X);
        void DEC();

        /*
        /   16-bit
        */

        void ADD(Register X, Register Y);

        void INC(Register X, Register Y);

        void DEC(Register X, Register Y);

    /*
    /   Rotate and Shift Operations
    */

    void RLCA();

    void RLC(Register X);
    void RLC();

    void RLA();

    void RL(Register X);
    void RL();

    //------------------------------------------------

    void RRCA();

    void RRC(Register X);
    void RRC();

    void RRA();

    void RR(Register X);
    void RR();

    //--------------------------------------------------

    void SLA(Register X);
    void SLA();

    void SRA(Register X);
    void SRA();

    void SRL(Register X);
    void SRL();

    /*
    /   Bit manipulation
    */

    void BIT(Bit b, Register X);
    void BIT(Bit b);

    void SET(Bit b, Register X);
    void SET(Bit b);

    void RES(Bit b, Register X);
    void RES(Bit b);

    /*
    /   General purpose Arithmetic and CPU control
    */

    void DAA();

    void CPL();

    void SWAP(Register X);
    void SWAP();

    void CCF();

    void SCF();

    void NOP();

    void HALT();

    void STOP();

    void DI();

    void EI();








    Compare(Register A, Register B);
    Complement(Register A); //Logical NOT
    Push(Register SP);
    Pop(Register SP);
    ClearCarryFlag(Register F);
    Reset(???);
};

private:
    Register A;
    Register B;
    Register C;
    Register D;
    Register E;
    Register H;
    Register L;
    Register F;
    Register PC;
    Register SP;
    unsigned int mClock;
    unsigned int tClock;

#endif	/* PROCESSOR_H */
