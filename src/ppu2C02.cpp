#include <ppu2C02.h>


ppu2C02::ppu2C02()
{
}

~ppu2C02::ppu2C02()
{
}

void ppu2C02::connectBus(Bus *b)
{
    bus = b;
}

//reading and writing to the cpu bus
void ppu2C02::cpuWrite(uint16_t addy, uint8_t data)
{
    switch (addy){
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;

    }
}

uint8_t ppu2C02::cpuRead(uint16_t addy, bool isReadOnly){
        switch (addy){
            case 0x0000: // Control
                break;
            case 0x0001: // Mask
                break;
            case 0x0002: // Status
                break;
            case 0x0003: // OAM Address
                break;
            case 0x0004: // OAM Data
                break;
            case 0x0005: // Scroll
                break;
            case 0x0006: // PPU Address
                break;
            case 0x0007: // PPU Data
                break;

        }
}

//reading and writing to the ppus bus
uint8_t ppu2C02::ppuRead(uint16_t addy, bool isReadOnly){
    uint8_t data = 0x00;
    //mirror the data  
    addy &= 0x3FFF;

    if(cart->ppuRead(addy, data)){
        
    }

    return data;
}

void ppu2C02::ppuWrite(uint16_t addy, uint8_t data){
    //mirror the data  
    addy &= 0x3FFF;

    if(cart->ppuWrite(addy, data)){
        
    }
}

void ppu2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
}