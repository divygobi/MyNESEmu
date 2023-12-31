#pragma once
#include <cstdint>
#include "cpu6502.h"
#include "ppu2C02.h"
#include "cartridge.h"
#include <array>

class Bus
{

public:
    // Constructer and Deconstructer
    Bus();
    ~Bus();

    // Devices on the bus
    cpu6502 cpu;
    ppu2C02 ppu;
    std::shared_ptr<Cartridge> cart;

    //  2 kilobyte cpu ram
    uint8_t cpuRam[2048];


    // Read and write for bus
    void cpuWrite(uint16_t addy, uint8_t data);
    uint8_t cpuRead(uint16_t addy, bool isReadOnly = false);

    // Connects a cartridge object to the internal buses
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	// Resets the system, represents a literal reset on the front of the NES
	void reset();
	// Clocks the system - a single whole systme tick
	void clock();

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;


};
