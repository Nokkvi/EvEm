#include "../include/Processor.h"

uint8_t Processor::FetchInstruction()
{
    uint16_t pc = this->PC->GetWord(0);
    this->PC->Increment();
    if (pc == 0x1000)
    {
        memory->SetBIOS(false);
    }
    return memory->GetByte(pc);
};


void Processor::ProcessOpcode(uint8_t code)
{

    (*operations)[code](this);
    if (this->cbInstruction)
    {
        this->cbInstruction = false;
        (*cboperations)[FetchInstruction()](this);
    }

    this->mTotal += this->M->GetByte(0);
    this->tTotal += this->T->GetByte(0);
    this->M->SetByte(0, 0);
    this->T->SetByte(0, 0);

    if(this->_ime && this->memory->ie && this->memory->interruptFlags){
        unsigned int ifired = (this->memory->ie & this->memory->interruptFlags);
        if(ifired & 0x01){
            this->memory->interruptFlags &= (255-0x01);
            this->RST(0x40);
        }
    }
    this->mTotal += this->M->GetByte(0);
    this->mTotal += this->T->GetByte(0);
}


uint8_t Processor::Get8BitImmediate()
{
    uint16_t pc = this->PC->GetWord(0);
    this->PC->Increment();
    return this->memory->GetByte(pc);
}

uint16_t Processor::Get16BitImmediate()
{
    uint16_t pc = this->PC->GetWord(0);
    this->PC->Increment();
    this->PC->Increment();
    return this->memory->GetWord(pc);

}

