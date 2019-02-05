    lui $s0, 0xbfd0
    lui $s1, 0x8500
    li $s2, 0x00000000
    li $s3, 0x00ffd700
draw_for0_begin:
    or $t0, $0, $0
draw_for0:
    slti $t2, $t0, 480
    beq $t2, $0, draw_for0_end
    nop
draw_for1_begin:
    or $t1, $0, $0
draw_for1:
    slti $t2, $t1, 640
    beq $t2, $0, draw_for1_end
    nop
    # calculate address
    ori $t2, $0, 640
    mult $t0, $t2
    mflo $t2
    add $t2, $t2, $t1
    sll $t2, $t2, 2
    add $t2, $t2, $s1
    # set pixel
    and $t3, $t0, $t1
    bne $t3, $0, draw_if
    sw $s2, 0x0($t2)
    j draw_if_end
    nop
draw_if:
    sw $s3, 0x0($t2)
draw_if_end:
    # next loop
    j draw_for1
    addi $t1, $t1, 1
draw_for1_end:
    j draw_for0
    addi $t0, $t0, 1
draw_for0_end:
    jal halt
    nop

halt:
    ori $t0, $0, 1
    sb $t0, 0x5100($s0)
halt_loop:
    j halt_loop
    nop
