#include "../include/Processor.h"
#include "../include/Memory.h"

#include <string>

int main()
{
    Processor p(new Memory());
    p.A->SetByte(0, 0x01);
    p.B->SetByte(0, 0x01);
    p.ProcessOpcode(0x80);

    printf("" + p.A->GetByte(0));

    return 0;
}
