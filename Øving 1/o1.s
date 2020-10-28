.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO

.text
	.global Start

Start:
ldr r1,=(PORT_E*PORT_SIZE)+GPIO_BASE+GPIO_PORT_DOUTCLR

ldr r3,=(PORT_E*PORT_SIZE)+GPIO_BASE+GPIO_PORT_DOUTSET

ldr r4, =GPIO_BASE+(PORT_SIZE*BUTTON_PORT)+GPIO_PORT_DIN


LOOP:
ldr r8,=1<<BUTTON_PIN
ldr r5,[r4]
eor r5,r5,r8
cmp r5,r8
beq turn_on

turn_off:
ldr r8,=1<<LED_PIN
str r8, [r1]
B LOOP
turn_on:
ldr r8, =1<<LED_PIN
str r8,[r3]
B LOOP

    // Skriv din kode her...


NOP // Behold denne på bunnen av fila
