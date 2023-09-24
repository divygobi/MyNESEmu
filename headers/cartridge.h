#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <memory>

#include "mapper_000.h"

class Cartridge{
    public:
        Cartridge(const std::string& sFileName);
        ~Cartridge();
        bool ImageValid();

        enum MIRROR
        {
            HORIZONTAL,
            VERTICAL,
            ONESCREEN_LO,
            ONESCREEN_HI,
        } mirror = HORIZONTAL;

    private:
        bool bImageValid = false;

        uint8_t nMapperID = 0;
        //how many banks of the program cha charecter memory
        uint8_t nPRGBanks = 0;
        uint8_t nCHRBanks = 0;

        //CPU WILL BE INTERACTING WITH THE PROGRAM MEMORY

        std::vector<uint8_t> vPRGMemory;

        //PPU WILL BE ITNERACTING WITH THE CHARECTER MEMORY(SPRITES and such)
        std::vector<uint8_t> vCHRMemory;

        std::shared_ptr<Mapper> pMapper;
    public:
        bool cpuWrite(uint16_t addy, uint8_t data);
        bool cpuRead(uint16_t addy, uint8_t &data);

        //communications with the ppu bus
        bool ppuWrite(uint16_t addy, uint8_t data);
        bool ppuRead(uint16_t addy, uint8_t &data);
};
