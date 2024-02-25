#include "gdt.hpp"
#include "types.hpp"

#define GDT_SEGMENT_CODE_FLAGS 0x9A
/*
    Present = 1
    Privilege level = 0 (privilege level 0 is for kernel code)
    Executable = 1 (this is a code segment)
    Direction = 0
    Readable = 1 Combining all these bits gets us the value 1001 1010b, or 0x9a.
*/

#define GDT_SEGMENT_DATA_FLAGS 0x92
/*
    Present = 1
    Privilege level = 0 (privilege level 0 is for kernel code)
    Executable = 0 (this is a data segment)
    Conforming = 0
    Writable = 1 Combining all these bits get us the value 1001 0010b, or 0x92.
*/

GlobalDescriptorTable::GlobalDescriptorTable() :
    null_segment_selector   (0, 0, 0),
    unused_segment_selector (0, 0, 0),
    code_segment_selector   (0, 64 * Mb, GDT_SEGMENT_CODE_FLAGS),
    data_segment_selector   (0, 64 * Mb, GDT_SEGMENT_DATA_FLAGS)
{
    uint32_t header[2];

    // header[0] = (uint32_t) this;
    // header[1] = sizeof(GlobalDescriptorTable) << 10;

    header[1] = (uint32_t) this;
    header[0] = sizeof(GlobalDescriptorTable) << 16;

    asm volatile("lgdt (%0)": :"p" (((uint8_t*) header) + 2));
}


uint16_t GlobalDescriptorTable::get_data_segment_selector() const
{
    return (uint8_t*) &data_segment_selector - (uint8_t*) this;
}

uint16_t GlobalDescriptorTable::get_code_segment_selector() const
{
    return (uint8_t*) &code_segment_selector - (uint8_t*) this;
}

SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t* target = (uint8_t*) this;

    if (limit <= 64 * Kb)
    {
        target[6] = 0x40; //16-bit entry
    }
    else
    {
        if ((limit & 0xFFF) != 0xFFF)
        {
            limit = (limit >> 12) - 1;
        }
        else
        {
            limit = limit >> 12;
        }

        target[6] = 0xC0; //not 16-bit mode
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    target[2] = (base >> 0)  & 0xFF;
    target[3] = (base >> 8)  & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flags;
}

uint32_t SegmentDescriptor::get_base() const
{
    uint8_t* target = (uint8_t*) this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t SegmentDescriptor::get_limit() const
{
    uint8_t* target = (uint8_t*) this;

    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0)
    {
        result = (result << 12) | 0xFFF;
    }

    return result;
}

#undef GDT_SEGMENT_DATA_FLAGS
#undef GDT_SEGMENT_CODE_FLAGS

