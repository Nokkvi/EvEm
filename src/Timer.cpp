#include "Timer.h"

#include "Processor.h"

Timer::Timer(Processor* p)
{
    this->p = p;
    this->clockdiv = 0;
    this->clockmain = 0;
    this->clocksub = 0;
    this->regdiv = 0;
    this->regtac = 0;
    this->regtima = 0;
    this->regtma = 0;
}

Timer::~Timer()
{

}

void Timer::Reset(){
    this->clockdiv = 0;
    this->clockmain = 0;
    this->clocksub = 0;
    this->regdiv = 0;
    this->regtac = 0;
    this->regtima = 0;
    this->regtma = 0;
}

void Timer::Step(){
    this->regtima++;
    this->clockmain = 0;
    if(this->regtima > 255){
        this->regtima = this->regtma;
        this->p->memory->interruptFlags |= 4;
    }
}

void Timer::Inc(){
    uint8_t oldclock = this->clockmain;
    this->clocksub += this->p->M->GetByte(0);
    if(this->clocksub > 3){
        this->clockmain++;
        this->clocksub -= 4;

        this->clockdiv++;
        if(this->clockdiv==16){
            this->clockdiv = 0;
            this->regdiv++;
            this->regdiv &= 255;
        }
    }

    if(this->regtac & 4){
        switch(this->regtac & 3)
        {
            case 0:
                if(this->clockmain >= 64) this->Step();
                break;
            case 1:
                if(this->clockmain >= 1) this->Step();
                break;
            case 2:
                if(this->clockmain >= 4) this->Step();
                break;
            case 3:
                if(this->clockmain >= 16) this->Step();
                break;

        }
    }
}

uint8_t Timer::ReadByte(uint16_t address){
    switch(address){
        case 0xFF04: return this->regdiv;
        case 0xFF05: return this->regtima;
        case 0xFF06: return this->regtma;
        case 0xFF07: return this->regtac;
    }
}

void Timer::WriteByte(uint16_t address, uint8_t value){
    switch(address){
        case 0xFF04: this->regdiv = 0; break;
        case 0xFF05: this->regtima = value; break;
        case 0xFF06: this->regtma = value; break;
        case 0xFF07: this->regtac = value&7; break;
    }
}
