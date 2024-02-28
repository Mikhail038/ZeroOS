#include "mouse.hpp"

#define START_OFFSET 0

MouseDriver::MouseDriver(InterruptManager* interrupt_manager_, Display& display_) :
    InterruptHandler(0x2C, interrupt_manager_),
    display(display_),
    data_port(0x60),
    command_port(0x64),
    offset(START_OFFSET), // start value may change
    buttons(0),
    last_mouse_cursor_cell(display[display.mouse_y][display.mouse_x])
{
    display[display.mouse_y][display.mouse_x].set_bg_colour(red);
    display[display.mouse_y][display.mouse_x].set_fg_colour(black);

    command_port.write(0xA8); //activate
    command_port.write(0x20); //get current state

    uint8_t status = data_port.read() | 2;

    command_port.write(0x60); //set state
    data_port.write(status);

    command_port.write(0xD4);
    data_port.write(0xF4);

    data_port.read();
}

uint32_t MouseDriver::handle_interrupt(uint32_t esp)
{
    uint8_t status = data_port.read();
    if (!(status & 0x20))
    {
        return esp;
    }

    buffer[offset] = data_port.read();
    offset = (offset + 1) % 3;

    if (offset == START_OFFSET)
    {
        // int8_t dif_x = buffer[1];
        // int8_t dif_y = buffer[2]; // is reversed (1 = -1)

        // if ((dif_x != 0) && (dif_y != 0)) //any move was done
        // {
        //     if (!(((dif_x > 0) && (display.mouse_x == display.width - 1)) || 
        //           ((dif_x < 0) && (display.mouse_x == 0)))) //not left or right out-of-border move
        //     {
        //         if (!(((dif_y < 0) && (display.mouse_y == display.height - 1)) || 
        //               ((dif_y > 0) && (display.mouse_y == 0)))) //not up or down out-of-border move
        //         {
        //             display[display.mouse_x ][display.mouse_y] = last_mouse_cursor_cell;            

                    display.mouse_x += buffer[1];
                    display.mouse_y -= buffer[2]; // -= cause dif_y is reversed   


                    // last_mouse_cursor_cell = display[display.mouse_y][display.mouse_x];

                    display[display.mouse_y][display.mouse_x].set_bg_colour(red);
                    display[display.mouse_y][display.mouse_x].set_fg_colour(black);
        //         }
        //     }
        // }
    }

    return esp;
}
