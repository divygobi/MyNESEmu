#include "../headers/cpu6502.h"
#include "../headers/bus.h"
#include <map>

cpu6502::cpu6502()
{
	// Assembles the translation table. It's big, it's ugly, but it yields a convenient way
	// to emulate the 6502. I'm certain there are some "code-golf" strategies to reduce this
	// but I've deliberately kept it verbose for study and alteration

	// It is 16x16 entries. This gives 256 instructions. It is arranged to that the bottom
	// 4 bits of the instruction choose the column, and the top 4 bits choose the row.

	// For convenience to get function pointers to members of this class, I'm using this
	// or else it will be much much larger :D

	// The table is one big initialiser list of initialiser lists...
	using a = cpu6502;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

cpu6502::~cpu6502(){
}

bool cpu6502::complete(){
     return cycles == 0;
}

uint8_t cpu6502::read(uint16_t addy){
    return bus->cpuRead(addy, false);
}

void cpu6502::write(uint16_t addy, uint8_t data){
    bus->cpuWrite(addy, data);
}

void cpu6502::clock()
{
	/* execute when we have 0 clock cycles remaning in the current running instrution,
	 we read in from the program counter and run an instruction */

	if (cycles == 0)
    {
		opcode = read(prgrmCtnr);
		prgrmCtnr++;

		// get starting number of cycles
        cycles = lookup[opcode].cycles;

        uint8_t addional_cycle1 = (this->*lookup[opcode].addrmode)(); // call the function required for the address mode. returns either 0 or 1 cycles. Max 3 cycles
        uint8_t addional_cycle2 = (this->*lookup[opcode].operate)(); // call the function required for the run the instruction. returns either 0 or 1 cycles

        cycles += (addional_cycle1 & addional_cycle2);

	}
    cycles--;   // executing instruction at one point in time, meaning one tick has passed
}


//INTERRUPT FUNCTIONS BELOW
// reset the cpu to a known state
void cpu6502::reset(){
    accReg = 0;
    xReg = 0;
    yReg = 0;
    stckPtr = 0xFD;
    statusReg = 0x00 | U;

    //set the PC to FFFC
    addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	// Set it
	prgrmCtnr = (hi << 8) | lo;

    // Clear internal helper variables
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles = 8;
}

// Interrupt requests are a complex operation and only happen if the
// "disable interrupt" flag is 0. IRQs can happen at any time, but
// you dont want them to be destructive to the operation of the running
// program. Therefore the current instruction is allowed to finish
// (which I facilitate by doing the whole thing when cycles == 0) and
// then the current program counter is stored on the stack. Then the
// current status register is stored on the stack. When the routine
// that services the interrupt has finished, the status register
// and program counter can be restored to how they where before it
// occurred. This is impemented by the "RTI" instruction. Once the IRQ
// has happened, in a similar way to a reset, a programmable address
// is read form hard coded location 0xFFFE, which is subsequently
// set to the program counter.
void cpu6502::irq(){
    if(getFlag(I) == 0){
        //push the program counter to the stack
        write(0x0100 + stckPtr, (prgrmCtnr >> 8) & 0x00FF);
        stckPtr--;
        write(0x0100 + stckPtr, prgrmCtnr & 0x00FF);
        stckPtr--;

        //push the status register to the stack
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);
        write(0x0100 + stckPtr, statusReg);
        stckPtr--;

        //set the program counter to the address at FFFA
        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        prgrmCtnr = (hi << 8) | lo;

        //IRQ takes time
        cycles = 7;
    }
}

