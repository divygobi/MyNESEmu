#include <bus.h>

Bus()
{
    //connect the cpu to the bus
    cpu.connectBus(this);   
    // intialize the fake ram to 0
    for (u_int8_t &i : ram)
    {
        i = 0x00;
    }
}

~Bus()
{
}

void write(uint16_t addy, uint8_t data)
{
    if(addy >=  0x0000 && addy <= 0xFFFF){
        ram[addy] = data;
    }
}

uint8_t read(uint16_t addy, bool isReadOnly = false)
{
    if(addy >=  0x0000 && addy <= 0xFFFF ){
        return ram[addy];
    }
    return 0x00;
    
}