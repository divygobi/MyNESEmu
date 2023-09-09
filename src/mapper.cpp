#include "Mapper.h"

mapper::mapper(uint8_t prgBanks, uint8_t chrBanks)
{
	nPRGBanks = prgBanks;
	nCHRBanks = chrBanks;
}


mapper::~mapper()
{
}