// A Non-Maskable Interrupt cannot be ignored. It behaves in exactly the
// same way as a regular IRQ, but reads the new program counter address
// form location 0xFFFA
void cpu6502::nmi(){
    //push the program counter to the stack
    write(0x0100 + stckPtr, (prgrmCtnr >> 8) & 0x00FF);
    stckPtr--;
    write(0x0100 + stckPtr, prgrmCtnr & 0x00FF);
    stckPtr--;

    //push the status register to the stack
    setFlag(B, 0);
    setFlag(U, 1);
    setFlag(I, 1);
    write(0x0100 + stckPtr, statusReg);
    stckPtr--;

    //set the program counter to the address at FFFA
    addr_abs = 0xFFFA;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    prgrmCtnr = (hi << 8) | lo;

    //IRQ takes time
    cycles = 8;
}



//set and get flag functions
uint8_t cpu6502::getFlag(FLAGS6502 f)
{

    return ((statusReg & f) > 0) ? 1 : 0;
}

void cpu6502::setFlag(FLAGS6502 f, bool v)
{
    if (v)
        statusReg |= f;
    else
        statusReg &= ~f;
}


// Addressing Modes IMPLEMENTATIONS. It calculates whether or not we need an additional clock cycle (return 1) or not (return 0)
/*
    There is a need for an additional clock cycle when accessing another page in memory due to the complexity of memory management.
    Accessing another page if there is an overflow requires more memory access time and thus an additional clock cycle.
    Page overflow happens when
*/
uint8_t cpu6502::IMP()     // implied and no data is part of instruction
{
	/*incoming data variable(fetched is set to accReg),
	the Implied addressing mode implies that the accumulator is the target or source of the operation */

	fetched = accReg;
	return 0;
}

uint8_t cpu6502::IMM()     // data will be next byte
{
	/* does not need an extra clock cycle to read in data,
	so when the operation is made,
	it uses this absolute address which is the next byte after the the opcode */

	addr_abs = prgrmCtnr++;
    return 0;
}

uint8_t cpu6502::ZP0()     // pages organizes memory
{
	//GET THE BYTE IN THE PAGE FROM THE NEXT BYTE IN THE PC
    addr_abs = read(prgrmCtnr);
	//INCREMENT PC to get to the next instruction
    prgrmCtnr++;

	//setting the address to the 0th page anded with the relative address provided in the next byte
    addr_abs & 0x00FF;
    return 0;
}

uint8_t cpu6502::ZPX()     // zero page addressing with x register offset
{
	addr_abs = (read(prgrmCtnr) + xReg);
    prgrmCtnr++;
    addr_abs & 0x00FF;
    return 0;
}

uint8_t cpu6502::ZPY()     // zero page addressing with y register offset
{
    addr_abs = (read(prgrmCtnr) + yReg);
    prgrmCtnr++;
    addr_abs & 0x00FF;
    return 0;
}

uint8_t cpu6502::ABS()     // absolute addressing. to get full and true address, THE HIGH BIT IS THE PAGE,
{

	// byte address
    uint16_t lo = read(prgrmCtnr);
    prgrmCtnr++;
    uint16_t hi = read(prgrmCtnr);
    prgrmCtnr++;

    addr_abs = (hi << 8) | lo;      // makes 16 byte address word
    return 0;
}


//TODO UNDERSTAND THIS LATER vvvvv

