MEMORY
{
  rom      (rx)  : ORIGIN = 0x00000000, LENGTH = 0x000A0000
  ram      (rwx) : ORIGIN = 0x02000000, LENGTH = 0x00009000
  hbl      (rwx) : ORIGIN = 0x02005500, LENGTH = 0x00001600
  app      (rwx) : ORIGIN = 0x02006B00, LENGTH = 0x00002600
}

SECTIONS
{
    .text :
    {
        *(.text.startup.start)
        *(.text*)
        *(.rodata*)
        *(.data*)
        *(.bss*)
    } > app
}
