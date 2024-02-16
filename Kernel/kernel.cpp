#include "types.hpp"
#include "gdt.hpp"
#include "display.hpp"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; ++i)
    {
        (*i)();
    }
}


extern "C" void kernel_main(void* multiboot_struture, uint32_t magic_number)
{
    GlobalDescriptorTable gdt;

    Display display({' ', black, white});

    display.print_welcome_z();

    display.set_all_fg_colour(black);

    display.print('L');
    display.print('E');
    display.print('H');
    display.print('A');
    display.print(' ');
    display.print('L');
    display.print('O');
    display.print('H');
    display.print('!');

    while (true);
}

