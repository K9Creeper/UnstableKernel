

.set MBOOT2_MAGIC, 0xE85250D6
.set MBOOT2_ARCH, 0
.set MBOOT2_LENGTH, (Multiboot2HeaderEnd - Multiboot2Header)
.set MBOOT2_CHECKSUM, -(MBOOT2_MAGIC + MBOOT2_ARCH + MBOOT2_LENGTH)

.section .multiboot.data, "aw"
.align 8

Multiboot2Header:
.long MBOOT2_MAGIC
.long MBOOT2_ARCH
.long MBOOT2_LENGTH
.long MBOOT2_CHECKSUM

.align 8

.short 0
.short 0
.short 8
Multiboot2HeaderEnd:
