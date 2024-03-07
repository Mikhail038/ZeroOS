#include <drivers/mouse.hpp>

#define START_OFFSET 2

MouseDriver::MouseDriver(InterruptManager* interrupt_manager_, Display& display_) :
    InterruptHandler(0x2C, interrupt_manager_),
    display(display_),
    data_port(0x60),
    command_port(0x64),
    offset(START_OFFSET), // start value may change
    buttons(0)
{}

void MouseDriver::activate()
{
    display.move_mouse(display.height / 2, display.width / 2);

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
    uint8_t status = command_port.read();
        if (!(status & 0x20))
            return esp;

        buffer[offset] = data_port.read();
        offset = (offset + 1) % 3;

        if(offset == START_OFFSET)
        {
            if(buffer[1] != 0 || buffer[2] != 0)
            {
                display.move_mouse(display.mouse_y - buffer[2], display.mouse_x + buffer[1]);
            }
        }
        return esp;
}
