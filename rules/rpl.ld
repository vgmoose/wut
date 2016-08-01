OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)

ENTRY(_start)

MEMORY {
   code     (rwx) : ORIGIN = 0x02000000, LENGTH = 224M
   data     (rw)  : ORIGIN = 0x10000000, LENGTH = 800M
   fimp     (rwx) : ORIGIN = 0xC0000000, LENGTH = 224M
}

SECTIONS {
   . = ORIGIN(code);

   .text ALIGN(32) : {
      *(.text .text.*)
   } > code

   . = ORIGIN(fimp);

   .fimport_coreinit ALIGN(32) : {
      *(.fimport_coreinit)
   } > fimp

   .fimport_proc_ui ALIGN(32) : {
      *(.fimport_proc_ui)
   } > fimp

   .fimport_vpad ALIGN(32) : {
      *(.fimport_vpad)
   } > fimp

   . = ORIGIN(data);

   .sdata2 ALIGN(32) : {
      *(.sdata2 .sdata2.*)
   } > data

   .rodata ALIGN(32) : {
      *(.rodata .rodata.*)
   } > data

   .data ALIGN(32) : {
      *(.data .data.*)
   } > data

   .bss ALIGN(32) : {
      *(.bss .bss.*)
   } > data

   .sdata ALIGN(32) : {
      *(.sdata .sdata.*)
   } > data

   .sbss ALIGN(32) : {
      *(.sbss .sbss.*)
	   *(.sbss2 .sbss2.*)
   } > data
}
