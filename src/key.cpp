#include "key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

key::key()
{
    this->_keys = new uint8_t[2]{ 0x0F, 0x0F };
    this->_colidx = 0;


}

void key::reset(){
    this->_keys[0] = 0x0F;
    this->_keys[1] = 0x0F;
    this->_colidx = 0;
}

uint8_t key::readByte(uint16_t addr){
    switch(this->_colidx){
        case 0x00: return 0x00; break;
        case 0x10: return this->_keys[0]; break;
        case 0x20: return this->_keys[1]; break;
        default: return 0x00; break;
    }
}

void key::writeByte(uint16_t addr, uint8_t val){
    this->_colidx = val&0x30;
}

void key::handleKeys(int key, int action){
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        this->_keys[1] &= 0xE;
    }else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        this->_keys[1] &= 0xD;
    }else if(key == GLFW_KEY_UP && action == GLFW_PRESS){
        this->_keys[1] &= 0xB;
    }else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        this->_keys[1] &= 0x7;
    }else if(key == GLFW_KEY_Z && action == GLFW_PRESS){
        this->_keys[0] &= 0xE;
    }else if(key == GLFW_KEY_X && action == GLFW_PRESS){
        this->_keys[0] &= 0xD;
    }else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        this->_keys[0] &= 0xB;
    }else if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
        this->_keys[0] &= 0x7;
    }else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE){
        this->_keys[1] |= 0x1;
    }else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE){
        this->_keys[1] |= 0x2;
    }else if(key == GLFW_KEY_UP && action == GLFW_RELEASE){
        this->_keys[1] |= 0x4;
    }else if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE){
        this->_keys[1] |= 0x8;
    }else if(key == GLFW_KEY_Z && action == GLFW_RELEASE){
        this->_keys[0] |= 0x1;
    }else if(key == GLFW_KEY_X && action == GLFW_RELEASE){
        this->_keys[0] |= 0x2;
    }else if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE){
        this->_keys[0] |= 0x4;
    }else if(key == GLFW_KEY_ENTER && action == GLFW_RELEASE){
        this->_keys[0] |= 0x8;
    }
}

key::~key()
{
    delete[] _keys;
}
