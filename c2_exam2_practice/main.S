./LemonGrammar input1.txt
li $t0, 2
sw $t0, x
lw $t0, x
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts
li $t0, 0
sw $t0, i
sum:
addi $sp, $sp, -24
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
sw $ra, 16($sp)
sw $fp, 20($sp)
move $fp, $sp
lw $t0, a
lw $t1, b
add $t0, $t0, $t1
sw $t0, 4($fp)
lw $t0, x
move $v0, $t0

move $sp, $fp
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
lw $ra, 16($sp)
lw $fp, 20($sp)
addi $sp, $sp, 24
jr $ra
li $t0, 1

move $a0, $t0
li $t0, 2

move $a1, $t0
jal sum
sw $v0, z
lw $t0, z
move $a0, $t0
jal put_udecimal
la $a0, enter
jal puts

