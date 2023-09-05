#include "../headers/bus.h"

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

// only device on bus is cpu RAM
void Bus::cpuWrite(uint16_t addy, uint8_t data)
{
    if(addy >=  0x0000 && addy <= 0x1FFF){      // guard against invalid memory access this is the cpu ram
        //ANDED WITH VALUE 0x07FF TO ENABLE MIRRORING OF RAM
        ram[addy & 0x07FF] = data;                // write to RAM
    }
}

uint8_t Bus::cpuRead(uint16_t addy, bool isReadOnly = false)
{
    if(addy >=  0x0000 && addy <= 0x1FFF ){
        //ANDED WITH VALUE 0x07FF TO ENABLE MIRRORING OF RAM
        return ram[addy & 0x07FF];
    }
    return 0x00;
    
}