#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include "Register.h"
#include "Definitions.h"
#include <unordered_map>
#include <functional>

class Memory;

class Processor
{
public:
<<<<<<< HEAD
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
    std::unordered_map<uint8_t, std::function<void(Processor* p)>>* operations;
=======
    Processor(Memory* pMemory);
    ~Processor();
    NOP();
    Increment(Register A);
    Decrement(Register A);
    Load(Register A, Register B);
    8Bit_Add(Register B);
    8Bit_Add_Mem();
    8Bit_Add_unsigned(uint8_t n);
    8Bit_Add_signed(int8_t n);
    16Bit_Add(Register A, Register B);
    16Bit_Add_SP();
    8Bit_Sub(Register B);
    8Bit_Sub_Mem();
    8Bit_Sub_unsigned(uint8_t n);
    Compare(Register A, Register B);
    RotateLeft(Register A);
    RotateLeftwc(Register A); //with carry
    RotateRight(Register A);
    RotateRightwc(Register A); //with carry
    ShiftLeft(Register A);
    ShiftLeft_HL();
    ShiftRight(Register A);
    ShiftRight_HL();
    Complement(Register A); //Logical NOT
    OR(Register A, Register B);
    AND(Register A, Register B);
    XOR(Register A, Register B);
    Push(Register SP);
    Pop(Register SP);
    ClearCarryFlag(Register F);
    Reset(???);
};

private:
    Registers A(8);
    Registers B(8);
    Registers C(8);
    Registers D(8);
    Registers E(8);
    Registers H(8);
    Registers L(8);
    Registers F(8);
    Registers PC(16);
    Registers SP(16);
>>>>>>> refs/remotes/origin/master
    unsigned int mClock;
    unsigned int tClock;

public:
    Processor(Memory* pMemory);
    ~Processor();

    void InitOpcodes();
    void ProcessOpcode(uint8_t code);

/*
/   Instruction Sets
*/

    /*
    /   8 and 16-bit Loads
    */

        /*
        /   Register to register
        */

        void LoadFrom(Register* X);                              //Loads the contents of register A into register X.
        void LoadFrom();                                        //Loads the contents of memory address (HL) into register A.

        void LoadTo(Register* X);                                //Loads the contents of register X into register A.
        void LoadTo();                                          //Loads the contents of register A into memory address (HL).

        /*
        /   Direct loading of registers
        */

        void DirectLoad(uint16_t n);                                //Loads from memory location ($uint16_t) into register A.
        void DirectLoad(Register* X, Register* Y, uint16_t n);        //Loads from memory locations ($uint16_t) and
                                                                    //($uint16_t+4) into registers X and Y.
        void DirectLoadSP(uint16_t n);                              //Loads from memory location ($uint16_t) into Stack Pointer.

        /*
        /   Immediate loading of registers
        /   Can be used to load any specific register or register pair with a specific fixed value.
        */

        void ImmediateLoad(Register* X, uint8_t n);                  //Loads register X with the value $uint8_t.
        void ImmediateLoad(Register* X, Register* Y, uint16_t n);     //Loads registers X and Y with the value $uint16_t.

        /*
        /   Stack loading of registers
        */

        void PopXY(Register* X, Register* Y);              //Loads a register pair from the top of the stack, incrementing SP.

        /*
        /   Storing registers in memory
        */

        void Store(uint16_t loc, Register* X);     //Loads the contents of register X into memory location ($Loc).
        void Store(uint16_t loc);                 //Loads the contents of register A into memory location ($Loc).
        void Store(Register* X);                     //Loads the contents of register X into memory location (HL).
        void Store();                               //Loads the contents of register A into memory location (HL).
        void StoreValue(uint8_t n);                  //Loads the value n into memory location (HL).

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
        void ADD(Register* X, int8_t n);

        void ADC(Register* X);
        void ADC(uint8_t n);
        void ADC();

        void SUB(Register* X, Register* Y);
        void SUB(Register* X, uint8_t n);
        void SUB();

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
        void INC();

        void DEC(Register* X);
        void DEC();

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
};

#endif	/* PROCESSOR_H */
