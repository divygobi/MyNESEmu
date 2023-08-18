#include <bus.h>

Bus::Bus()
{
    //connect the cpu to the bus, function from cpu6502.h
    cpu.connectBus(this);   

    // intialize the RAM content to empty
    for (u_int8_t &i : ram)
    {
        i = 0x00;
    }
}

Bus::~Bus()
{
}

// only device on bus is RAM
void Bus::write(uint16_t addy, uint8_t data)
{
    if(addy >=  0x0000 && addy <= 0xFFFF){      // guard against invalid memory access
        ram[addy] = data;                // write to RAM
    }
}

uint8_t Bus::read(uint16_t addy, bool isReadOnly = false)
{
    if(addy >=  0x0000 && addy <= 0xFFFF ){
        return ram[addy];
    }
    return 0x00;
    
}