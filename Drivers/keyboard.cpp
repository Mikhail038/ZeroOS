#include "keyboard.hpp"

KeyboardDriver::KeyboardDriver(InterruptManager* interrupt_manager_, Display& display_) :
    InterruptHandler(0x21, interrupt_manager_),
    display(display_),
    data_port(0x60),
    command_port(0x64)
{
    while (command_port.read() & 0x1)
    {
        data_port.read();
    }

    command_port.write(0xAE); //activate
    command_port.write(0x20); //get current state

    uint8_t status = (data_port.read() | 1) & ~0x10;

    command_port.write(0x60); //set state
    data_port.write(status);

    data_port.write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{

}

uint32_t KeyboardDriver::handle_interrupt(uint32_t esp)
{
    const uint8_t FIRST_KEY_RELEASE = 0x80;

    uint8_t key = data_port.read();

    if (key < FIRST_KEY_RELEASE)
    {
        switch (key)
        {
            case 0xFA:
                break;

            case 0x1E:
                display.print('a');
                break;

            case 0x26:
                display.print('l');
                break;

            case 0x12:
                display.print('e');
                break;

            case 0x23:
                display.print('h');
                break;

            case 0x39:
                display.print(' ');
                break;

            case 0x18:
                display.print('o');
                break;

            case 0x1C:
                display.print('\n');
                break;

            case 0x4F:
                display.set_all_char(' ');
                display.set_start_cursor();
                break;
                
            default:
                leha_loh_print_key(key);
                // hex_print_key(key);
                // display.print(key); 
        }
    }

    return esp;
}

void KeyboardDriver::hex_print_key(uint8_t key)
{
    char* foo = "00 ";
    char* hex = "0123456789ABCDEF";

    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0F];

    display.print_line(foo);
}

void KeyboardDriver::leha_loh_print_key(uint8_t key)
{
    display.print_line("LEHA LOH ");
}