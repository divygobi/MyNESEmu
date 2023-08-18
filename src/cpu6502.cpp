#include <cpu6502.h>

cpu6502();
~cpu6502();
uint8_t read(uint16_t addy){
    return bus->read(addy, false);

}

void ::write(uint16_t addy, uint8_t data){
    bus->write(addy, data);
}