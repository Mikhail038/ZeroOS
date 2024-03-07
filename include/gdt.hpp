#ifndef GDT_HPP
#define GDT_HPP

#include <common/types.hpp>

class SegmentDescriptor final
{
private:
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_high;
    uint8_t  type;
    uint8_t  flags_limit_high;
    uint8_t  base_vhigh;

public:
    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);

    uint32_t get_base()  const;
    uint32_t get_limit() const;

} __attribute__((packed));

class GlobalDescriptorTable final
{
public:
    SegmentDescriptor   null_segment_selector;
    SegmentDescriptor unused_segment_selector;
    SegmentDescriptor   code_segment_selector;
    SegmentDescriptor   data_segment_selector;

    GlobalDescriptorTable();
    ~GlobalDescriptorTable() = default;

    uint16_t get_code_segment_selector() const;
    uint16_t get_data_segment_selector() const;   
};

#endif