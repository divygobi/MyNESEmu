#include <mapper_000.h>
mapper_000::mapper_000(uint8_t prgBanks, uint8_t chrBanks){
        nPRGBanks = prgBanks;
        nCHRBanks = chrBanks;
}

mapper_000:~mapper_000(){

}

bool mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr){
    if(addr >= 0x8000 && addr <= 0xFFFF){
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

}

bool mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr){
        if(addr >= 0x8000 && addr <= 0xFFFF){
            mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
            return true;
        }
}

bool mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr){
    if(addr >= 0x0000 && addr <= 0x1FFF){
        mapped_addr = addr;
        return true;
    }
}

bool mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr){
    if(addr >= 0x0000 && addr <= 0x1FFF){
        if(nCHRBanks == 0){
            mapped_addr = addr;
            return true;
        }
    }

}