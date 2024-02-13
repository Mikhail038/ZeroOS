#include "types.hpp"

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

extern "C" void kernel_main(void* multiboot_struture, uint32_t magic_number)
{
    putline("Hello User! ===ZOS=== Mikhail038, Dash8f");

    while (true);
}

