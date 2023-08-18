#pragma once
#include <cstdint>
#include <cpu6502.h>
#include <array>

class Bus
{

public:
    // Constructer and Deconstructer
    Bus();
    ~Bus();

public:
    // Devices on the bus
    cpu6502 cpu;

    // dummy 64 kilobyte ram
    std::array<u_int8_t, 64 * 1024> ram;

public:
    // Read and write for bus
    void write(uint16_t addy, uint8_t data);
    uint8_t read(uint16_t addy, bool isReadOnly = false);
};