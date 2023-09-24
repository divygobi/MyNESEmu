#pragma once
#include <cstdint>
#include <memory>


#include "cartridge.h"
#include "../../NESEmuTesting/olcPixelGameEngine.h"

//to prevent circular includes
class Bus;

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
        void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	    void clock();

    private:
        //pointer to the cartridge
        std::shared_ptr<Cartridge> cart;
        Bus* bus = nullptr;

    private:
	olc::Pixel  palScreen[0x40];
	// In Video
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

	// Changed To for API breaking subsequent PGE Update
	olc::Sprite* sprScreen;
	olc::Sprite* sprNameTable[2];
	olc::Sprite* sprPatternTable[2];

	int16_t scanline = 0
	int16_t cycle = 0

    public:
            // Debugging Utilities
        olc::Sprite& GetScreen();
        olc::Sprite& GetNameTable(uint8_t i);
        olc::Sprite& GetPatternTable(uint8_t i);
        bool frame_complete = false;
    private:

        uint8_t     tblName[2][1024];
        //pattern memory(exists on the cartridge usually)
        uint8_t     tblPattern[2][4096]; // 2 4kb tables, could eventually be scaled into running games with more capabilities
        uint8_t		tblPalette[32];
};
