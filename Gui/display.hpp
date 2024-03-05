#ifndef DISPLAY_HPP
#define DISPLAY_HPP


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

class Cell final
{
private:
    uint8_t character;    
    uint8_t colour;

public:
    Cell(uint8_t character_ = ' ', uint8_t colour_ = 0x0F); //TODO
    Cell(uint8_t character_ = ' ', Colour bg_colour_ = black, Colour fg_colour_ = white);

    ~Cell() = default;

    void set_bg_colour(Colour colour);
    void set_fg_colour(Colour colour);
    void set_char(uint8_t character);

    uint8_t get_char() const;
    uint8_t get_bg_colour() const;
    uint8_t get_fg_colour() const;

} __attribute__((packed));

class Line final
{
private:
    Cell cell_buffer[width];
    
public:
    Line() = default;
    ~Line() = default;

    void set_bg_colour(Colour colour_);
    void set_fg_colour(Colour colour_);
    void set_char(uint8_t character_);

    void set_line_from_one(Cell cell);
    void set_line_from_three(Cell left_cell, Cell mid_cell, Cell right_cell);
    void set_line_from_three_one_colour(uint8_t colour_, uint8_t left_char, uint8_t mid_char, uint8_t right_char); 

    Cell& operator[](uint16_t x);
};

class Display final
{
// private:
public:
    uint16_t** video_ram_address = (uint16_t**) 0xB8000;

    static const uint8_t width    = 80;
    static const uint8_t height   = 25;
    
    Colour bg_colour;
    Colour fg_colour;

    uint8_t character;

    uint8_t cur_x;
    uint8_t cur_y;

    int8_t mouse_x;
    int8_t mouse_y;

    Line* screen_buffer;

public:
    Display(Colour bg_colour_ = black, Colour fg_colour_ = white, uint8_t character_ = ' ');
    Display(Cell cell = {' ', black, white});

    ~Display() = default;

    void set_bg_colour(Colour colour_);
    void set_fg_colour(Colour colour_);

    void set_all_bg_colour(Colour colour_);
    void set_all_fg_colour(Colour colour_);
    void set_all_char(uint8_t character_);

    void set_start_cursor();

    void print_welcome_z();

    void print(int8_t character_);
    void print_line(const int8_t* line);

    void backspace();
    void ctrl_backspace();

    void go_back_untill_char();

    void move_currsor(int8_t new_cur_y, int8_t new_cur_x);
    void decr_currsor_x();
    void incr_currsor_x();

    void move_mouse(int8_t new_mouse_y, int8_t new_mouse_x);

    Line& operator[](uint16_t y);
};

#endif