void Processor::InitOpcodes()
{
	operations = new std::unordered_map<uint8_t, std::function<void(Processor* p)>>();

    (*operations)[0x00] = [](Processor* p) {
        p->NOP();
    };

    (*operations)[0x01] = [](Processor* p) {
        p->Load(p->B, p->C, p->Get16BitImmediate());
    };

    (*operations)[0x02] = [](Processor* p) {
        p->Store(p->A, p->B, p->C);
    };

    (*operations)[0x03] = [](Processor* p) {
        p->INCPair(p->B, p->C);
    };

    (*operations)[0x04] = [](Processor* p) {
        p->INC(p->B);
    };

    (*operations)[0x05] = [](Processor* p) {
        p->DEC(p->B);
    };

    (*operations)[0x06] = [](Processor* p) {
		p->Load(p->B, p->Get8BitImmediate());
    };

    (*operations)[0x07] = [](Processor* p) {
        p->RLCA();
    };

    (*operations)[0x08] = [](Processor* p) {
        p->Store(p->SP, p->Get16BitImmediate());
    };

    (*operations)[0x09] = [](Processor* p) {
        p->ADDPairs(p->B, p->C, p->H, p->L);
    };

    (*operations)[0x0A] = [](Processor* p) {
        p->Load(p->A, p->B, p->C);
    };

    (*operations)[0x0B] = [](Processor* p) {
        p->DECPair(p->B, p->C);
    };

    (*operations)[0x0C] = [](Processor* p) {
        p->INC(p->C);
    };

    (*operations)[0x0D] = [](Processor* p) {
        p->DEC(p->C);
    };

    (*operations)[0x0E] = [](Processor* p) {

        p->Load(p->C, p->Get8BitImmediate());

    };

    (*operations)[0x0F] = [](Processor* p) {
        p->RRCA();
    };

    (*operations)[0x10] = [](Processor* p) {
        p->STOP();
    };

    (*operations)[0x11] = [](Processor* p) {

        p->Load(p->D, p->E, p->Get16BitImmediate());
    };

    (*operations)[0x12] = [](Processor* p) {
        uint16_t value = ((uint16_t)p->E->GetByte(0) << 8) | ((uint16_t)p->D->GetByte(0));
        p->Store(p->A, value);
    };

    (*operations)[0x13] = [](Processor* p) {
        p->INCPair(p->D, p->E);
    };

    (*operations)[0x14] = [](Processor* p) {
        p->INC(p->D);
    };

    (*operations)[0x15] = [](Processor* p) {
        p->DEC(p->D);
    };

    (*operations)[0x16] = [](Processor* p) {
        p->Load(p->D, p->Get8BitImmediate());
    };

    (*operations)[0x17] = [](Processor* p) {
        p->RLA();
    };

    (*operations)[0x18] = [](Processor* p) {
        p->JR((int8_t)p->Get8BitImmediate());
    };

    (*operations)[0x19] = [](Processor* p) {
        p->ADDPairs(p->H, p->L, p->D, p->E);
    };

    (*operations)[0x1A] = [](Processor* p) {

        p->Load(p->A, p->Get8BitImmediate());

    };

    (*operations)[0x1B] = [](Processor* p) {
        p->DECPair(p->D, p->E);
    };

    (*operations)[0x1C] = [](Processor* p) {
        p->INC(p->E);
    };

    (*operations)[0x1D] = [](Processor* p) {
        p->DEC(p->E);
    };

    (*operations)[0x1E] = [](Processor* p) {
        p->Load(p->E, p->Get8BitImmediate());
    };

    (*operations)[0x1F] = [](Processor* p) {
        p->RRA();
    };

    (*operations)[0x20] = [](Processor* p) {
		p->JR(0x00, (int8_t)p->Get8BitImmediate());
    };

    (*operations)[0x21] = [](Processor* p) {
        p->Load(p->H, p->L, p->Get16BitImmediate());
    };

    (*operations)[0x22] = [](Processor* p) {
        p->Store_Increment(p->A, p->H, p->L);
    };

    (*operations)[0x23] = [](Processor* p) {
        p->INCPair(p->H, p->L);
    };

    (*operations)[0x24] = [](Processor* p) {
        p->INC(p->H);
    };

    (*operations)[0x25] = [](Processor* p) {
        p->DEC(p->H);
    };

    (*operations)[0x26] = [](Processor* p) {
        p->Load(p->H, p->Get8BitImmediate());
    };

    (*operations)[0x27] = [](Processor* p) {
		p->DAA();
    };

    (*operations)[0x28] = [](Processor* p) {
		p->JR(0x01, (int8_t)p->Get8BitImmediate());
    };

    (*operations)[0x29] = [](Processor* p) {
        p->ADDPairs(p->H, p->L, p->H, p->L);
    };

    (*operations)[0x2A] = [](Processor* p) {
        p->Load_Increment(p->A, p->H, p->L);
    };

    (*operations)[0x2B] = [](Processor* p) {
        p->DECPair(p->H, p->L);
    };

    (*operations)[0x2C] = [](Processor* p) {
        p->INC(p->L);
    };

    (*operations)[0x2D] = [](Processor* p) {
        p->DEC(p->L);
    };

    (*operations)[0x2E] = [](Processor* p) {

        p->Load(p->L, p->Get8BitImmediate());

    };

    (*operations)[0x2F] = [](Processor* p) {
		p->CPL();
    };

    (*operations)[0x30] = [](Processor* p) {

		p->JR(0x02, (int8_t)p->Get8BitImmediate());
    };

    (*operations)[0x31] = [](Processor* p) {
        p->Load(p->SP, p->Get16BitImmediate());

    };

    (*operations)[0x32] = [](Processor* p) {

    };

    (*operations)[0x33] = [](Processor* p) {
		if (p->SP->GetWord(0) == 0xFFFF) {
			p->SP->SetWord(0, 0x0000);
		}
		else {
			p->SP->SetWord(0, p->SP->GetWord(0) + 0x01);
		}
		p->M->SetByte(0, 0x02);
		p->T->SetByte(0, 0x08);
    };

    (*operations)[0x34] = [](Processor* p) {
		p->INC(p->H, p->L);
    };

    (*operations)[0x35] = [](Processor* p) {
		p->DEC(p->H, p->L);
    };

    (*operations)[0x36] = [](Processor* p) {


    };

    (*operations)[0x37] = [](Processor* p) {
		p->SCF();
    };

    (*operations)[0x38] = [](Processor* p) {
		p->JR(0x03, (int8_t)p->Get8BitImmediate());
    };

    (*operations)[0x39] = [](Processor* p) {
		p->Trash8->SetByte(0, p->SP->GetByte(1));
		p->Trash8_2->SetByte(0, p->SP->GetByte(0));
		p->ADDPairs(p->H, p->L, p->Trash8, p->Trash8_2);
    };

    (*operations)[0x3A] = [](Processor* p) {
    };

    (*operations)[0x3B] = [](Processor* p) {
		if (p->SP->GetWord(0) == 0) {
			p->SP->SetWord(0, 0xFFFF);
		}
		else {
			p->SP->SetWord(0, p->SP->GetWord(0) - 0x01);
		}
		p->M->SetByte(0, 0x02);
		p->T->SetByte(0, 0x08);
    };

    (*operations)[0x3C] = [](Processor* p) {
		p->INC(p->A);
    };

    (*operations)[0x3D] = [](Processor* p) {
		p->DEC(p->A);
    };

    (*operations)[0x3E] = [](Processor* p) {

    };

    (*operations)[0x3F] = [](Processor* p) {
		p->CCF();
    };

//--------------------------------------------------------------

	(*operations)[0x40] = [](Processor* p) {
		p->Load(p->B, p->B);
	};

	(*operations)[0x41] = [](Processor* p) {
		p->Load(p->B, p->C);
	};

	(*operations)[0x42] = [](Processor* p) {
		p->Load(p->B, p->D);
	};

	(*operations)[0x43] = [](Processor* p) {
		p->Load(p->B, p->E);
	};

	(*operations)[0x44] = [](Processor* p) {
		p->Load(p->B, p->H);
	};

	(*operations)[0x45] = [](Processor* p) {
		p->Load(p->B, p->L);
	};

	(*operations)[0x46] = [](Processor* p) {
		p->Load(p->B, p->H, p->L);
	};

	(*operations)[0x47] = [](Processor* p) {
		p->Load(p->B, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x48] = [](Processor* p) {
		p->Load(p->C, p->B);
	};

	(*operations)[0x49] = [](Processor* p) {
		p->Load(p->C, p->C);
	};

	(*operations)[0x4A] = [](Processor* p) {
		p->Load(p->C, p->D);
	};

	(*operations)[0x4B] = [](Processor* p) {
		p->Load(p->C, p->E);
	};

	(*operations)[0x4C] = [](Processor* p) {
		p->Load(p->C, p->H);
	};

	(*operations)[0x4D] = [](Processor* p) {
		p->Load(p->C, p->L);
	};

	(*operations)[0x4E] = [](Processor* p) {
		p->Load(p->C, p->H, p->L);
	};

	(*operations)[0x4F] = [](Processor* p) {
		p->Load(p->C, p->A);
	};

//----------------------------------------------------------

	(*operations)[0x50] = [](Processor* p) {
		p->Load(p->D, p->B);
	};

	(*operations)[0x51] = [](Processor* p) {
		p->Load(p->D, p->C);
	};

	(*operations)[0x52] = [](Processor* p) {
		p->Load(p->D, p->D);
	};

	(*operations)[0x53] = [](Processor* p) {
		p->Load(p->D, p->E);
	};

	(*operations)[0x54] = [](Processor* p) {
		p->Load(p->D, p->H);
	};

	(*operations)[0x55] = [](Processor* p) {
		p->Load(p->D, p->L);
	};

	(*operations)[0x56] = [](Processor* p) {
		p->Load(p->D, p->H, p->L);
	};

	(*operations)[0x57] = [](Processor* p) {
		p->Load(p->D, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x58] = [](Processor* p) {
		p->Load(p->E, p->B);
	};

	(*operations)[0x59] = [](Processor* p) {
		p->Load(p->E, p->C);
	};

	(*operations)[0x5A] = [](Processor* p) {
		p->Load(p->E, p->D);
	};

	(*operations)[0x5B] = [](Processor* p) {
		p->Load(p->E, p->E);
	};

	(*operations)[0x5C] = [](Processor* p) {
		p->Load(p->E, p->H);
	};

	(*operations)[0x5D] = [](Processor* p) {
		p->Load(p->E, p->L);
	};

	(*operations)[0x5E] = [](Processor* p) {
		p->Load(p->E, p->H, p->L);
	};

	(*operations)[0x5F] = [](Processor* p) {
		p->Load(p->E, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x60] = [](Processor* p) {
		p->Load(p->H, p->B);
	};

	(*operations)[0x61] = [](Processor* p) {
		p->Load(p->H, p->C);
	};

	(*operations)[0x62] = [](Processor* p) {
		p->Load(p->H, p->D);
	};

	(*operations)[0x63] = [](Processor* p) {
		p->Load(p->H, p->E);
	};

	(*operations)[0x64] = [](Processor* p) {
		p->Load(p->H, p->H);
	};

	(*operations)[0x65] = [](Processor* p) {
		p->Load(p->H, p->L);
	};

	(*operations)[0x66] = [](Processor* p) {
		p->Load(p->H, p->H, p->L);
	};

	(*operations)[0x67] = [](Processor* p) {
		p->Load(p->H, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x68] = [](Processor* p) {
		p->Load(p->L, p->B);
	};

	(*operations)[0x69] = [](Processor* p) {
		p->Load(p->L, p->C);
	};

	(*operations)[0x6A] = [](Processor* p) {
		p->Load(p->L, p->D);
	};

	(*operations)[0x6B] = [](Processor* p) {
		p->Load(p->L, p->E);
	};

	(*operations)[0x6C] = [](Processor* p) {
		p->Load(p->L, p->H);
	};

	(*operations)[0x6D] = [](Processor* p) {
		p->Load(p->L, p->L);
	};

	(*operations)[0x6E] = [](Processor* p) {
		p->Load(p->L, p->H, p->L);
	};

	(*operations)[0x6F] = [](Processor* p) {
		p->Load(p->L, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x70] = [](Processor* p) {
		p->Store(p->B, p->H, p->L);
	};

	(*operations)[0x71] = [](Processor* p) {
		p->Store(p->C, p->H, p->L);
	};

	(*operations)[0x72] = [](Processor* p) {
		p->Store(p->D, p->H, p->L);
	};

	(*operations)[0x73] = [](Processor* p) {
		p->Store(p->E, p->H, p->L);
	};

	(*operations)[0x74] = [](Processor* p) {
		p->Store(p->H, p->H, p->L);
	};

	(*operations)[0x75] = [](Processor* p) {
		p->Store(p->L, p->H, p->L);
	};

	(*operations)[0x76] = [](Processor* p) {
		p->HALT();
	};

	(*operations)[0x77] = [](Processor* p) {
		p->Store(p->A, p->H, p->L);
	};
//-----------------------------------------------------------

	(*operations)[0x78] = [](Processor* p) {
		p->Load(p->A, p->B);
	};

	(*operations)[0x79] = [](Processor* p) {
		p->Load(p->A, p->C);
	};

	(*operations)[0x7A] = [](Processor* p) {
		p->Load(p->A, p->D);
	};

	(*operations)[0x7B] = [](Processor* p) {
		p->Load(p->A, p->E);
	};

	(*operations)[0x7C] = [](Processor* p) {
		p->Load(p->A, p->H);
	};

	(*operations)[0x7D] = [](Processor* p) {
		p->Load(p->A, p->L);
	};

	(*operations)[0x7E] = [](Processor* p) {
		p->Load(p->A, p->H, p->L);
	};

	(*operations)[0x7F] = [](Processor* p) {
		p->Load(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x80] = [](Processor* p) {
		p->ADD(p->A, p->B);
	};

	(*operations)[0x81] = [](Processor* p) {
		p->ADD(p->A, p->C);
	};

	(*operations)[0x82] = [](Processor* p) {
		p->ADD(p->A, p->D);
	};

	(*operations)[0x83] = [](Processor* p) {
		p->ADD(p->A, p->E);
	};

	(*operations)[0x84] = [](Processor* p) {
		p->ADD(p->A, p->H);
	};

	(*operations)[0x85] = [](Processor* p) {
		p->ADD(p->A, p->L);
	};

	(*operations)[0x86] = [](Processor* p) {
		p->ADD(p->A, p->H, p->L);
	};

	(*operations)[0x87] = [](Processor* p) {
		p->ADD(p->A, p->A);
	};

//---------------------------------------------------------------------

	(*operations)[0x88] = [](Processor* p) {
		p->ADC(p->A, p->B);
	};

	(*operations)[0x89] = [](Processor* p) {
		p->ADC(p->A, p->C);
	};

	(*operations)[0x8A] = [](Processor* p) {
		p->ADC(p->A, p->D);
	};

	(*operations)[0x8B] = [](Processor* p) {
		p->ADC(p->A, p->E);
	};

	(*operations)[0x8C] = [](Processor* p) {
		p->ADC(p->A, p->H);
	};

	(*operations)[0x8D] = [](Processor* p) {
		p->ADC(p->A, p->L);
	};

	(*operations)[0x8E] = [](Processor* p) {
		p->ADC(p->A, p->H, p->L);
	};

	(*operations)[0x8F] = [](Processor* p) {
		p->ADC(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0x90] = [](Processor* p) {
		p->SUB(p->A, p->B);
	};

	(*operations)[0x91] = [](Processor* p) {
		p->SUB(p->A, p->C);
	};

	(*operations)[0x92] = [](Processor* p) {
		p->SUB(p->A, p->D);
	};

	(*operations)[0x93] = [](Processor* p) {
		p->SUB(p->A, p->E);
	};

	(*operations)[0x94] = [](Processor* p) {
		p->SUB(p->A, p->H);
	};

	(*operations)[0x95] = [](Processor* p) {
		p->SUB(p->A, p->L);
	};

	(*operations)[0x96] = [](Processor* p) {
		p->SUB(p->A, p->H, p->L);
	};

	(*operations)[0x97] = [](Processor* p) {
		p->SUB(p->A, p->A);
	};

//-------------------------------------------------------------

	(*operations)[0x98] = [](Processor* p) {
		p->SBC(p->A, p->B);
	};

	(*operations)[0x99] = [](Processor* p) {
		p->SBC(p->A, p->C);
	};

	(*operations)[0x9A] = [](Processor* p) {
		p->SBC(p->A, p->D);
	};

	(*operations)[0x9B] = [](Processor* p) {
		p->SBC(p->A, p->E);
	};

	(*operations)[0x9C] = [](Processor* p) {
		p->SBC(p->A, p->H);
	};

	(*operations)[0x9D] = [](Processor* p) {
		p->SBC(p->A, p->L);
	};

	(*operations)[0x9E] = [](Processor* p) {
		p->SBC(p->A, p->H, p->L);
	};

	(*operations)[0x9F] = [](Processor* p) {
		p->SBC(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0xA0] = [](Processor* p) {
		p->AND(p->A, p->B);
	};

	(*operations)[0xA1] = [](Processor* p) {
		p->AND(p->A, p->C);
	};

	(*operations)[0xA2] = [](Processor* p) {
		p->AND(p->A, p->C);
	};

	(*operations)[0xA3] = [](Processor* p) {
		p->AND(p->A, p->D);
	};

	(*operations)[0xA4] = [](Processor* p) {
		p->AND(p->A, p->E);
	};

	(*operations)[0xA5] = [](Processor* p) {
		p->AND(p->A, p->H);
	};

	(*operations)[0xA6] = [](Processor* p) {
		p->AND(p->A, p->L);
	};

	(*operations)[0xA7] = [](Processor* p) {
		p->AND(p->A, p->H, p->L);
	};

	(*operations)[0xA8] = [](Processor* p) {
		p->AND(p->A, p->A);
	};

//-----------------------------------------------------------------

	(*operations)[0xA8] = [](Processor* p) {
		p->XOR(p->A, p->B);
	};

	(*operations)[0xA9] = [](Processor* p) {
		p->XOR(p->A, p->C);
	};

	(*operations)[0xAA] = [](Processor* p) {
		p->XOR(p->A, p->D);
	};

	(*operations)[0xAB] = [](Processor* p) {
		p->XOR(p->A, p->E);
	};

	(*operations)[0xAC] = [](Processor* p) {
		p->XOR(p->A, p->H);
	};

	(*operations)[0xAD] = [](Processor* p) {
		p->XOR(p->A, p->L);
	};

	(*operations)[0xAE] = [](Processor* p) {
		p->XOR(p->A, p->H, p->L);
	};

	(*operations)[0xAF] = [](Processor* p) {
		p->XOR(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0xB0] = [](Processor* p) {
		p->OR(p->A, p->B);
	};

	(*operations)[0xB1] = [](Processor* p) {
		p->OR(p->A, p->C);
	};

	(*operations)[0xB2] = [](Processor* p) {
		p->OR(p->A, p->D);
	};

	(*operations)[0xB3] = [](Processor* p) {
		p->OR(p->A, p->E);
	};

	(*operations)[0xB4] = [](Processor* p) {
		p->OR(p->A, p->H);
	};

	(*operations)[0xB5] = [](Processor* p) {
		p->OR(p->A, p->L);
	};

	(*operations)[0xB6] = [](Processor* p) {
		p->OR(p->A, p->H, p->L);
	};

	(*operations)[0xB7] = [](Processor* p) {
		p->OR(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0xB8] = [](Processor* p) {
		p->CP(p->A, p->B);
	};

	(*operations)[0xB9] = [](Processor* p) {
		p->CP(p->A, p->C);
	};

	(*operations)[0xBA] = [](Processor* p) {
		p->CP(p->A, p->D);
	};

	(*operations)[0xBB] = [](Processor* p) {
		p->CP(p->A, p->E);
	};

	(*operations)[0xBC] = [](Processor* p) {
		p->CP(p->A, p->H);
	};

	(*operations)[0xBD] = [](Processor* p) {
		p->CP(p->A, p->L);
	};

	(*operations)[0xBE] = [](Processor* p) {
		p->CP(p->A, p->H, p->L);
	};

	(*operations)[0xBF] = [](Processor* p) {
		p->CP(p->A, p->A);
	};

//-----------------------------------------------------------

	(*operations)[0xC0] = [](Processor* p) {

	};

	(*operations)[0xC1] = [](Processor* p) {
		p->RET(0x00);
	};

	(*operations)[0xC1] = [](Processor* p) {
		p->Pop(p->B, p->C);
	};

	(*operations)[0xC2] = [](Processor* p) {
		p->JP(0x00, p->Get16BitImmediate());
	};

	(*operations)[0xC3] = [](Processor* p) {
		p->JP(p->Get16BitImmediate());
	};

	(*operations)[0xC4] = [](Processor* p) {
		p->CALL(0x00, p->Get16BitImmediate());
	};

	(*operations)[0xC5] = [](Processor* p) {
		p->Push(p->B, p->C);
	};

	(*operations)[0xC6] = [](Processor* p) {
		p->ADD(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xC7] = [](Processor* p) {
		p->RST(0x00);
	};

	(*operations)[0xC8] = [](Processor* p) {
		p->RET(0x01);
	};

	(*operations)[0xC9] = [](Processor* p) {
		p->RET();
	};

	(*operations)[0xCA] = [](Processor* p) {
		p->JP(0x01, p->Get16BitImmediate());
	};

	(*operations)[0xCB] = [](Processor* p) {
		/*
			EXT OPS
		*/
	};

	(*operations)[0xCC] = [](Processor* p) {
		p->CALL(0x01, p->Get16BitImmediate());
	};

	(*operations)[0xCD] = [](Processor* p) {
		p->CALL(p->Get16BitImmediate());
	};

	(*operations)[0xCE] = [](Processor* p) {
		p->ADC(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xCF] = [](Processor* p) {
		p->RST(0x01);
	};

//-----------------------------------------------------------

	(*operations)[0xD0] = [](Processor* p) {
		p->RET(0x02);
	};

	(*operations)[0xD1] = [](Processor* p) {
		p->Pop(p->D, p->E);
	};

	(*operations)[0xD2] = [](Processor* p) {
		p->JP(0x02, p->Get16BitImmediate());
	};

	(*operations)[0xD3] = [](Processor* p) {
		uint16_t value = p->PC->GetWord(0);
		p->JP(p->memory->GetWord(value));
		p->PC->SetWord(0, value + 2);
	};

	(*operations)[0xD4] = [](Processor* p) {
		p->CALL(0x02, p->Get16BitImmediate());
	};

	(*operations)[0xD5] = [](Processor* p) {
		p->Push(p->D, p->E);
	};

	(*operations)[0xD6] = [](Processor* p) {
		p->SUB(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xD7] = [](Processor* p) {
		p->RST(0x02);
	};

	(*operations)[0xD8] = [](Processor* p) {
		p->RET(0x03);
	};

	(*operations)[0xD9] = [](Processor* p) {
		p->RETI();
	};

	(*operations)[0xDA] = [](Processor* p) {
		p->JP(0x01, p->Get16BitImmediate());
	};

	(*operations)[0xDB] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xDC] = [](Processor* p) {
		p->CALL(0x03, p->Get16BitImmediate());
	};

	(*operations)[0xDD] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xDE] = [](Processor* p) {
		p->SBC(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xDF] = [](Processor* p) {
		p->RST(0x03);
	};

//---------------------------------------------------------------------

	(*operations)[0xE0] = [](Processor* p) {
		//LDH (n), A
	};

	(*operations)[0xE1] = [](Processor* p) {
		p->Pop(p->H, p->L);
	};

	(*operations)[0xE2] = [](Processor* p) {
		//LDH (C), A
	};

	(*operations)[0xE3] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xE4] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xE5] = [](Processor* p) {
		p->Push(p->H, p->L);
	};

	(*operations)[0xE6] = [](Processor* p) {
		p->AND(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xE7] = [](Processor* p) {
		p->RST(0x04);
	};

	(*operations)[0xE8] = [](Processor* p) {
		p->ADDSP(p->Get8BitImmediate());
	};

	(*operations)[0xE9] = [](Processor* p) {
		p->JP(p->H, p->L);
	};

	(*operations)[0xEA] = [](Processor* p) {
		//LD (nn), A
	};

	(*operations)[0xEB] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xEC] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xED] = [](Processor* p) {
		//XXX
	};

	(*operations)[0xEE] = [](Processor* p) {
		p->XOR(p->A, p->Get8BitImmediate());
	};

	(*operations)[0xEF] = [](Processor* p) {
		p->RST(0x05);
	};

//---------------------------------------------------------------------

	(*operations)[0xF0] = [](Processor* p) {
		//LDH A,(n)
	};
	(*operations)[0xF1] = [](Processor* p) {
		p->Pop(p->A, p->F);
	};
	(*operations)[0xF2] = [](Processor* p) {
		//XXX
	};
	(*operations)[0xF3] = [](Processor* p) {
		p->DI();
	};
	(*operations)[0xF4] = [](Processor* p) {
		//XXX
	};
	(*operations)[0xF5] = [](Processor* p) {
		p->Push(p->A, p->F);
	};
	(*operations)[0xF6] = [](Processor* p) {
		p->OR(p->A, p->Get8BitImmediate());
	};
	(*operations)[0xF7] = [](Processor* p) {
		p->RST(0x06);
	};
	(*operations)[0xF8] = [](Processor* p) {
		//LDHL SP, d
	};
	(*operations)[0xF9] = [](Processor* p) {
		//LD SP, HL
	};
	(*operations)[0xFA] = [](Processor* p) {
		//LD A, (nn)
	};
	(*operations)[0xFB] = [](Processor* p) {
		p->EI();
	};
	(*operations)[0xFC] = [](Processor* p) {
		//XXX
	};
	(*operations)[0xFD] = [](Processor* p) {
		//XXX
	};
	(*operations)[0xFE] = [](Processor* p) {
		p->CP(p->A, p->Get8BitImmediate());
	};
	(*operations)[0xFF] = [](Processor* p) {
		p->RST(0x07);
	};

    cboperations = new std::unordered_map<uint8_t, std::function<void(Processor* p)>>();
    (*cboperations)[0x00] = [](Processor* p) {
        p->RLC(p->B);
    };
    (*cboperations)[0x01] = [](Processor* p) {
        p->RLC(p->C);
    };
    (*cboperations)[0x02] = [](Processor* p) {
        p->RLC(p->D);
    };
    (*cboperations)[0x03] = [](Processor* p) {
        p->RLC(p->E);
    };
    (*cboperations)[0x04] = [](Processor* p) {
        p->RLC(p->H);
    };
    (*cboperations)[0x05] = [](Processor* p) {
        p->RLC(p->L);
    };
    (*cboperations)[0x06] = [](Processor* p) {
        p->RLC(p->H, p->L);
    };
    (*cboperations)[0x07] = [](Processor* p) {
        p->RLC(p->A);
    };
    (*cboperations)[0x08] = [](Processor* p) {
        p->RRC(p->B);
    };
    (*cboperations)[0x09] = [](Processor* p) {
        p->RRC(p->C);
    };
    (*cboperations)[0x0A] = [](Processor* p) {
        p->RRC(p->D);
    };
    (*cboperations)[0x0B] = [](Processor* p) {
        p->RRC(p->D);
    };
    (*cboperations)[0x0C] = [](Processor* p) {
        p->RRC(p->H);
    };
    (*cboperations)[0x0D] = [](Processor* p) {
        p->RRC(p->L);
    };
    (*cboperations)[0x0E] = [](Processor* p) {
        p->RRC(p->H, p->L);
    };
    (*cboperations)[0x0F] = [](Processor* p) {
        p->RRC(p->A);
    };
    (*cboperations)[0x10] = [](Processor* p) {
        p->RL(p->B);
    };
    (*cboperations)[0x11] = [](Processor* p) {
        p->RL(p->C);
    };
    (*cboperations)[0x12] = [](Processor* p) {
        p->RL(p->D);
    };
    (*cboperations)[0x13] = [](Processor* p) {
        p->RL(p->E);
    };
    (*cboperations)[0x14] = [](Processor* p) {
        p->RL(p->H);
    };
    (*cboperations)[0x15] = [](Processor* p) {
        p->RL(p->L);
    };
    (*cboperations)[0x16] = [](Processor* p) {
        p->RL(p->H, p->L);
    };
    (*cboperations)[0x17] = [](Processor* p) {
        p->RL(p->A);
    };
    (*cboperations)[0x18] = [](Processor* p) {
        p->RR(p->B);
    };
    (*cboperations)[0x19] = [](Processor* p) {
        p->RR(p->C);
    };
    (*cboperations)[0x1A] = [](Processor* p) {
        p->RR(p->D);
    };
    (*cboperations)[0x1B] = [](Processor* p) {
        p->RR(p->E);
    };
    (*cboperations)[0x1C] = [](Processor* p) {
        p->RR(p->H);
    };
    (*cboperations)[0x1D] = [](Processor* p) {
        p->RR(p->L);
    };
    (*cboperations)[0x1E] = [](Processor* p) {
        p->RR(p->H, p->L);
    };
    (*cboperations)[0x1F] = [](Processor* p) {
        p->RR(p->A);
    };
    (*cboperations)[0x20] = [](Processor* p) {
        p->SLA(p->B);
    };
    (*cboperations)[0x21] = [](Processor* p) {
        p->SLA(p->C);
    };
    (*cboperations)[0x22] = [](Processor* p) {
        p->SLA(p->D);
    };
    (*cboperations)[0x23] = [](Processor* p) {
        p->SLA(p->E);
    };
    (*cboperations)[0x24] = [](Processor* p) {
        p->SLA(p->H);
    };
    (*cboperations)[0x25] = [](Processor* p) {
        p->SLA(p->L);
    };
    (*cboperations)[0x26] = [](Processor* p) {
        p->SLA(p->H, p->L);
    };
    (*cboperations)[0x27] = [](Processor* p) {
        p->SLA(p->A);
    };
    (*cboperations)[0x28] = [](Processor* p) {
        p->SRA(p->B);
    };
    (*cboperations)[0x29] = [](Processor* p) {
        p->SRA(p->C);
    };
    (*cboperations)[0x2A] = [](Processor* p) {
        p->SRA(p->D);
    };
    (*cboperations)[0x2B] = [](Processor* p) {
        p->SRA(p->E);
    };
    (*cboperations)[0x2C] = [](Processor* p) {
        p->SRA(p->H);
    };
    (*cboperations)[0x2D] = [](Processor* p) {
        p->SRA(p->L);
    };
    (*cboperations)[0x2E] = [](Processor* p) {
        p->SRA(p->H, p->L);
    };
    (*cboperations)[0x2F] = [](Processor* p) {
        p->SRA(p->A);
    };
    (*cboperations)[0x30] = [](Processor* p) {
        p->SWAP(p->B);
    };
    (*cboperations)[0x31] = [](Processor* p) {
        p->SWAP(p->C);
    };
    (*cboperations)[0x32] = [](Processor* p) {
        p->SWAP(p->D);
    };
    (*cboperations)[0x33] = [](Processor* p) {
        p->SWAP(p->E);
    };
    (*cboperations)[0x34] = [](Processor* p) {
        p->SWAP(p->H);
    };
    (*cboperations)[0x35] = [](Processor* p) {
        p->SWAP(p->L);
    };
    (*cboperations)[0x36] = [](Processor* p) {
        p->SWAP(p->H, p->L);
    };
    (*cboperations)[0x37] = [](Processor* p) {
        p->SWAP(p->A);
    };
    (*cboperations)[0x38] = [](Processor* p) {
        p->SRL(p->B);
    };
    (*cboperations)[0x39] = [](Processor* p) {
        p->SRL(p->C);
    };
    (*cboperations)[0x3A] = [](Processor* p) {
        p->SRL(p->D);
    };
    (*cboperations)[0x3B] = [](Processor* p) {
        p->SRL(p->E);
    };
    (*cboperations)[0x3C] = [](Processor* p) {
        p->SRL(p->H);
    };
    (*cboperations)[0x3D] = [](Processor* p) {
        p->SRL(p->L);
    };
    (*cboperations)[0x3E] = [](Processor* p) {
        p->SRL(p->H, p->L);
    };
    (*cboperations)[0x3F] = [](Processor* p) {
        p->SRL(p->A);
    };
    (*cboperations)[0x40] = [](Processor* p) {
        p->BIT(0, p->B);
    };
    (*cboperations)[0x41] = [](Processor* p) {
        p->BIT(0, p->C);
    };
    (*cboperations)[0x42] = [](Processor* p) {
        p->BIT(0, p->D);
    };
    (*cboperations)[0x43] = [](Processor* p) {
        p->BIT(0, p->E);
    };
    (*cboperations)[0x44] = [](Processor* p) {
        p->BIT(0, p->H);
    };
    (*cboperations)[0x45] = [](Processor* p) {
        p->BIT(0, p->L);
    };
    (*cboperations)[0x46] = [](Processor* p) {
        p->BIT(0, p->H, p->L);
    };
    (*cboperations)[0x47] = [](Processor* p) {
        p->BIT(0, p->A);
    };
    (*cboperations)[0x48] = [](Processor* p) {
        p->BIT(1, p->B);
    };
    (*cboperations)[0x49] = [](Processor* p) {
        p->BIT(1, p->C);
    };
    (*cboperations)[0x4A] = [](Processor* p) {
        p->BIT(1, p->D);
    };
    (*cboperations)[0x4B] = [](Processor* p) {
        p->BIT(1, p->E);
    };
    (*cboperations)[0x4C] = [](Processor* p) {
        p->BIT(1, p->H);
    };
    (*cboperations)[0x4D] = [](Processor* p) {
        p->BIT(1, p->L);
    };
    (*cboperations)[0x4E] = [](Processor* p) {
        p->BIT(1, p->H, p->L);
    };
    (*cboperations)[0x4F] = [](Processor* p) {
        p->BIT(1, p->A);
    };
    (*cboperations)[0x50] = [](Processor* p) {
        p->BIT(2, p->B);
    };
    (*cboperations)[0x51] = [](Processor* p) {
        p->BIT(2, p->C);
    };
    (*cboperations)[0x52] = [](Processor* p) {
        p->BIT(2, p->D);
    };
    (*cboperations)[0x53] = [](Processor* p) {
        p->BIT(2, p->E);
    };
    (*cboperations)[0x54] = [](Processor* p) {
        p->BIT(2, p->H);
    };
    (*cboperations)[0x55] = [](Processor* p) {
        p->BIT(2, p->L);
    };
    (*cboperations)[0x56] = [](Processor* p) {
        p->BIT(2, p->H, p->L);
    };
    (*cboperations)[0x57] = [](Processor* p) {
        p->BIT(2, p->A);
    };
    (*cboperations)[0x58] = [](Processor* p) {
        p->BIT(3, p->B);
    };
    (*cboperations)[0x59] = [](Processor* p) {
        p->BIT(3, p->C);
    };
    (*cboperations)[0x5A] = [](Processor* p) {
        p->BIT(3, p->D);
    };
    (*cboperations)[0x5B] = [](Processor* p) {
        p->BIT(3, p->E);
    };
    (*cboperations)[0x5C] = [](Processor* p) {
        p->BIT(3, p->H);
    };
    (*cboperations)[0x5D] = [](Processor* p) {
        p->BIT(3, p->L);
    };
    (*cboperations)[0x5E] = [](Processor* p) {
        p->BIT(3, p->H, p->L);
    };
    (*cboperations)[0x5F] = [](Processor* p) {
        p->BIT(3, p->A);
    };

    (*cboperations)[0x60] = [](Processor* p) {
        p->BIT(4, p->B);
    };
    (*cboperations)[0x61] = [](Processor* p) {
        p->BIT(4, p->C);
    };
    (*cboperations)[0x62] = [](Processor* p) {
        p->BIT(4, p->D);
    };
    (*cboperations)[0x63] = [](Processor* p) {
        p->BIT(4, p->E);
    };
    (*cboperations)[0x64] = [](Processor* p) {
        p->BIT(4, p->H);
    };
    (*cboperations)[0x65] = [](Processor* p) {
        p->BIT(4, p->L);
    };
    (*cboperations)[0x66] = [](Processor* p) {
        p->BIT(4, p->H, p->L);
    };
    (*cboperations)[0x67] = [](Processor* p) {
        p->BIT(4, p->A);
    };
    (*cboperations)[0x68] = [](Processor* p) {
        p->BIT(5, p->B);
    };
    (*cboperations)[0x69] = [](Processor* p) {
        p->BIT(5, p->C);
    };
    (*cboperations)[0x6A] = [](Processor* p) {
        p->BIT(5, p->D);
    };
    (*cboperations)[0x6B] = [](Processor* p) {
        p->BIT(5, p->E);
    };
    (*cboperations)[0x6C] = [](Processor* p) {
        p->BIT(5, p->H);
    };
    (*cboperations)[0x6D] = [](Processor* p) {
        p->BIT(5, p->L);
    };
    (*cboperations)[0x6E] = [](Processor* p) {
        p->BIT(5, p->H, p->L);
    };
    (*cboperations)[0x6F] = [](Processor* p) {
        p->BIT(5, p->A);
    };
    (*cboperations)[0x70] = [](Processor* p) {
        p->BIT(6, p->B);
    };
    (*cboperations)[0x71] = [](Processor* p) {
        p->BIT(6, p->C);
    };
    (*cboperations)[0x72] = [](Processor* p) {
        p->BIT(6, p->D);
    };
    (*cboperations)[0x73] = [](Processor* p) {
        p->BIT(6, p->E);
    };
    (*cboperations)[0x74] = [](Processor* p) {
        p->BIT(6, p->H);
    };
    (*cboperations)[0x75] = [](Processor* p) {
        p->BIT(6, p->L);
    };
    (*cboperations)[0x76] = [](Processor* p) {
        p->BIT(6, p->H, p->L);
    };
    (*cboperations)[0x77] = [](Processor* p) {
        p->BIT(6, p->A);
    };
    (*cboperations)[0x78] = [](Processor* p) {
        p->BIT(7, p->B);
    };
    (*cboperations)[0x79] = [](Processor* p) {
        p->BIT(7, p->C);
    };
    (*cboperations)[0x7A] = [](Processor* p) {
        p->BIT(7, p->D);
    };
    (*cboperations)[0x7B] = [](Processor* p) {
        p->BIT(7, p->E);
    };
    (*cboperations)[0x7C] = [](Processor* p) {
        p->BIT(7, p->H);
    };
    (*cboperations)[0x7D] = [](Processor* p) {
        p->BIT(7, p->L);
    };
    (*cboperations)[0x7E] = [](Processor* p) {
        p->BIT(7, p->H, p->L);
    };
    (*cboperations)[0x7F] = [](Processor* p) {
        p->BIT(7, p->A);
    };
    (*cboperations)[0x80] = [](Processor* p) {
        p->RES(0, p->B);
    };
    (*cboperations)[0x81] = [](Processor* p) {
        p->RES(0, p->C);
    };
    (*cboperations)[0x82] = [](Processor* p) {
        p->RES(0, p->D);
    };
    (*cboperations)[0x83] = [](Processor* p) {
        p->RES(0, p->E);
    };
    (*cboperations)[0x84] = [](Processor* p) {
        p->RES(0, p->H);
    };
    (*cboperations)[0x85] = [](Processor* p) {
        p->RES(0, p->L);
    };
    (*cboperations)[0x86] = [](Processor* p) {
        p->RES(0, p->H, p->L);
    };
    (*cboperations)[0x87] = [](Processor* p) {
        p->RES(0, p->A);
    };
    (*cboperations)[0x88] = [](Processor* p) {
        p->RES(1, p->B);
    };
    (*cboperations)[0x89] = [](Processor* p) {
        p->RES(1, p->C);
    };
    (*cboperations)[0x8A] = [](Processor* p) {
        p->RES(1, p->D);
    };
    (*cboperations)[0x8B] = [](Processor* p) {
        p->RES(1, p->E);
    };
    (*cboperations)[0x8C] = [](Processor* p) {
        p->RES(1, p->H);
    };
    (*cboperations)[0x8D] = [](Processor* p) {
        p->RES(1, p->L);
    };
    (*cboperations)[0x8E] = [](Processor* p) {
        p->RES(1, p->H, p->L);
    };
    (*cboperations)[0x8F] = [](Processor* p) {
        p->RES(1, p->A);
    };
    (*cboperations)[0x90] = [](Processor* p) {
        p->RES(2, p->B);
    };
    (*cboperations)[0x91] = [](Processor* p) {
        p->RES(2, p->C);
    };
    (*cboperations)[0x92] = [](Processor* p) {
        p->RES(2, p->D);
    };
    (*cboperations)[0x93] = [](Processor* p) {
        p->RES(2, p->E);
    };
    (*cboperations)[0x94] = [](Processor* p) {
        p->RES(2, p->H);
    };
    (*cboperations)[0x95] = [](Processor* p) {
        p->RES(2, p->L);
    };
    (*cboperations)[0x96] = [](Processor* p) {
        p->RES(2, p->H, p->L);
    };
    (*cboperations)[0x97] = [](Processor* p) {
        p->RES(2, p->A);
    };
    (*cboperations)[0x98] = [](Processor* p) {
        p->RES(3, p->B);
    };
    (*cboperations)[0x99] = [](Processor* p) {
        p->RES(3, p->C);
    };
    (*cboperations)[0x9A] = [](Processor* p) {
        p->RES(3, p->D);
    };
    (*cboperations)[0x9B] = [](Processor* p) {
        p->RES(3, p->E);
    };
    (*cboperations)[0x9C] = [](Processor* p) {
        p->RES(3, p->H);
    };
    (*cboperations)[0x9D] = [](Processor* p) {
        p->RES(3, p->L);
    };
    (*cboperations)[0x9E] = [](Processor* p) {
        p->RES(3, p->H, p->L);
    };
    (*cboperations)[0x9F] = [](Processor* p) {
        p->RES(3, p->A);
    };
    (*cboperations)[0xA0] = [](Processor* p) {
        p->RES(4, p->B);
    };
    (*cboperations)[0xA1] = [](Processor* p) {
        p->RES(4, p->C);
    };
    (*cboperations)[0xA2] = [](Processor* p) {
        p->RES(4, p->D);
    };
    (*cboperations)[0xA3] = [](Processor* p) {
        p->RES(4, p->E);
    };
    (*cboperations)[0xA4] = [](Processor* p) {
        p->RES(4, p->H);
    };
    (*cboperations)[0xA5] = [](Processor* p) {
        p->RES(4, p->L);
    };
    (*cboperations)[0xA6] = [](Processor* p) {
        p->RES(4, p->H, p->L);
    };
    (*cboperations)[0xA7] = [](Processor* p) {
        p->RES(4, p->A);
    };
    (*cboperations)[0xA8] = [](Processor* p) {
        p->RES(5, p->B);
    };
    (*cboperations)[0xA9] = [](Processor* p) {
        p->RES(5, p->C);
    };
    (*cboperations)[0xAA] = [](Processor* p) {
        p->RES(5, p->D);
    };
    (*cboperations)[0xAB] = [](Processor* p) {
        p->RES(5, p->E);
    };
    (*cboperations)[0xAC] = [](Processor* p) {
        p->RES(5, p->H);
    };
    (*cboperations)[0xAD] = [](Processor* p) {
        p->RES(5, p->L);
    };
    (*cboperations)[0xAE] = [](Processor* p) {
        p->RES(5, p->H, p->L);
    };
    (*cboperations)[0xAF] = [](Processor* p) {
        p->RES(5, p->A);
    };
    (*cboperations)[0xB0] = [](Processor* p) {
        p->RES(6, p->B);
    };
    (*cboperations)[0xB1] = [](Processor* p) {
        p->RES(6, p->C);
    };
    (*cboperations)[0xB2] = [](Processor* p) {
        p->RES(6, p->D);
    };
    (*cboperations)[0xB3] = [](Processor* p) {
        p->RES(6, p->E);
    };
    (*cboperations)[0xB4] = [](Processor* p) {
        p->RES(6, p->H);
    };
    (*cboperations)[0xB5] = [](Processor* p) {
        p->RES(6, p->L);
    };
    (*cboperations)[0xB6] = [](Processor* p) {
        p->RES(6, p->H, p->L);
    };
    (*cboperations)[0xB7] = [](Processor* p) {
        p->RES(6, p->A);
    };
    (*cboperations)[0xB8] = [](Processor* p) {
        p->RES(7, p->B);
    };
    (*cboperations)[0xB9] = [](Processor* p) {
        p->RES(7, p->C);
    };
    (*cboperations)[0xBA] = [](Processor* p) {
        p->RES(7, p->D);
    };
    (*cboperations)[0xBB] = [](Processor* p) {
        p->RES(7, p->E);
    };
    (*cboperations)[0xBC] = [](Processor* p) {
        p->RES(7, p->H);
    };
    (*cboperations)[0xBD] = [](Processor* p) {
        p->RES(7, p->L);
    };
    (*cboperations)[0xBE] = [](Processor* p) {
        p->RES(7, p->H, p->L);
    };
    (*cboperations)[0xBF] = [](Processor* p) {
        p->RES(7, p->A);
    };
    (*cboperations)[0xC0] = [](Processor* p) {
        p->SET(0, p->B);
    };
    (*cboperations)[0xC1] = [](Processor* p) {
        p->SET(0, p->C);
    };
    (*cboperations)[0xC2] = [](Processor* p) {
        p->SET(0, p->D);
    };
    (*cboperations)[0xC3] = [](Processor* p) {
        p->SET(0, p->E);
    };
    (*cboperations)[0xC4] = [](Processor* p) {
        p->SET(0, p->H);
    };
    (*cboperations)[0xC5] = [](Processor* p) {
        p->SET(0, p->L);
    };
    (*cboperations)[0xC6] = [](Processor* p) {
        p->SET(0, p->H, p->L);
    };
    (*cboperations)[0xC7] = [](Processor* p) {
        p->SET(0, p->A);
    };
    (*cboperations)[0xC8] = [](Processor* p) {
        p->SET(1, p->B);
    };
    (*cboperations)[0xC9] = [](Processor* p) {
        p->SET(1, p->C);
    };
    (*cboperations)[0xCA] = [](Processor* p) {
        p->SET(1, p->D);
    };
    (*cboperations)[0xCB] = [](Processor* p) {
        p->SET(1, p->E);
    };
    (*cboperations)[0xCC] = [](Processor* p) {
        p->SET(1, p->H);
    };
    (*cboperations)[0xCD] = [](Processor* p) {
        p->SET(1, p->L);
    };
    (*cboperations)[0xCE] = [](Processor* p) {
        p->SET(1, p->H, p->L);
    };
    (*cboperations)[0xCF] = [](Processor* p) {
        p->SET(1, p->A);
    };
    (*cboperations)[0xD0] = [](Processor* p) {
        p->SET(2, p->B);
    };
    (*cboperations)[0xD1] = [](Processor* p) {
        p->SET(2, p->C);
    };
    (*cboperations)[0xD2] = [](Processor* p) {
        p->SET(2, p->D);
    };
    (*cboperations)[0xD3] = [](Processor* p) {
        p->SET(2, p->E);
    };
    (*cboperations)[0xD4] = [](Processor* p) {
        p->SET(2, p->H);
    };
    (*cboperations)[0xD5] = [](Processor* p) {
        p->SET(2, p->L);
    };
    (*cboperations)[0xD6] = [](Processor* p) {
        p->SET(2, p->H, p->L);
    };
    (*cboperations)[0xD7] = [](Processor* p) {
        p->SET(2, p->A);
    };
    (*cboperations)[0xD8] = [](Processor* p) {
        p->SET(3, p->B);
    };
    (*cboperations)[0xD9] = [](Processor* p) {
        p->SET(3, p->C);
    };
    (*cboperations)[0xDA] = [](Processor* p) {
        p->SET(3, p->D);
    };
    (*cboperations)[0xDB] = [](Processor* p) {
        p->SET(3, p->E);
    };
    (*cboperations)[0xDC] = [](Processor* p) {
        p->SET(3, p->H);
    };
    (*cboperations)[0xDD] = [](Processor* p) {
        p->SET(3, p->L);
    };
    (*cboperations)[0xDE] = [](Processor* p) {
        p->SET(3, p->H, p->L);
    };
    (*cboperations)[0xDF] = [](Processor* p) {
        p->SET(3, p->A);
    };
    (*cboperations)[0xE0] = [](Processor* p) {
        p->SET(4, p->B);
    };
    (*cboperations)[0xE1] = [](Processor* p) {
        p->SET(4, p->C);
    };
    (*cboperations)[0xE2] = [](Processor* p) {
        p->SET(4, p->D);
    };
    (*cboperations)[0xE3] = [](Processor* p) {
        p->SET(4, p->E);
    };
    (*cboperations)[0xE4] = [](Processor* p) {
        p->SET(4, p->H);
    };
    (*cboperations)[0xE5] = [](Processor* p) {
        p->SET(4, p->L);
    };
    (*cboperations)[0xE6] = [](Processor* p) {
        p->SET(4, p->H, p->L);
    };
    (*cboperations)[0xE7] = [](Processor* p) {
        p->SET(4, p->A);
    };
    (*cboperations)[0xE8] = [](Processor* p) {
        p->SET(5, p->B);
    };
    (*cboperations)[0xE9] = [](Processor* p) {
        p->SET(5, p->C);
    };
    (*cboperations)[0xEA] = [](Processor* p) {
        p->SET(5, p->D);
    };
    (*cboperations)[0xEB] = [](Processor* p) {
        p->SET(5, p->E);
    };
    (*cboperations)[0xEC] = [](Processor* p) {
        p->SET(5, p->H);
    };
    (*cboperations)[0xED] = [](Processor* p) {
        p->SET(5, p->L);
    };
    (*cboperations)[0xEE] = [](Processor* p) {
        p->SET(5, p->H, p->L);
    };
    (*cboperations)[0xEF] = [](Processor* p) {
        p->SET(5, p->A);
    };
    (*cboperations)[0xF0] = [](Processor* p) {
        p->SET(6, p->B);
    };
    (*cboperations)[0xF1] = [](Processor* p) {
        p->SET(6, p->C);
    };
    (*cboperations)[0xF2] = [](Processor* p) {
        p->SET(6, p->D);
    };
    (*cboperations)[0xF3] = [](Processor* p) {
        p->SET(6, p->E);
    };
    (*cboperations)[0xF4] = [](Processor* p) {
        p->SET(6, p->H);
    };
    (*cboperations)[0xF5] = [](Processor* p) {
        p->SET(6, p->L);
    };
    (*cboperations)[0xF6] = [](Processor* p) {
        p->SET(6, p->H, p->L);
    };
    (*cboperations)[0xF7] = [](Processor* p) {
        p->SET(6, p->A);
    };
    (*cboperations)[0xF8] = [](Processor* p) {
        p->SET(7, p->B);
    };
    (*cboperations)[0xF9] = [](Processor* p) {
        p->SET(7, p->C);
    };
    (*cboperations)[0xFA] = [](Processor* p) {
        p->SET(7, p->D);
    };
    (*cboperations)[0xFB] = [](Processor* p) {
        p->SET(7, p->E);
    };
    (*cboperations)[0xFC] = [](Processor* p) {
        p->SET(7, p->H);
    };
    (*cboperations)[0xFD] = [](Processor* p) {
        p->SET(7, p->L);
    };
    (*cboperations)[0xFE] = [](Processor* p) {
        p->SET(7, p->H, p->L);
    };
    (*cboperations)[0xFF] = [](Processor* p) {
        p->SET(7, p->A);
    };
}
