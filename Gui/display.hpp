#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "types.hpp"

#define VRAM_ADDR 0xB8000
#define HIGH_BYTES 0xFF00

class Cell
{
public:
    enum Colour
    {
        black           = 0x0,
        blue            = 0x1,
        green           = 0x2,
        aqua            = 0x3,
        red             = 0x4,
        purple          = 0x5,
        yellow          = 0x6,
        white           = 0x7,
        gray            = 0x8,
        light_blue      = 0x9,
        light_green     = 0xA,
        light_aqua      = 0xB,
        light_red       = 0xC,
        light_purple    = 0xD,
        light_yellow    = 0xE,
        bright_white    = 0xF
    };

private:
    uint8_t character;    
    uint8_t colour;

public:
    Cell(uint8_t character_ = ' ', uint8_t colour_ = 0x0F);
    ~Cell();

    void set_bg_colour(Colour colour);
    void set_fg_colour(Colour colour);

    void set_char(uint8_t character);
} __attribute__((packed));

void putline(int8_t* line);
void putline(const int8_t* line);

#undef VRAM_ADDR
#undef HIGH_BYTES

#endif
