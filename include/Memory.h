#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
    public:
        Memory();
        virtual ~Memory();
    private:

    public:
        uint8_t readByte(uint16_t address);
        uint16_t readWord(uint16_t address);
        void writeByte(uint16_t address, uint8_t byte);
        void writeWord(uint16_t address, uint16_t word);

};

#endif // MEMORY_H
