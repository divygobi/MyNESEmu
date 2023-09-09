#include "cartridge.h"

#

 //constructor
cartridge::cartridge(const std::string& sFileName)
{
    //iNES format header
   struct sHeader{
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
   } header;

    //open the file
    std: ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);

    if (ifs.is_open()){
        //read file header in 
        ifs.read((char*)&header, sizeof(sHeader));
    }

    if (header.mapper1 & 0x04){
        ifs.seekg(512, std::ios_base::cur);
    }

    nMapperId = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

    //Discover the file format
    uint8_t nFileType = 1;

    if(nFileType == 0){

    }

    if(nFileType == 1){
        //PRG IS THE PROGRAM ROM
        nPRGBanks = header.prg_rom_chunks;
        vPRGMemory.resize(nPRGBanks * 16384);
        ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

        //CHR IS THE CHARACTER ROM(sprites and stuff)
        nCHRBanks = header.chr_rom_chunks;
        vCHRMemory.resize(nCHRBanks * 8192);
        ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
    }

    if(nFileType == 2){

    }

    switch(nMapperID){
        case 0: pMapper = std::make_shared<mapper_000>(nPRGBanks, nCHRBanks); break;
    }

    ifs.close()
   
}

cartridge::~cartridge()
{
    //destructor
}

bool cartridge::cpuRead(uint16_t addy, uint8_t & data)
{
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addy, mapped_addr)){
        data = vPRGMemory[mapped_addr];
        return true;
    }
    else{
        return false;
    }
}


bool cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr))
	{
		vPRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool cartridge::ppuRead(uint16_t addr, uint8_t & data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		data = vCHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}