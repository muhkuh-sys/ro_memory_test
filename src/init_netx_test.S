
	.section .init_code, "ax"
	.arm

	.extern test_main

	.global start
	.global stack_top

@--------------------------------------

  .func start_init_s

start:
	stmfd	sp!, {r0, r1, r2, lr}

	@ set own stack position
	ldr	r0, =stack_top
	mov	sp, r0

	@ copy code or data
	ldr	r0, =__copy_loadaddr__
	ldr	r1, =__copy_start__
	ldr	r2, =__copy_end__
copy:
	cmp	r1, r2
	ldrlo	r3, [r0], #4
	strlo	r3, [r1], #4
	blo	copy

	@ clear .bss section (zero init)
	mov	r0, #0
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__
clear_bss:
	cmp	r1, r2
	strlo	r0, [r1], #4
	blo	clear_bss

	ldr	r0, =test_main
	blx	r0

  .endfunc

@--------------------------------------

	.align 4

NetxTestParameter_Init:
	.word 0

@--------------------------------------

  .end

