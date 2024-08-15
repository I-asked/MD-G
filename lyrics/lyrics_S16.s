.thumb
.thumb_func
.globl atrac_write_metadata
atrac_write_metadata:
  bx pc
  .word 0x7e9d1

.thumb
.thumb_func
.globl atrac_read_contentdata
atrac_read_contentdata:
  bx pc
  .word 0x7e661

.thumb
.thumb_func
.globl atrac_write_contentdata
atrac_write_contentdata:
  bx pc
  .word 0x7e5f1

.thumb
.thumb_func
.globl tron_dly_tsk
tron_dly_tsk:
  bx pc
  .word 0x7c201;
