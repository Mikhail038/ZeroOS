#include "types.hpp"
#include "gdt.hpp"
#include "display.hpp"
#include "irq.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"

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
    Display display({' ', black, white});
    display.move_currsor(0,0);
    display.move_mouse(10, 20);

    display.print_line("Chek our github:\n");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    KeyboardDriver keyboard(&interrupts, display);
    //MouseDriver mouse(&interrupts, display);

    interrupts.activate();

    while (true);
}

