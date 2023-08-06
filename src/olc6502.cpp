#include <olc6502.h>

Olc6502();
~Olc6502();
uint8_t read(uint16_t addy){
    bus->read(addy, false);

}

void ::write(uint16_t addy, uint8_t data){
    bus->write(addy, data);
}