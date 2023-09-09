#pragma once
#include <cstdint>
#include <memory>

#include "olcPixelGameEngine.h"

#include "cartridge.h"


class ppu2C02{
    public:
        ppu2C02();
        ~ppu2C02();

        void connectBus(Bus *b){bus = b; };
        
        //communications with the main bus
        void cpuWrite(uint16_t addy, uint8_t data);
        uint8_t cpuRead(uint16_t addy, bool isReadOnly = false);

        //communications with the ppu bus
        void ppuWrite(uint16_t addy, uint8_t data);
        uint8_t ppuRead(uint16_t addy, bool isReadOnly = false);

        //loads cartridge into the ppu
        void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	    void clock();

    private:
        //pointer to the cartridge
        std::shared_ptr<Cartridge> cart;
        Bus* bus = nullptr;
    private:		
        uint8_t     tblName[2][1024];
        //pattern memory(exists on the cartridge usually)
        uint8_t     tblPattern[2][4096]; // 2 4kb tables, could eventually be scaled into running games with more capabilities
        uint8_t		tblPalette[32];
}