uint8_t cpu6502::ABX()  {
	// byte address
    uint16_t lo = read(prgrmCtnr);
    prgrmCtnr++;
    uint16_t hi = read(prgrmCtnr);
    prgrmCtnr++;

    addr_abs = (hi << 8) | lo;      // makes 16 byte address word
	addr_abs +=xReg;

	if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

//TODO UNDERSTAND THIS LATER vvvvv

uint8_t cpu6502::ABY()     // absolute addressing with y register offset
{
    uint16_t lo = read(prgrmCtnr);
    prgrmCtnr++;
    uint16_t hi = read(prgrmCtnr);
    prgrmCtnr++;

    addr_abs = (hi << 8) | lo;      // makes 16 byte address word
    addr_abs += yReg;

    if ((addr_abs & 0xFF00) != (hi << 8))       // corrects the overflow
        return 1;
    else
        return 0;
}

uint8_t cpu6502::IND()     // indirect addressing. 6502's way to implement pointers
{
    uint16_t ptr_lo = read(prgrmCtnr);
    prgrmCtnr++;
    uint16_t ptr_hi = read(prgrmCtnr);
    prgrmCtnr++;

    //ptr to the address we want
    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    //if the lo ptr address is the last one (FF)
    if (ptr_lo == 0x00FF)       // hardware bug. FF means 256
    {
        //read the address frm (hi + 0x00) and append the regular ptr to it
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    }
    else            // ignore the bug
    {
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
    }
    return 0;
}

uint8_t cpu6502::IZX()     // indirect addressing with x register offset
{
    uint16_t t = read(prgrmCtnr);
    prgrmCtnr++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)xReg) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)xReg + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t cpu6502::IZY()     // indirect addressing with y register offset
{
    uint16_t t = read(prgrmCtnr);
    prgrmCtnr++;

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += yReg;

    if ((addr_abs & 0xFF00) != (hi << 8))       // corrects the overflow
        return 1;
    else
        return 0;
}

//only applies to branching instruction
//cant jump any further any that 127 memory locations
uint8_t cpu6502::REL(){
    addr_rel = read(prgrmCtnr);
    prgrmCtnr++;
    //IF the first bit is 1 in the relative address its a negative sign.
    //We then or it with all 1s at the start so the pointer arthimatic will make sense
    if(addr_rel & 0x80){
        addr_rel |= 0xFF00;
    }
    return 0;
}

uint8_t cpu6502::fetch(){
    //IF the addressing mode is not implied, we need to fetch the data from the absolute adress
    //if it is implied, recall that the fetched data lies in the accumulator register
    if (!(lookup[opcode].addrmode == &cpu6502::IMP))
        //We get the data from the absolute address which was set in the respective addressing mode function
		fetched = read(addr_abs);
	return fetched;
}


//INSTRUCTIONS FOR THE CPU


//ANDS THE ACCUMULATOR WITH FETCHED DATA
uint8_t cpu6502::AND(){
    fetch();
    accReg = accReg & fetched;
    //set the zero flag if the ACCUMULTOR is 0
    setFlag(Z, accReg == 0x00);
    //set the negative flag if the ACCUMULATOR is negative
    setFlag(N, accReg & 0x80);

    // we need to return 1 because
    return 1;
}

//ADD WITH CARRY BIT
//This function will add to the accumaulator register and the carry bit
//If A is the accumalator avalue, B is the memory value(will be in fetched), and C is the carry bit, this function will do:
// A = A + B + C
uint8_t cpu6502::ADC(){
    // Grab the data that we are adding to the accumulator(sets fetched)
	fetch();

	// Add is performed in 16-bit domain for emulation to capture any
	// carry bit, which will exist in bit 8 of the 16-bit word
	temp = (uint16_t)accReg + (uint16_t)fetched + (uint16_t)getFlag(C);

	// The carry flag out exists in the high byte bit 0
	setFlag(C, temp > 255);

	// The Zero flag is set if the result is 0
	setFlag(Z, (temp & 0x00FF) == 0);

	// The signed Overflow flag is set based on all that up there! :D
	setFlag(V, (~((uint16_t)accReg ^ (uint16_t)fetched) & ((uint16_t)accReg ^ (uint16_t)temp)) & 0x0080);

	// The negative flag is set to the most significant bit of the result
	setFlag(N, temp & 0x80);

	// Load the result into the accumulator (it's 8-bit dont forget!)
	accReg = temp & 0x00FF;

	// This instruction has the potential to require an additional clock cycle
	return 1;
}




