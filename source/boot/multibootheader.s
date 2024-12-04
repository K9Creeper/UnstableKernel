

.set MBOOT2_MAGIC, 0xE85250D6
.set MBOOT2_ARCH, 0
.set MBOOT2_LENGTH, (Multiboot2HeaderEnd - Multiboot2Header)
.set MBOOT2_CHECKSUM, -(MBOOT2_MAGIC + MBOOT2_ARCH + MBOOT2_LENGTH)

.section .multiboot
.align 8

Multiboot2Header:
.long MBOOT2_MAGIC
.long MBOOT2_ARCH
.long MBOOT2_LENGTH
.long MBOOT2_CHECKSUM

framebuffer_tag_start:
.short 5
.short 1
.long framebuffer_tag_end - framebuffer_tag_start
.long 320
.long 200
.long 8
framebuffer_tag_end:

.align 8

.short 0
.short 0
.short 8
Multiboot2HeaderEnd:
