#include "../include/Processor.h"

void Processor::ProcessOpcode(uint8_t code)
{
    (*operations)[code](this);
}

void Processor::InitOpcodes()
{
    operations = new std::unordered_map<uint8_t, std::function<void(Processor* p)>>();
    (*operations)[0x00] = [](Processor* p) {

    };
    (*operations)[0x01] = [](Processor* p) {

    };
    (*operations)[0x02] = [](Processor* p) {

    };
    (*operations)[0x03] = [](Processor* p) {

    };
    (*operations)[0x04] = [](Processor* p) {

    };
    (*operations)[0x05] = [](Processor* p) {

    };
    (*operations)[0x06] = [](Processor* p) {

    };
    (*operations)[0x07] = [](Processor* p) {

    };
    (*operations)[0x08] = [](Processor* p) {

    };
    (*operations)[0x09] = [](Processor* p) {

    };
    (*operations)[0x0A] = [](Processor* p) {

    };
    (*operations)[0x0B] = [](Processor* p) {

    };
    (*operations)[0x0C] = [](Processor* p) {

    };
    (*operations)[0x0D] = [](Processor* p) {

    };
    (*operations)[0x0E] = [](Processor* p) {

    };
    (*operations)[0x0F] = [](Processor* p) {

    };
    (*operations)[0x80] = [](Processor* p) {
        p->ADD(p->A, p->B);
    };
}