//ARTHIMATIC SHIFT LEFT
//Shifts all bits left by 1, gets rid of the MSB and makes the rightmost bit a 0
//The carry bit is set to the MSB before the shift
//The zero flag is set if the result is 0
//The negative flag is set if the MSB after the shift is 1
uint8_t cpu6502::ASL()
{
    fetch();
    //puts it into a 16 bit domain to capture the carry bit
    temp = (uint16_t)fetched << 1;
    setFlag(C, (temp & 0xFF00) > 0);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        accReg = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

//BRANCH IF HTE CARRY BIT IT NOT SET
uint8_t cpu6502::BCC()
{
	if (getFlag(C) == 0)
	{
        //increments cycle counter because we are branching
		cycles++;
		addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
		if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
			cycles++;

		prgrmCtnr = addr_abs;
	}
	return 0;
}


//BRANCH IF HTE CARRY BIT IS SET
uint8_t cpu6502::BCS()
{
	if (getFlag(C) == 1)
	{
        //increments cycle counter because we are branching
		cycles++;
		addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
		if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
			cycles++;

		prgrmCtnr = addr_abs;
	}
	return 0;
}

//BRANCH IF THE ZERO BIT IS SET
//If you do some operation to check if things are equal, you can do something like load accumalator(a) then subtract(b)
//and then check the zero bit. this is an equality check yfm
uint8_t cpu6502::BEQ(){
    if (getFlag(Z) == 1){
        //increments cycle counter because we are branching
		cycles++;
		addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
		if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
			cycles++;

		prgrmCtnr = addr_abs;

    }
    return 0;
}

//This instruction is basically a non descutive AND instruction because it doesnt change the value of the accumulator,
//it just ands the input and sets the flags according

uint8_t cpu6502::BIT()
{
	fetch();
	temp = accReg & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

//BRANCH IS NEGATIVE
uint8_t cpu6502::BMI(){
    if (getFlag(N) == 1){
        //increments cycle counter because we are branching
        cycles++;
        addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
        if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
            cycles++;

        prgrmCtnr = addr_abs;

    }
    return 0;
}


//branch if not equalt (if zero but is not set)
uint8_t cpu6502::BNE(){
    if (getFlag(Z)==0){
        //increments cycle counter because we are branching
        cycles++;
        addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
        if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
            cycles++;

        prgrmCtnr = addr_abs;
    }
    return 0;
}

//branch if postive
uint8_t cpu6502::BPL(){
     if (getFlag(N)==0){
        //increments cycle counter because we are branching
        cycles++;
        addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
        if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
            cycles++;

        prgrmCtnr = addr_abs;
    }
    return 0;
}

//this is a interreupt command
uint8_t cpu6502::BRK(){
    //increments pc
	prgrmCtnr++;

    //sets the interuppt flag
	setFlag(I, 1);
    //writes the high byte of program counter to the stack
	write(0x0100 + stckPtr, (prgrmCtnr >> 8) & 0x00FF);
	stckPtr--;
    //writes the low byte of the program counter to the stack
	write(0x0100 + stckPtr, prgrmCtnr & 0x00FF);
	stckPtr--;

    //sets the break flag(indicates that the interrupt was caused by a software breakpoint)
	setFlag(B, 1);
    //writes the status register to the stack
	write(0x0100 + stckPtr, statusReg);
	stckPtr--;
	setFlag(B, 0);

    //sets program counter to contents of some random frick off address at the end of the memory
	prgrmCtnr = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;

}

uint8_t cpu6502::BVC()
{
    if (getFlag(V) == 0)
    {
        //increments cycle counter because we are branching
        cycles++;
        addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
        if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
            cycles++;

        prgrmCtnr = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BVS()
{
    if (getFlag(V) == 1)
    {
        //increments cycle counter because we are branching
        cycles++;
        addr_abs = prgrmCtnr + addr_rel;

        //adds an extra cycle if the branch is on a new page
        if((addr_abs & 0xFF00) != (prgrmCtnr & 0xFF00))
            cycles++;

        prgrmCtnr = addr_abs;
    }
    return 0;
}

//ALL THESE GUYS CLEAR FLAGS vvvvvvvv

uint8_t cpu6502::CLC()
{
    setFlag(C, false);
    return 0;
}

uint8_t cpu6502::CLD()
{
    setFlag(D, false);
    return 0;
}

uint8_t cpu6502::CLI()
{
    setFlag(I, false);
    return 0;
}

uint8_t cpu6502::CLV()
{
    setFlag(V, false);
    return 0;
}

//COMPARE DATA WITH ACCUMULATOR
uint8_t cpu6502::CMP(){
    //gets data
    fetch();
    //does a subtraction operation but doesnt update the accumulator
    temp = (uint16_t)accReg - (uint16_t)fetched;
    //sets the carry bit if the accumulator is greater than or equal to the fetched data
    setFlag(C, accReg >= fetched);
    //sets the zero bit if the accumulator is equal to the fetched data
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    //sets the negative bit if the accumulator is less than the fetched data(the MSB of the low byte is 1)
    setFlag(N, temp & 0x0080);
    return 1;
}

//COMPARE DATA WITH X REGISTER
uint8_t cpu6502::CPX(){
    //gets data
    fetch();
    //does a subtraction operation but doesnt update the accumulator
    temp = (uint16_t)xReg - (uint16_t)fetched;
    //sets the carry bit if the accumulator is greater than or equal to the fetched data
    setFlag(C, xReg >= fetched);
    //sets the zero bit if the accumulator is equal to the fetched data
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    //sets the negative bit if the accumulator is less than the fetched data(the MSB of the low byte is 1)
    setFlag(N, temp & 0x0080);
    return 0;
}


//COMPARE DATA WITH Y REGSITER
uint8_t cpu6502::CPY(){
    //gets data
    fetch();
    //does a subtraction operation but doesnt update the accumulator
    temp = (uint16_t)yReg - (uint16_t)fetched;
    //sets the carry bit if the accumulator is greater than or equal to the fetched data
    setFlag(C, yReg >= fetched);
    //sets the zero bit if the accumulator is equal to the fetched data
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    //sets the negative bit if the accumulator is less than the fetched data(the MSB of the low byte is 1)
    setFlag(N, temp & 0x0080);
    return 0;
}

//Decrement Value at Memory Location
uint8_t cpu6502::DEC(){
    fetch();
    temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}

//Decrement X register
uint8_t cpu6502::DEX(){
    xReg--;
    setFlag(Z, xReg == 0x00);
    setFlag(N, xReg & 0x80);
    return 0;
}

//Decrement Y register
uint8_t cpu6502::DEY(){
    yReg--;
    setFlag(Z, yReg == 0x00);
    setFlag(N, yReg & 0x80);
    return 0;
}

//Bitwise XOR with accumulator
uint8_t cpu6502::EOR(){
    fetch();
    accReg = accReg ^ fetched;
    setFlag(Z, accReg == 0x00);
    setFlag(N, accReg & 0x80);
    return 1;
}

//Increment Value at Memory Location
uint8_t cpu6502::INC(){
    fetch();
    temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    setFlag(N, temp & 0x0080);
    return 0;
}

//incredment x register
uint8_t cpu6502::INX(){
    xReg++;
    setFlag(Z, xReg == 0x00);
    setFlag(N, xReg & 0x80);
    return 0;
}

//increment y register
uint8_t cpu6502::INY(){
    yReg++;
    setFlag(Z, yReg == 0x00);
    setFlag(N, yReg & 0x80);
    return 0;
}

//Jump to location stored in addr abs
uint8_t cpu6502::JMP(){
    prgrmCtnr = addr_abs;
    return 0;
}


//jump to subroutune
uint8_t cpu6502::JSR(){
    //puts the pc one instruction before
    prgrmCtnr--;
    //write the high byte of the program counter to the stack
    write(0x0100 + stckPtr, (prgrmCtnr >> 8) & 0x00FF);
    stckPtr--;
    //write the low byte of the program conter to the stacks
    write(0x0100 + stckPtr, prgrmCtnr & 0x00FF);
    stckPtr--;

    //jump to the address stored
    prgrmCtnr = addr_abs;
    return 0;
}

//Load the accumulator with a value
uint8_t cpu6502::LDA(){
    fetch();
    accReg = fetched;
    setFlag(Z, accReg == 0x00);
    setFlag(N, accReg & 0x80);
    return 1;
}

//Load the X reg with a value
uint8_t cpu6502::LDX(){
    fetch();
    xReg = fetched;
    setFlag(Z, xReg == 0x00);
    setFlag(N, xReg & 0x80);
    return 1;
}

//Load the Y reg with a value
uint8_t cpu6502::LDY(){
    fetch();
    yReg = fetched;
    setFlag(Z, yReg == 0x00);
    setFlag(N, yReg & 0x80);
    return 1;
}

//Logical Shift Right fetched data
uint8_t cpu6502::LSR(){
    fetch();
    //set the bit shifted away to the carry bit
    setFlag(C, fetched & 0x0001);
    //shift the bits to the right
    temp = fetched >> 1;
    //set the zero flag if the temp is 0
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    //set the negative flag
    setFlag(N, temp & 0x0080);
    //if we are in an implied mode, set the accumulator to the temp
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        accReg = temp & 0x00FF;
    else
    //else write the temp to the address
        write(addr_abs, temp & 0x00FF);
    return 0;
}


//TODO ADD MORE NOPs TO HERE
uint8_t cpu6502::NOP(){

    //// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
    switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


//LOGICAL OR OPERATION
uint8_t cpu6502::ORA(){
    fetch();
    //OR the accumulator with the fetched data
    accReg = accReg | fetched;
    //sets the zero flag
    setFlag(Z, accReg == 0x00);
    //sets the negative flag
    setFlag(N, accReg & 0x80);
    return 1;
}

uint8_t cpu6502::PHA(){
    //pushes the accumulator to the stack
    write(0x0100 + stckPtr, accReg);
    stckPtr--;
    return 0;
}

uint8_t cpu6502::PHP(){
    //pushes the status register to the stack
    write(0x0100 + stckPtr, statusReg);
    stckPtr--;
    return 0;
}

uint8_t cpu6502::PLA(){
    //pops the accumulator from the stack
    stckPtr++;
    accReg = read(0x0100 + stckPtr);
    setFlag(Z, accReg == 0x00);
    setFlag(N, accReg & 0x80);
    return 0;
}

uint8_t cpu6502::PLP(){
    //pops the status register from the stack
    stckPtr++;
    statusReg = read(0x0100 + stckPtr);
    setFlag(U, true);
    return 0;
}

//ROTATE LEFT A PIECE OF DATA
uint8_t cpu6502::ROL(){
    fetch();
    //rotate the bits to the left
    temp = (uint16_t)(fetched << 1) | getFlag(C);
    //sets the carry bit to the MSB
    setFlag(C, temp & 0xFF00);
    //sets the zero bit
    setFlag(Z, (temp & 0x00FF) == 0x0000);
    //sets the negative bit
    setFlag(N, temp & 0x0080);
    //if we are in an implied mode, set the accumulator to the temp
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        accReg = temp & 0x00FF;
    else
    //else write the temp to the address
        write(addr_abs, temp & 0x00FF);
    return 0;
}

//RORATE A PIECE OF DATA TO THE RIGHT
uint8_t cpu6502::ROR(){
    fetch();
    //rotate the bits to the right
    temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
    //sets the carry bit to the LSB
    setFlag(C, fetched & 0x01);
    //sets the zero bit
    setFlag(Z, (temp & 0x00FF) == 0x00);
    //sets the negative bit
    setFlag(N, temp & 0x0080);
    //if we are in an implied mode, set the accumulator to the temp
    if (lookup[opcode].addrmode == &cpu6502::IMP)
        accReg = temp & 0x00FF;
    else
    //else write the temp to the address
        write(addr_abs, temp & 0x00FF);
    return 0;
}

//RETURN FROM INTERRIPT
uint8_t cpu6502::RTI(){
    //pops the status register from the stack
    stckPtr++;
    statusReg = read(0x0100 + stckPtr);
    statusReg &= ~B;
	statusReg &= ~U;

    //pops the program counter from the stack
    stckPtr++;
    prgrmCtnr = (uint16_t)read(0x0100 + stckPtr);
    stckPtr++;
    prgrmCtnr |= (uint16_t)read(0x0100 + stckPtr) << 8;
    return 0;
}

//RETURN FROM SUBROUTINE
uint8_t cpu6502::RTS(){
    //pops the program counter from the stack
    //low byte
    stckPtr++;
    prgrmCtnr = (uint16_t)read(0x0100 + stckPtr);
    //high byte
    stckPtr++;
    prgrmCtnr |= (uint16_t)read(0x0100 + stckPtr) << 8;

    //increments the program counter
    prgrmCtnr++;
    return 0;
}

//subtract with carry
//TODO UNDERSTAND THE OVERLOW BIT OPERTAION
uint8_t cpu6502::SBC()
{
	fetch();

	// Operating in 16-bit domain to capture carry out

	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// Notice this is exactly the same as addition from here!
	temp = (uint16_t)accReg + value + (uint16_t)getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, ((temp & 0x00FF) == 0));
	setFlag(V, (temp ^ (uint16_t)accReg) & (temp ^ value) & 0x0080);
	setFlag(N, temp & 0x0080);
	accReg = temp & 0x00FF;
	return 1;
}

//set the carry flag
uint8_t cpu6502::SEC()
{
    setFlag(C, true);
    return 0;
}

//set decimal mode flag(not even needed in this 6502 implementation)
uint8_t cpu6502::SED()
{
    setFlag(D, true);
    return 0;
}

//sets interupt flag
uint8_t cpu6502::SEI()
{
    setFlag(I, true);
    return 0;
}

//write accumalator to memory
uint8_t cpu6502::STA()
{
    write(addr_abs, accReg);
    return 0;
}

//write x register to memory
uint8_t cpu6502::STX()
{
    write(addr_abs, xReg);
    return 0;
}

//write y register to memory
uint8_t cpu6502::STY(){
    write(addr_abs, yReg);
    return 0;
}

//transfer accumalator to x register
uint8_t cpu6502::TAX(){
    xReg = accReg;
    setFlag(Z, xReg == 0x00);
    setFlag(N, xReg & 0x80);
    return 0;
}

//transfer accumalator to x register
uint8_t cpu6502::TAY(){
    yReg = accReg;
    setFlag(Z, yReg == 0x00);
    setFlag(N, yReg & 0x80);
    return 0;
}

//transfer stack pointer to x register
uint8_t cpu6502::TSX(){
    xReg = stckPtr;
    setFlag(Z, xReg == 0x00);
    setFlag(N, xReg & 0x80);
    return 0;
}

//transfer x register to accumalator
uint8_t cpu6502::TXA(){
    accReg = xReg;
    setFlag(Z, accReg == 0x00);
    setFlag(N, accReg & 0x80);
    return 0;
}

//transfer x register to stack pointer
uint8_t cpu6502::TXS(){
    stckPtr = xReg;
    return 0;
}

//transfer y register to accumalator
uint8_t cpu6502::TYA(){
    accReg = yReg;
    setFlag(Z, accReg == 0x00);
    setFlag(N, accReg & 0x80);
    return 0;
}

//capture the illegal opcodes
uint8_t cpu6502::XXX(){
    return 0;
}


///HELPER FUNCTIONS FOR TESTING


// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &cpu6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IMM)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZP0)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZPX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZPY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IZX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IZY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABS)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABX)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABY)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IND)
		{
			lo = bus->cpuRead(addr, true); addr++;
			hi = bus->cpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::REL)
		{
			value = bus->cpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
