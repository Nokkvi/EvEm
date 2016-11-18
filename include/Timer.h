#ifndef TIMER_H
#define TIMER_H

#include "Definitions.h"

class Processor;

class Timer
{
    public:
        Processor* p;
        Timer(Processor* p);
        uint8_t clockmain, clocksub, clockdiv;
        uint8_t regdiv, regtima, regtma, regtac;
        void Reset();
        void Step();
        void Inc();
        uint8_t ReadByte(uint16_t address);
        void WriteByte(uint16_t address, uint8_t value);
        virtual ~Timer();

    protected:

    private:
};

#endif // TIMER_H
