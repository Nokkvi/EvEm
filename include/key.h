#ifndef KEY_H
#define KEY_H

#include <functional>


class key
{
    public:
        uint8_t* _keys;
        uint8_t _colidx;

        void reset();

        uint8_t readByte(uint16_t address);
        void writeByte(uint16_t address, uint8_t val);

        void handleKeys(int key, int action);

    public:
        key();
        virtual ~key();

    protected:

    private:
};

#endif // KEY_H
