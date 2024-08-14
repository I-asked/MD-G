.thumb
.thumb_func
.globl strlen
strlen:
  bx pc
  .word 0x7cd25

.thumb
.thumb_func
.globl memcpy
memcpy:
  bx pc
  .word 0x7cb19

.thumb
.thumb_func
.globl memset
memset:
  bx pc
  .word 0x7cc45

.thumb
.thumb_func
.globl memcmp
memcmp:
  bx pc
  .word 0x7cce9

.thumb
.thumb_func
.globl strcat
strcat:
  bx pc
  .word 0x7cea1

.thumb
.thumb_func
.globl strcpy
strcpy:
  bx pc
  .word 0x7cecd

.thumb
.thumb_func
.globl strtol
strtol:
  bx pc
  .word 0x7d111

.thumb
.thumb_func
.globl strtoul
strtoul:
  bx pc
  .word 0x7daa1

.thumb
.thumb_func
.globl strncat
strncat:
  bx pc
  .word 0x7cdc1

.thumb
.thumb_func
.globl strncpy
strncpy:
  bx pc
  .word 0x7cd69

.thumb
.thumb_func
.globl atoi
atoi:
  bx pc
  .word 0x7cded

.thumb
.thumb_func
.globl atrac_read_metadata
atrac_read_metadata:
  bx pc
  .word 0x7ea3d

@.thumb
@.thumb_func
@.globl atrac_write_metadata
@atrac_write_metadata:
@  bx pc
@  .word 0x7e9d1
@
@.thumb
@.thumb_func
@.globl atrac_read_contentdata
@atrac_read_contentdata:
@  bx pc
@  .word 0x7e661
@
@.thumb
@.thumb_func
@.globl atrac_write_contentdata
@atrac_write_contentdata:
@  bx pc
@  .word 0x7e5f1
@
@.thumb
@.thumb_func
@.globl tron_dly_tsk
@tron_dly_tsk:
@  bx pc
@  .word 0x7c201;
