; Identical to lesson 13's boot sector, but the %included files have new paths
[org 0x7c00]
SECOND_OFFSET equ 0x8000

    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
    mov bp, 0x9000
    mov sp, bp
    call load_stage_two

%include "boot/print.asm"
%include "boot/hexprint.asm"
%include "boot/disk.asm"

[bits 16]
load_stage_two:
    mov bx, MSG_LOAD_STAGE
    call print
    call print_nl

    mov bx, SECOND_OFFSET ; Read from disk and store in 0x8000
    mov dh, 2 ; load 2 sectors for second stage
    mov dl, [BOOT_DRIVE]
    call disk_load
    call SECOND_OFFSET
    ret




BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_STAGE db "Loading Stage 2 into memory", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55