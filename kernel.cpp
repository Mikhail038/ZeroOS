#include "types.hpp"
#include "gdt.hpp"

#define VRAM_ADDR 0xB8000
#define HIGH_BYTES 0xFF00
 
void putline(int8_t* line)
{
    uint16_t* video_memory_entry = (uint16_t*) VRAM_ADDR;

    for (uint32_t cnt = 0; line[cnt] != '\0'; ++cnt)
    {
        video_memory_entry[cnt] = (video_memory_entry[cnt] & HIGH_BYTES) | line[cnt]; //save high bytes (color info)
    }
}

void putline(const int8_t* line)
{ 
    uint16_t* video_memory_entry = (uint16_t*) VRAM_ADDR;

    for (uint32_t cnt = 0; line[cnt] != '\0'; ++cnt)
    {
        video_memory_entry[cnt] = (video_memory_entry[cnt] & HIGH_BYTES) | line[cnt]; //save high bytes (color info)
    }
}

#undef VRAM_ADDR
#undef HIGH_BYTES

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
    putline("Hello User! ===ZOS=== Mikhail038, Dash8f");

    GlobalDescriptorTable gdt;

    while (true);
}

