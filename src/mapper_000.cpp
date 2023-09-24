#include "../headers/mapper_000.h"
mapper_000::mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks){
        nPRGBanks = prgBanks;
        nCHRBanks = chrBanks;
}

mapper_000::~mapper_000(){

}

//ROMS ARE 32k OR 16k, we need to mask the address based on this
bool mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr){
    if(addr >= 0x8000 && addr <= 0xFFFF){

        //if we have more tyhan one bank of rom, we mask it with 0x7FFF
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr){
        if(addr >= 0x8000 && addr <= 0xFFFF){
            mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }
        return false;
}

bool mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr){
    //encomppasses the pattern tables
    if(addr >= 0x0000 && addr <= 0x1FFF){
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr){
    //you are not goin to be writing to the cartridge, \ just return false
    //if the cartidge has some sort of ram, then yiou can do this
    if(addr >= 0x0000 && addr <= 0x1FFF){
        if(nCHRBanks == 0){
                //treating as RAM
            mapped_addr = addr;
            return true;
        }
    }
    return false;
}
