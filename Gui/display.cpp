#include "types.hpp"
#include "display.hpp"

//----------------------------------------------------------------------------------------------------------------------------------

Cell::Cell(uint8_t character_, uint8_t colour_) : character(character_), colour(colour_) {}

Cell::Cell(uint8_t character_, Colour bg_colour_, Colour fg_colour_) : character(character_)
{
    set_bg_colour(bg_colour_);
    set_fg_colour(fg_colour_);
}


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

uint8_t Cell::get_bg_colour()
{
    return colour >> 4;
}

uint8_t Cell::get_fg_colour()
{
    return (colour & 0x0F) ;
}

uint8_t Cell::get_char()
{
    return character;
}

//----------------------------------------------------------------------------------------------------------------------------------

void Line::set_bg_colour(Colour colour_)
{
    for (uint16_t x = 0; x != width; ++x)
    {
        cell_buffer[x].set_bg_colour(colour_);
    }
}

void Line::set_fg_colour(Colour colour_)
{
    for (uint16_t x = 0; x != width; ++x)
    {
        cell_buffer[x].set_fg_colour(colour_);
    }
}

void Line::set_char(uint8_t character_)
{
    for (uint16_t x = 0; x != width; ++x)
    {
        cell_buffer[x].set_char(character_);
    }
}

void Line::set_line_from_one(Cell cell)
{
    for (uint16_t x = 0; x != width; ++x)
    {
        cell_buffer[x] = cell;
    }
}

void Line::set_line_from_three(Cell left_cell, Cell mid_cell, Cell right_cell)
{
    cell_buffer[0] = left_cell;
    for (uint16_t x = 1; x != width - 1; ++x)
    {
        cell_buffer[x] = mid_cell;
    }
    cell_buffer[width-1] = right_cell;
}

void Line::set_line_from_three_one_colour(uint8_t colour_, uint8_t left_char, uint8_t mid_char, uint8_t right_char)
{
    set_line_from_three({left_char, colour_},{mid_char, colour_},{right_char, colour_});
}

Cell& Line::operator[](uint16_t x)
{
    return cell_buffer[x];
}

const Cell& Line::operator[](uint16_t x) const
{
    return cell_buffer[x];
}

//----------------------------------------------------------------------------------------------------------------------------------

Display::Display(Colour bg_colour_, Colour fg_colour_, uint8_t character_) : 
    bg_colour(bg_colour_), fg_colour(fg_colour_), character(character_), cur_x(0), cur_y(0)
{
    screen_buffer = (Line*) video_ram_address;

    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_bg_colour(bg_colour);
        screen_buffer[y].set_fg_colour(fg_colour);
        screen_buffer[y].set_char(character);
    }
};

Display::Display(Cell cell) : cur_x(0), cur_y(0)
{
    bg_colour = (Colour) cell.get_bg_colour();
    fg_colour = (Colour) cell.get_fg_colour();
    character = cell.get_char();

    screen_buffer = (Line*) video_ram_address;

    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_bg_colour(bg_colour);
        screen_buffer[y].set_fg_colour(fg_colour);
        screen_buffer[y].set_char(character);
    }
}


void Display::set_bg_colour(Colour colour_)
{
    bg_colour = colour_;
}

void Display::set_fg_colour(Colour colour_)
{
    fg_colour = colour_;
}

void Display::set_all_bg_colour(Colour colour_)
{
    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_bg_colour(colour_);
    }
}

void Display::set_all_fg_colour(Colour colour_)
{
    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_fg_colour(colour_);
    }
}
    
void Display::set_all_char(uint8_t character_)
{
    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_char(character_);
    }
}

void Display::set_start_cursor()
{
    cur_x = 0;
    cur_y = 0;
}

void Display::print(int8_t character_)
{
    if (cur_x == width)
    {   
        cur_x = 0;
        ++cur_y;
    }

    if (character_ == '\n')
    {
        ++cur_y;
        cur_x = 0;

        return;
    }

    screen_buffer[cur_y][cur_x].set_char(character_);

    ++cur_x;
}

void Display::print_line(const int8_t* line)
{
    for (uint8_t cnt = 0; line[cnt] != '\0'; ++cnt)
    {
        print(line[cnt]);
    }
}

void Display::backspace()
{
    go_back_untill_char();

    if (cur_x == 0)
    {
        if (screen_buffer[cur_y][cur_x].get_char() != ' ')
        {
            print(' ');
            --cur_x;

            return;
        }

        if (cur_y > 0)
        {
            --cur_y;
            cur_x = width - 1;
            go_back_untill_char();

            return;
        }
    }

    print(' ');

    if (cur_x > 0)
    {
        --cur_x;
    }
}

void Display::go_back_untill_char()
{
    while ((cur_x != 0) && (screen_buffer[cur_y][cur_x].get_char() == ' '))
    {
        --cur_x;
    }
}


void Display::print_welcome_z()
{
    for (uint16_t y = 0; y != 8; ++y)
    {
        screen_buffer[y].set_bg_colour(white);
    }

    for (uint16_t y = 8; y != 16; ++y)
    {
        screen_buffer[y].set_bg_colour(blue);
    }

    for (uint16_t y = 16; y != 24; ++y)
    {
        screen_buffer[y].set_bg_colour(red);
    }
}

Line& Display::operator[](uint16_t y)
{
    return screen_buffer[y];
}

const Line& Display::operator[](uint16_t y) const
{
    return screen_buffer[y];
}

//----------------------------------------------------------------------------------------------------------------------------------
