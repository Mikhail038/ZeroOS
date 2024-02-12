void putline(char* line)
{
    unsigned short* video_memory_entry = (unsigned short*) 0xB8000;

    for (int cnt = 0; line[cnt] != '\0'; ++cnt)
    {
        video_memory_entry[cnt] = (video_memory_entry[cnt] & 0xFF00) | line[cnt]; //save high bytes (color info)
    }

}

extern "C" void kernel_main(void* multiboot_struture, unsigned int magic_number)
{
    putline("Hello User! ===ZOS=== Mikhail038, Dash8f");

    while (1);
}

