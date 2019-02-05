main:
    # read input
    jal read_num
    lui $s7, 0xbfd0
    # calculate
    move $s0, $v0
    or $s1, $0, $0
    ori $s2, $0, 1
main_l0:
    beq $s0, $0, main_l0_end
    addiu $s0, $s0, 0xffff
    add $s1, $s1, $s2
    move $t0, $s1
    move $s1, $s2
    move $s2, $t0
    # print
    jal print_num
    move $a0, $s1
    ori $t0, $0, 0x20
    j main_l0
    sb $t0, 0x5104($s7)
main_l0_end:
    ori $t0, $0, 0x0a
    jal halt
    sb $t0, 0x5104($s7)

read_num:
    ori $t0, $0, 0x0a
    or $t1, $0, $0
read_num_l0:
    lb $t2, 0x5104($s7)
    beq $t2, $t0, read_num_l0_end
    addiu $t2, $t2, 0xffd0
    multu $t0, $t1
    mflo $t1
    j read_num_l0
    add $t1, $t1, $t2
read_num_l0_end:
    jr $ra
    move $v0, $t1

print_num:
    or $t0, $0, $0
    ori $t1, $0, 10
print_num_l0:
    divu $a0, $t1
    mfhi $t2
    mflo $a0
    sw $t2, 0x0($t0)
    bne $a0, $0, print_num_l0
    addi $t0, $t0, 4
print_num_l1:
    addiu $t0, $t0, 0xfffc
    lw $t1, 0x0($t0)
    addi $t1, $t1, 0x30
    bne $t0, $0, print_num_l1
    sb $t1, 0x5104($s7)
    jr $ra
    nop

halt:
    ori $t0, $0, 1
    sb $t0, 0x5100($s7)
halt_loop:
    j halt_loop
    nop
