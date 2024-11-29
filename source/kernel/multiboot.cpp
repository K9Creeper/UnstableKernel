#include "multiboot.hpp"

#define MAX_ARRAY_SIZES 128

extern "C" uint32_t kmalloc_(uint32_t size, uint8_t align, uint32_t *physAddress);

extern "C" char unused_(void *a, void *b) { return true; }

Kernel::Multiboot::ParsedMultibootInfo::~ParsedMultibootInfo()
{
    orderedArray_destroy(&modules);
    orderedArray_destroy(&mmap_entries);
}

Kernel::Multiboot::ParsedMultibootInfo Kernel::Multiboot::ParseTags(unsigned addr)
{
    ParsedMultibootInfo info;
    unsigned size = *reinterpret_cast<unsigned *>(addr);
    struct multiboot_tag *tag;

    info.modules = orderedArray_place(reinterpret_cast<void *>(kmalloc_(MAX_ARRAY_SIZES * sizeof(ModuleInfo), 0, 0)), MAX_ARRAY_SIZES, unused_);
    info.mmap_entries = orderedArray_place(reinterpret_cast<void *>(kmalloc_(MAX_ARRAY_SIZES * sizeof(MmapEntry), 0, 0)), MAX_ARRAY_SIZES, unused_);

    for (tag = reinterpret_cast<struct multiboot_tag *>(addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = reinterpret_cast<struct multiboot_tag *>(
             reinterpret_cast<multiboot_uint8_t *>(tag) + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_CMDLINE:
        {
            struct multiboot_tag_string *cmdline_tag = reinterpret_cast<struct multiboot_tag_string *>(tag);
            info.cmdline = cmdline_tag->string;
            break;
        }

        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        {
            struct multiboot_tag_string *bootloader_tag = reinterpret_cast<struct multiboot_tag_string *>(tag);
            info.bootloader_name = bootloader_tag->string;
            break;
        }

        case MULTIBOOT_TAG_TYPE_MODULE:
        {
            struct multiboot_tag_module *module_tag = reinterpret_cast<struct multiboot_tag_module *>(tag);

            ModuleInfo *module = reinterpret_cast<ModuleInfo *>(kmalloc_(sizeof(ModuleInfo), 0, 0));
            module->mod_start = module_tag->mod_start;
            module->mod_end = module_tag->mod_end;
            module->cmdline = module_tag->cmdline;

            orderedArray_insert(module, &info.modules);
            break;
        }

        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            struct multiboot_tag_basic_meminfo *meminfo_tag = reinterpret_cast<struct multiboot_tag_basic_meminfo *>(tag);
            info.mem_lower = meminfo_tag->mem_lower;
            info.mem_upper = meminfo_tag->mem_upper;
            break;
        }

        case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
            struct multiboot_tag_bootdev *bootdev_tag = reinterpret_cast<struct multiboot_tag_bootdev *>(tag);
            info.boot_device = bootdev_tag->biosdev;
            break;
        }

        case MULTIBOOT_TAG_TYPE_MMAP:
        {
            struct multiboot_tag_mmap *mmap_tag = reinterpret_cast<struct multiboot_tag_mmap *>(tag);
            multiboot_memory_map_t *mmap;
            for (mmap = mmap_tag->entries;
                 reinterpret_cast<multiboot_uint8_t *>(mmap) <
                 reinterpret_cast<multiboot_uint8_t *>(tag) + tag->size;
                 mmap = reinterpret_cast<multiboot_memory_map_t *>(
                     reinterpret_cast<unsigned long>(mmap) + mmap_tag->entry_size))
            {

                MmapEntry *entry = reinterpret_cast<MmapEntry *>(kmalloc_(sizeof(MmapEntry), 0, 0));
                entry->base_addr = mmap->addr;
                entry->length = mmap->len;
                entry->type = mmap->type;

                orderedArray_insert(entry, &info.mmap_entries);
            }
            break;
        }

        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        {
            struct multiboot_tag_framebuffer *fb_tag = reinterpret_cast<struct multiboot_tag_framebuffer *>(tag);
            info.framebuffer = {
                static_cast<uint32_t>(fb_tag->common.framebuffer_addr),
                fb_tag->common.framebuffer_width,
                fb_tag->common.framebuffer_height,
                fb_tag->common.framebuffer_bpp};
            break;
        }
        }
    }

    return info;
}
