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
    uint16_t* video_memory_entry = (uint16_t*) 0xB8004;

    Cell cell('Z', 0x42);

    cell.set_fg_colour(Cell::black);
    cell.set_bg_colour(Cell::white);

    cell.set_char('V');

    video_memory_entry[0] = *((uint16_t*) &cell);

    // putline("Hello User! ===ZOS=== Mikhail038, Dash8f");

    GlobalDescriptorTable gdt;

    while (true);
}

