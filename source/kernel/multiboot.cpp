#include "multiboot.hpp"

#define MAX_ARRAY_SIZES 128

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);

extern "C" char unused_(void *a, void *b) { return true; }

extern void printf(const char *format, ...);

Kernel::Multiboot::ParsedMultibootInfo::~ParsedMultibootInfo()
{
    orderedArray_destroy(&modules);
    orderedArray_destroy(&mmap_entries);
}

Kernel::Multiboot::ParsedMultibootInfo Kernel::Multiboot::ParseTags(uint32_t addr) {
    ParsedMultibootInfo info;

    return info;
}
