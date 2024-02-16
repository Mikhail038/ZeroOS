#include "types.hpp"

static const uint16_t width = 80; 

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

class Cell
{
public:
private:
    uint8_t character;    
    uint8_t colour;

public:
    Cell(uint8_t character_ = ' ', uint8_t colour_ = 0x0F);
    Cell(uint8_t character_ = ' ', Colour bg_colour_ = black, Colour fg_colour_ = white);

    ~Cell();

    void set_bg_colour(Colour colour);
    void set_fg_colour(Colour colour);
    void set_char(uint8_t character);

    uint8_t get_char();
    uint8_t get_bg_colour();
    uint8_t get_fg_colour();
} __attribute__((packed));

class Line
{
private:
    Cell cell_buffer[width];
    
public:
    Line();
    ~Line();

    void set_bg_colour(Colour colour_);
    void set_fg_colour(Colour colour_);
    void set_char(uint8_t character_);

    void set_decor_line(Cell left_cell, Cell mid_cell, Cell right_cell);
    void set_monochrome_decor_line(Colour colour_, uint8_t left_char, uint8_t mid_char, uint8_t right_char);

    Cell& operator[](uint16_t x);

    const Cell& operator[](uint16_t x) const;
};

class Display 
{
// private:
public:
    uint16_t** video_ram_address = (uint16_t**) 0xB8000;

    static const uint16_t width    = 80;
    static const uint16_t height   = 25;
    
    Colour bg_colour;
    Colour fg_colour;

    uint8_t character;

    uint16_t cur_x;
    uint16_t cur_y;

    Line* screen_buffer;

public:
    Display(Colour bg_colour_ = black, Colour fg_colour_ = white, uint8_t character_ = ' ');
    Display(Cell cell = {' ', black, white});

    ~Display();

    void set_bg_colour(Colour colour_);
    void set_fg_colour(Colour colour_);

    void set_all_bg_colour(Colour colour_);
    void set_all_fg_colour(Colour colour_);

    void print_welcome_z();

    void print(uint8_t character_);

    void set_line(uint16_t y, Cell cell);

    Line& operator[](uint16_t y);
    const Line& operator[](uint16_t y) const;
};

