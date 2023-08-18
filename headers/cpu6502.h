#pragma once
#include <bus.h>

class Bus;

class cpu6502
{
//set up of the CPU
public:
    cpu6502();
    ~cpu6502();

    void connectBus(Bus *b){bus = b; }

public:
	// These are the 6502 CPU registers
    uint8_t accReg = 0x00; //accumalator register
    uint8_t xReg = 0x00; //X register
    uint8_t yReg = 0x00; //Y register
    uint8_t stckPtr = 0x00; //stack pointer (points to a location on the bus)
    uint16_t prgrmCtnr = 0x0000; //program counter (16-bit)
    uint8_t statusReg = 0x00; //status register (8-bit)

	// enumeration of the bits of the status register. 6502 is an 8-bit architecture
    enum FLAGS6502		
						// status registers reflects the results of each instruction executed and general state of the CPU
	{
        C = (1 << 0), // carry bit: the carry or borrow from the most significant bit during arithmetic operations
        Z = (1 << 1), // zero: when result of operation is 0
        I = (1 << 2), // disable interrupts: interrupts from external sources are disabled.
        D = (1 << 3), // decimal mode (not needed)
        B = (1 << 4), // break:  indicate that the interrupt was caused by a software breakpoint
        U = (1 << 5), // unused
        V = (1 << 6), // overflow: when processor uses signed variables
        N = (1 << 7), // negative: when processor uses signed variables
    };
    
	// Event Functions (asserts change in state)
	void clock();	// indicates to cpu that we want 1 clock cycle to occur

	// These functions will let CPU finish instruction before running 
	void reset();   // reset the cpu to a known state
	void irq();     // interrupt request signal
	void nmi();     // non-maskable interrupt request signal
};

//any devices the cpu is connected to
private:
    Bus *bus = nullptr;

private: 
	// Addressing Modes =============================================
	// The 6502 has a variety of addressing modes to access data in 
	// memory, some of which are direct and some are indirect (like
	// pointers in C++). Each opcode contains information about which
	// addressing mode should be employed to facilitate the 
	// instruction, in regards to where it reads/writes the data it
	// uses. The address mode changes the number of bytes that
	// makes up the full instruction, so we implement addressing
	// before executing the instruction, to make sure the program
	// counter is at the correct location, the instruction is
	// primed with the addresses it needs, and the number of clock
	// cycles the instruction requires is calculated. These functions
	// may adjust the number of cycles required depending upon where
	// and how the memory is accessed, so they return the required
	// adjustment.

	uint8_t IMP();	uint8_t IMM();	
	uint8_t ZP0();	uint8_t ZPX();	
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();	
	uint8_t ABY();	uint8_t IND();	
	uint8_t IZX();	uint8_t IZY();

private: 
	// Opcodes ======================================================
	// There are 56 "legitimate" opcodes provided by the 6502 CPU. I
	// have not modelled "unofficial" opcodes. As each opcode is 
	// defined by 1 byte, there are potentially 256 possible codes.
	// Codes are not used in a "switch case" style on a processor,
	// instead they are repsonisble for switching individual parts of
	// CPU circuits on and off. The opcodes listed here are official, 
	// meaning that the functionality of the chip when provided with
	// these codes is as the developers intended it to be. Unofficial
	// codes will of course also influence the CPU circuitry in 
	// interesting ways, and can be exploited to gain additional
	// functionality!
	//
	// These functions return 0 normally, but some are capable of
	// requiring more clock cycles when executed under certain
	// conditions combined with certain addressing modes. If that is 
	// the case, they return 1.
	//
	// I have included detailed explanations of each function in 
	// the class implementation file. Note they are listed in
	// alphabetical order here for ease of finding.

	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

	// I capture all "unofficial" opcodes with this function. It is
	// functionally identical to a NOP
	uint8_t XXX();

	uint8_t fetch(); // fetches data from the bus

	uint8_t fetched = 0x00; // data fetched from the bus
	uint16_t addr_abs = 0x0000; // absolute address. to read from diff locations of memory
	uint16_t addr_rel = 0x0000; // relative address. in 6502, branch can only jump certain distance from current location which is why we need relative addr
	uint8_t opcode = 0x00; // current opcode
	uint8_t cycles = 0; // number of cycles the instruction has remaining
    

private:
    uint8_t read(uint16_t addy);
    void write(uint16_t addy, uint8_t data);

    void setFlag(FLAGS6502 f, bool v);
    uint8_t getFlag(FLAGS6502 f);

	struct INSTRUCTION			// This structure stores all the properties of a specific instruction
	{
		std::string name;						// Name of the instruction - used for disassembly listing
		uint8_t(cpu6502::*operate)(void) = nullptr;	// Function pointer to the implementation of the operation (one of Opcodes)
		uint8_t(cpu6502::*addrmode)(void) = nullptr;	// Function pointer to the implementation of the address mode (one of Adress Modes)
		uint8_t cycles = 0;						// Number of clock cycles needed for the instruction to execute
	};

	std::vector<INSTRUCTION> lookup;
	

