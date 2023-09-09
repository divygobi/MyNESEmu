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
    //CARTRIDGE HAS PRIORITY OVER RAM, flexible extension of the cpu
    if (cart->cpuWrite(addy, data)){

    }
    else if(addy >=  0x0000 && addy <= 0x1FFF){      // guard against invalid memory access this is the cpu ram
        //ANDED WITH VALUE 0x07FF TO ENABLE MIRRORING OF RAM
        ram[addy & 0x07FF] = data;                // write to RAM
    }
    else uf (addy >= 0x2000 && addy <=0x3FFF){   // this is the ppu address space 
        ppu.cpuWrite(addy & 0x0007, data);
    }
}

uint8_t Bus::cpuRead(uint16_t addy, bool isReadOnly = false)
{
    data = 0x00;
    if (cart->cpuRead(addy, data)){

    }
    else if(addy >=  0x0000 && addy <= 0x1FFF ){
        //ANDED WITH VALUE 0x07FF TO ENABLE MIRRORING OF RAM
        data = ram[addy & 0x07FF];
    }
    else(addy >= 0x2000 && addy <=0x3FFF){
        data = ppu.cpuRead(addy & 0x0007, isReadOnly);
    }
    
    return data;
    
}

// Connects a cartridge object to the internal buses
void insertCartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}
	// Resets the system, represents a literal reset on the front of the NES
void reset(){

}
// Clocks the system - a single whole systme tick
void clock(){

}