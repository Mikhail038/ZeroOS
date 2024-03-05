#include "keyboard.hpp"

KeyboardDriver::KeyboardDriver(InterruptManager* interrupt_manager_, Display& display_) :
    InterruptHandler(0x21, interrupt_manager_),
    display(display_),
    data_port(0x60),
    command_port(0x64),
    shift_pressed(false),
    ctrl_pressed(false)
{}

void KeyboardDriver::activate()
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

uint32_t KeyboardDriver::handle_interrupt(uint32_t esp)
{
    uint8_t key = data_port.read();

    const uint8_t FIRST_KEY_RELEASE = 0x80;

    const int8_t* scan_codes_02_0D         = "1234567890-=";
    const int8_t* shift_scan_codes_02_0D   = "!@#$%^&*()_+";

    const int8_t* scan_codes_10_1B         = "qwertyuiop[]";
    const int8_t* shift_scan_codes_10_1B   = "QWERTYUIOP{}";

    const int8_t* scan_codes_1E_28         = "asdfghjkl;'";
    const int8_t* shift_scan_codes_1E_28   = "ASDFGHJKL:\"";

    const int8_t* scan_codes_2C_35         = "zxcvbnm,./";
    const int8_t* shift_scan_codes_2C_35   = "ZXCVBNM<>?";

    //hex_print_key(key);
    if (key < FIRST_KEY_RELEASE)
    {
        if ((key >= 0x02) && (key <= 0x0D))
        {
            if (shift_pressed)
            {
                display.print(shift_scan_codes_02_0D[key-0x02]);
                return esp;
            }
            
            display.print(scan_codes_02_0D[key-0x02]);
            return esp;
        }

        if ((key >= 0x10) && (key <= 0x1B))
        {
            if (shift_pressed)
            {
                display.print(shift_scan_codes_10_1B[key-0x10]);
                return esp;
            }
            
            display.print(scan_codes_10_1B[key-0x10]);
            return esp;
        }

        if ((key >= 0x1E) && (key <= 0x28))
        {
            if (shift_pressed)
            {
                display.print(shift_scan_codes_1E_28[key-0x1E]);
                return esp;
            }
            
            display.print(scan_codes_1E_28[key-0x1E]);
            return esp;
        }

        if ((key >= 0x2C) && (key <= 0x35))
        {
            if (shift_pressed)
            {
                display.print(shift_scan_codes_2C_35[key-0x2C]);
                return esp;
            }
            
            display.print(scan_codes_2C_35[key-0x2C]);
            return esp;
        }

        switch (key)
        {
                
            case 0xFA:
                break;

            case 0x0E: // backspace
                if(ctrl_pressed)
                {
                    display.ctrl_backspace();
                }
                else
                {
                    display.backspace();
                }
                break;

            case 0x1C: //Enter
                display.print('\n');
                break;

            case 0x2A: //Shift
                shift_pressed = true;
                break;

            case 0x1D: //Ctrl
                ctrl_pressed = true;
                break;

            case 0x3A: //CapsLock
                shift_pressed = (shift_pressed) ? false : true;
                break;

            case 0x39: //Space
                display.print(' ');
                break;

            case 0x52: // Insert
                display.set_all_fg_colour(red); // RED MODE
                break;
            
            case 0x45: // num lock
                //num_func();
                break;

            case 0x4B: // left
                if((display.cur_x == 0) && (display.cur_y != 0))
                {
                    display.move_currsor(display.cur_y - 1, display.width - 1);
                    break;
                }
                display.decr_currsor_x();
                break;

            case 0x48: // up
                display.move_currsor(display.cur_y - 1, display.cur_x);
                break;

            case 0x4D: // right
                if((display.cur_x == display.width - 1) && (display.cur_y != display.height - 1))
                {
                    display.move_currsor(display.cur_y + 1, 0);
                    break;
                }
                display.incr_currsor_x();
                break;

            case 0x50: // down
                display.move_currsor(display.cur_y + 1, display.cur_x);
                break;

            case 0x4F: //End
                display.set_all_char(' ');
                display.set_start_cursor();
                break;

            // case 0x47:
            //     display.move_mouse(display.mouse_y, display.mouse_x - 1);
            //     break;

            // case 0x49:
            //     display.move_mouse(display.mouse_y, display.mouse_x + 1);
            //     break;

            // case 0x4c:
            //     display.move_currsor(display.mouse_y, display.mouse_x);
            //     break;
                
            default:
                // leha_loh_print_key(key);
                hex_print_key(key);
                // display.print(key); 
        }
    }

    if (key == 0xAA) //Shift depressed
    {
        shift_pressed = (shift_pressed) ? false : true;
    }
    if (key == 0x9D) //Ctrl depressed
    {
        ctrl_pressed = false;
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