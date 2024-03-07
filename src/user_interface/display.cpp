#include <common/types.hpp>
#include <user_interface/display.hpp>

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

uint8_t Cell::get_bg_colour() const
{
    return colour >> 4;
}

uint8_t Cell::get_fg_colour() const
{
    return (colour & 0x0F) ;
}

uint8_t Cell::get_char() const
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

//----------------------------------------------------------------------------------------------------------------------------------

Display::Display(Colour bg_colour_, Colour fg_colour_, uint8_t character_) : 
    bg_colour(bg_colour_), 
    fg_colour(fg_colour_),
    character(character_), 
    cur_x(0), cur_y(0)
{
    screen_buffer = (Line*) video_ram_address;

    for (uint16_t y = 0; y != height; ++y)
    {
        screen_buffer[y].set_bg_colour(bg_colour);
        screen_buffer[y].set_fg_colour(fg_colour);
        screen_buffer[y].set_char(character);
    }

    move_currsor(0, 0);
};

Display::Display(Cell cell) : 
cur_x(0), cur_y(0)
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
    //cur_x = 0;
    //cur_y = 0;

    move_currsor(0, 0);
}

void Display::print(int8_t character_)
{

    if (cur_x == width - 1)
    {   
        screen_buffer[cur_y][cur_x].set_char(character_);

        move_currsor(cur_y + 1, 0);

        return;
    }

    if (character_ == '\n')
    {
        move_currsor(cur_y + 1, 0);

        return;
    }

    screen_buffer[cur_y][cur_x].set_char(character_);

    incr_currsor_x();
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
    if (cur_x == 0)
    {
        if (cur_y == 0)
        {
            print(' ');

            set_start_cursor();

            return;
        }

        if (cur_y > 0)
        {
            print(' ');

            move_currsor(cur_y - 1, width - 1);

            go_back_untill_char();

            return;
        }
    }

    if(screen_buffer[cur_y][cur_x].get_char() == ' ')
    {
        go_back_untill_char();
        incr_currsor_x();
    }

    screen_buffer[cur_y][cur_x].set_char(' ');

    if (cur_x > 0)
    {
        decr_currsor_x();
    }
    else
    {
        move_currsor(cur_y - 1, width - 1);
    }
}

void Display::ctrl_backspace()
{
    go_back_untill_char();

    while(screen_buffer[cur_y][cur_x].get_char() != ' ')
    {
        screen_buffer[cur_y][cur_x].set_char(' ');

        decr_currsor_x();
    }

    screen_buffer[cur_y][cur_x].set_char(' ');
}

void Display::go_back_untill_char()
{
    while ((cur_x != 0) && (screen_buffer[cur_y][cur_x].get_char() == ' '))
    {
        decr_currsor_x();
    }


}

void Display::move_currsor(int8_t new_cur_y, int8_t new_cur_x)
{
    if((new_cur_y == cur_y) && (new_cur_x == cur_x)) // no movement
    {
        return;
    }

    if(new_cur_x > width - 1)
    {
        cur_x = width - 1;
    }
    else if(new_cur_x < 0)
    {
        cur_x = 0;
    }
    else if(new_cur_y > height - 1)
    {
        cur_y = height - 1;
    }
    else if(new_cur_y < 0)
    {
        cur_y = 0;
    }
    else
    {
        if((cur_x == mouse_x) && (cur_y == mouse_y))
        {
            screen_buffer[cur_y][cur_x].set_bg_colour(red);
            screen_buffer[cur_y][cur_x].set_fg_colour(white);
        }
        else
        {
            screen_buffer[cur_y][cur_x].set_bg_colour(black);
            screen_buffer[cur_y][cur_x].set_fg_colour(white);
        }

        cur_x = new_cur_x;
        cur_y = new_cur_y;

        screen_buffer[cur_y][cur_x].set_bg_colour(white);
        screen_buffer[cur_y][cur_x].set_fg_colour(black);
    }

    if((cur_y == mouse_y) && (cur_x == mouse_x)) // move currsor to mouse
    {
        screen_buffer[mouse_y][mouse_x].set_bg_colour(red);
        screen_buffer[mouse_y][mouse_x].set_fg_colour(white);
    }
}

void Display::decr_currsor_x()
{
    move_currsor(cur_y, cur_x - 1);
}

void Display::incr_currsor_x()
{
    move_currsor(cur_y, cur_x + 1);
}

void Display::move_mouse(int8_t new_mouse_y, int8_t new_mouse_x)
{
    if(new_mouse_x > width - 1)
    {
        new_mouse_x = width - 1;
    }
    else if(new_mouse_x < 0)
    {
        new_mouse_x = 0;
    }
    if(new_mouse_y > height - 1)
    {
        new_mouse_y = height - 1;
    }
    else if(new_mouse_y < 0)
    {
        new_mouse_y = 0;
    }

    if((mouse_x == new_mouse_x) && (mouse_y == new_mouse_y))
    {
        return;
    }

    if((mouse_x == cur_x) && (mouse_y == cur_y))
    {
        screen_buffer[mouse_y][mouse_x].set_bg_colour(white);
        screen_buffer[mouse_y][mouse_x].set_fg_colour(black);
    }
    else
    {
        screen_buffer[mouse_y][mouse_x].set_bg_colour(black);
        screen_buffer[mouse_y][mouse_x].set_fg_colour(white);
    }

    mouse_x = new_mouse_x;
    mouse_y = new_mouse_y;

    screen_buffer[mouse_y][mouse_x].set_bg_colour(red);
    screen_buffer[mouse_y][mouse_x].set_fg_colour(white);
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

//----------------------------------------------------------------------------------------------------------------------------------
