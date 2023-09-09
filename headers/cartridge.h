#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <memory>

#include "mapper_000.h"

class cartridge{
    public:
        cartridge(const std::string& sFileName);
        ~cartridge();
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
        uint8_t nPRGBanks = 0;
        uint8_t nCHRBanks = 0;

        std::vector<uint8_t> vPRGMemory;
        std::vector<uint8_t> vCHRMemory;

        std::shared_ptr<Mapper> pMapper;

        bool cpuWrite(uint16_t addy, uint8_t data);
        bool cpuRead(uint16_t addy, uint8_t data);

        //communications with the ppu bus
        bool ppuWrite(uint16_t addy, uint8_t data);
        bool ppuRead(uint16_t addy, uint8_t data);
}