OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
  . = 0x10000;
  .rela.plt       : { *(.rela.plt) *(.rela.iplt) }
  . = 0x18000;
  .plt            : { *(.plt) *(.iplt) }
  . = 0x20000;
  .text           : { *(.text) }
  . = 0x28000;
  .got            : { *(.got) *(.got.plt) }
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
