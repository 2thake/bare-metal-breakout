# Example software running in Logisim RISC-V Computer System model by Pavel Gladyshev
# licensed under Creative Commons Attribution International license 4.0

    .text

    # void showpic(int picture[32]) - displays 32x32 picture filling the entire graphcis display
    #
    # input: a0 = starting address of the array with picture data (32 words) 
    #
    # This is an example of a function written in assembly language.
    # According to the RISC-V function calling convention (ilp32), CPU registers t0-t6 and a0-a7
    # can be modified by the function and do not need to be saved on the stack
    # as long as this function is not calling other functions.
    
    .globl showpic
    
showpic:
    li t0,0xffff8000    # starting address of the graphics display
    li t1,32            # number of lines on the display (each word encodes one line) 
loop:
    lw t2,0(a0)         # load next line (word) of the picture data
    sw t2,0(t0)         # write it to the corresponding line of the graphics display
    addi t0,t0,4        # move to the next line of the graphics display
    addi a0,a0,4        # move to the next line of the picture data
    addi t1,t1,-1       # reduce the number of remaining lines
    bnez t1,loop        # keep going until all lines on the display are filled with data
    jr ra               # return 
 
    .globl display_score
display_score:
li t0, 0xffff0010	# address of the least significant digit of 7-seg display
li t3, 10		# load constant 10 for separating digits
display_loop:		# this part would loop through larger numbers if it more displays were available
rem t2, a0, t3		# find least signif. digit of number
beqz a0, end		# if there is no remainder to display, exit

	# look-up table for the digits 0-9
beqz t2, n0
li t1, 1
beq t2, t1, n1
li t1, 2
beq t2, t1, n2
li t1, 3
beq t2, t1, n3
li t1, 4
beq t2, t1, n4
li t1, 5
beq t2, t1, n5
li t1, 6
beq t2, t1, n6
li t1, 7
beq t2, t1, n7
li t1, 8
beq t2, t1, n8
li t1, 9
beq t2, t1, n9

j end 		# if an unexpected digit is receiver, exit

continue:	# after the 7-seg code has been found, put it in memory
sb t4, 0(t0)
div a0, a0, t3	# discard the least significant digit
addi t0, t0, 1	# move the memory address to the next 7-seg display
j display_loop	# repeat all

	# for every possible digit, load its 7-seg value and then continue the loop
n0:
li t4, 0x3F
j continue
n1:
li t4, 0x06
j continue
n2:
li t4, 0x5B
j continue
n3:
li t4, 0x4F
j continue
n4:
li t4, 0x66
j continue
n5:
li t4, 0x6D
j continue
n6:
li t4, 0x7D
j continue
n7:
li t4, 0x7
j continue
n8:
li t4, 0x7F
j continue
n9:
li t4, 0x6F
j continue

end:
jr ra

.globl start_button
start_button:
li t0, 0xffff0060	# address of start button
lb a0, 0(t0)		# load data at start button
jr ra			# return

.globl slider
slider:
li t0, 0xffff003C
lb a0, 0(t0)
jr ra
