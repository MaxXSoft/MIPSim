    lui $a0, 0xbfc0
    # NOTE:
    # '80' is a hard coded number
    # because there are 20 instructions here
    # each of which is 4 bytes long
    addi $a0, $a0, 80
    jal print
    nop
    jal halt
    nop

print:
    lui $t8, 0xbfd0
    move $t0, $a0
print_char:
    lb $t1, 0x0($t0)
    beq $t1, $0, print_end
    addi $t0, $t0, 1
    j print_char
    sb $t1, 0x5104($t8)
print_end:
    jr $ra
    nop

halt:
    lui $t8, 0xbfd0
    ori $t0, $0, 1
    sb $t0, 0x5100($t8)
halt_loop:
    j halt_loop
    nop

data:
    # 'Hello world!\n'
