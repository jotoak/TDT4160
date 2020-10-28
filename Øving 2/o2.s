.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO
.include "sys-tick_constants.s" // Register-adresser og konstanter for SysTick

.text
	.global Start
	
Start:
	ldr r0, =SYSTICK_BASE
	ldr r1, =0b110
	str r1,[r0]

	ldr r0,=SYSTICK_BASE+SYSTICK_LOAD
	ldr r1,=1400000
	str r1,[r0]

	ldr r0, =SYSTICK_BASE+SYSTICK_VAL
	ldr r1, =0
	str r1,[r0]

	ldr r0, =GPIO_BASE+GPIO_IFC
	ldr r1, =0b1000000000
	ldr r2,[r0]
	orr r3,r1,r2
	str r3,[r0]

	ldr r0, =GPIO_BASE+GPIO_EXTIPSELH
	ldr r2,[r0]
	ldr r1,=0b00010000
	orr r3,r2,r1
	str r3,[r0]

	ldr r0, =GPIO_BASE+GPIO_EXTIFALL
	ldr r1, [r0]
	ldr r2, =0b1000000000
	orr r3, r2,r1
	str r3,[r0]

	ldr r0, =GPIO_BASE+GPIO_IEN
	ldr r1,[r0]
	ldr r2,=0b1000000000
	orr r3,r2,r1
	str r3,[r0]

Loop:
	wfi
	B Loop

.global GPIO_ODD_IRQHandler
.thumb_func




.global SysTick_Handler
.thumb_func
SysTick_Handler:

	ldr r0,=tenths
	ldr r1,[r0]
	cmp r1,9
	BEQ Tenths_reset
	ldr r2, =1
	add r3,r2,r1
	str r3,[r0]
	BX LR



Tenths_reset:
	ldr r0,=tenths
	ldr r1,=0
	str r1,[r0]
	ldr r0, =GPIO_BASE+(LED_PORT*PORT_SIZE)
	ldr r3, =1 << LED_PIN
	ldr r4, =GPIO_PORT_DOUTTGL
	add r5,r0,r4
	str r3, [r5]

	ldr r0, =seconds
	ldr r1, [r0]
	add r1,#1
	cmp r1, #59
	BEQ Minute_up
	str r1,[r0]

	BX LR
    // Skriv din kode her...
Minute_up:
	ldr r0,=minutes
	ldr r1,[r0]
	add r1,#1
	str r1,[r0]

	ldr r0,=seconds
	ldr r1,=0
	str r1,[r0]

	BX LR


.global GPIO_ODD_IRQHandler
.thumb_func
GPIO_ODD_IRQHandler:
	ldr r0, =SYSTICK_BASE
	ldr r1, =0b001
	ldr r2,[r0]
	eor r3,r2,r1
	str r3,[r0]


	ldr r0, =GPIO_BASE+GPIO_IFC
	ldr r1, =0b1000000000
	ldr r2,[r0]
	orr r3,r1,r2
	str r3,[r0]
	BX LR


NOP // Behold denne på bunnen av fila

