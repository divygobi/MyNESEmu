#include "../headers/ppu2C02.h"


ppu2C02::ppu2C02()
{
	palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);

	sprScreen = new olc::Sprite(256, 240);
	sprNameTable[0] = new olc::Sprite(256, 240);
	sprNameTable[1] = new olc::Sprite(256, 240);
	sprPatternTable[0] = new olc::Sprite(128, 128);
	sprPatternTable[1] = new olc::Sprite(128, 128);
}


ppu2C02::~ppu2C02()
{
	delete sprScreen;
	delete sprNameTable[0];
	delete sprNameTable[1];
	delete sprPatternTable[0];
	delete sprPatternTable[1];
}

olc::Sprite& ppu2C02::GetScreen()
{
	return *sprScreen;
}

olc::Sprite & ppu2C02::GetNameTable(uint8_t i)
{
	return *sprNameTable[i];
}

olc::Sprite & ppu2C02::GetPatternTable(uint8_t i)
{
	for(uint16_t nTileY = 0; nTileY < 16; nTileY++){
		for(uint16_t nTileX = 0; nTileX < 16; nTileX++){
			//a single type has 16 bytes of info, 
			//nOffset is the BYE offset of the tile
			uint16_t nOffset = nTileY * 256 + nTileX * 16;
			//8rows of 8 pixles
			for(uint16_t row = 0; row < 8; row++){
				//ppuRead reads form the pallete memory(cartridge)
				uint8_t tileLsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tileMsb = ppuRead(i * 0x1000 + nOffset + row + 0x1000);
				for(uint16_t col = 0; col < 8; col++){
					//get the color id
					uint8_t pixel = (tileLsb & 0x01) + (tileMsb & 0x01);
					//shift the tile to the right
					tileLsb >>= 1;
					tileMsb >>= 1;
					//set the color
					sprPatternTable[i]->SetPixel(nTileX * 8 + (7 - col), nTileY * 8 + row, GetColorFromPaletteRam(i, pixel));
				}
			}

		}
	}
}

olc::Pixel ppu2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	// This is a convenience function that takes a specified palette and pixel to get an index in the NES memory
	return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

//void ppu2C02::connectBus(Bus *b)
//{
  //  bus = b;
//}

//reading and writing to the cpu bus
void ppu2C02::cpuWrite(uint16_t addy, uint8_t data)
{
    switch (addy){
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;

    }
}

uint8_t ppu2C02::cpuRead(uint16_t addy, bool isReadOnly){

        uint8_t data = 0x00;

        switch (addy){
            case 0x0000: // Control
                break;
            case 0x0001: // Mask
                break;
            case 0x0002: // Status
                break;
            case 0x0003: // OAM Address
                break;
            case 0x0004: // OAM Data
                break;
            case 0x0005: // Scroll
                break;
            case 0x0006: // PPU Address
                break;
            case 0x0007: // PPU Data
                break;

        }
        return data;
}

//reading and writing to the ppus bus
uint8_t ppu2C02::ppuRead(uint16_t addy, bool isReadOnly){
    uint8_t data = 0x00;
    //mirror the data
    addy &= 0x3FFF;

	//in the cartridge space
    if(cart->ppuRead(addy, data)){

    }
	//pattern memory
	else if (addy >= 0x0000 && addy <= 0x1FFF){
		//first dim chooses whether its the left or right side of the data
		//second dim makses the remaining bits		
		data = tblPattern[(addy & 0x1000) >> 12][addy & 0x0FFF];

	}
	//nametable memory
	else if (addy >=0x0000 && addy <= 0x3EFF){ 
	
	}
	//palette memory
	else if (addy >= 0x3F00 && addy <= 0x3FFF){
		//masking bottom 5 bits
		addy &= 0x001F;
		//mirror addresses
		if (addy == 0x0010) addy = 0x0000;
		if (addy == 0x0014) addy = 0x0004;
		if (addy == 0x0018) addy = 0x0008;
		if (addy == 0x001C) addy = 0x000C;
		data = tblPalette[addy];
	}
    return data;
}

void ppu2C02::ppuWrite(uint16_t addy, uint8_t data){
    //mirror the data
    addy &= 0x3FFF;

    if(cart->ppuWrite(addy, data)){

    }
	//pattern memory
	else if (addy >= 0x0000 && addy <= 0x1FFF){
		tblPattern[(addy & 0x1000) >> 12][addy & 0x0FFF] = data;
	}
	//nametable memory
	else if (addy >=0x0000 && addy <= 0x3EFF){ 
	
	}
	//palette memory
	else if (addy >= 0x3F00 && addy <= 0x3FFF){
		//masking bottom 5 bits
		addy &= 0x001F;
		//mirror addresses
		if (addy == 0x0010) addy = 0x0000;
		if (addy == 0x0014) addy = 0x0004;
		if (addy == 0x0018) addy = 0x0008;
		if (addy == 0x001C) addy = 0x000C;
		tblPalette[addy] = data;
	}
}

void ppu2C02::connectCartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
}

void ppu2C02::clock(){

    // Fake some noise for now
	sprScreen->SetPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);

    //ALWAYS KEEPS GOING
    //MIMICKS THE HARDWARE
    cycle++;
    if (cycle >= 341)
	{
		cycle = 0;
		scanline++;
		if (scanline >= 261)
		{
			scanline = -1;
			frame_complete = true;
		}
	}
}
