#include "types.hpp"
#include "display.hpp"

#define VRAM_ADDR 0xB8000
#define HIGH_BYTES 0xFF00


Cell::Cell(uint8_t character_, uint8_t colour_) : character(character_), colour(colour_) {};
Cell::~Cell() {};

void Cell::set_bg_colour(Colour colour_)
{
    colour = colour & 0x0F;
    colour += (colour_ << 4);
}

void Cell::set_fg_colour(Colour colour_)
{
    colour = colour & 0xF0;
    colour += colour_;
}

void Cell::set_char(uint8_t character_)
{
    character = character_;
}

#undef VRAM_ADDR
#undef HIGH_BYTES