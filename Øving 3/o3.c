#include "o3.h"
#include "gpio.h"
#include "systick.h"

/**************************************************************************//**
 * @brief Konverterer nummer til string 
 * Konverterer et nummer mellom 0 og 99 til string
 *****************************************************************************/
void int_to_string(char *timestamp, unsigned int offset, int i) {
    if (i > 99) {
        timestamp[offset]   = '9';
        timestamp[offset+1] = '9';
        return;
    }

    while (i > 0) {
	    if (i >= 10) {
		    i -= 10;
		    timestamp[offset]++;
		
	    } else {
		    timestamp[offset+1] = '0' + i;
		    i=0;
	    }
    }
}

/**************************************************************************//**
 * @brief Konverterer 3 tall til en timestamp-string
 * timestamp-argumentet må være et array med plass til (minst) 7 elementer.
 * Det kan deklareres i funksjonen som kaller som "char timestamp[7];"
 * Kallet blir dermed:
 * char timestamp[7];
 * time_to_string(timestamp, h, m, s);
 *****************************************************************************/


void time_to_string(char *timestamp, int h, int m, int s) {
    timestamp[0] = '0';
    timestamp[1] = '0';
    timestamp[2] = '0';
    timestamp[3] = '0';
    timestamp[4] = '0';
    timestamp[5] = '0';
    timestamp[6] = '\0';

    int_to_string(timestamp, 0, h);
    int_to_string(timestamp, 2, m);
    int_to_string(timestamp, 4, s);
}

void setup_gpio_port(port_pin_t p,int mode, int h_l){
	if(h_l){
		volatile gpio_map_t* start=(gpio_map_t*)GPIO_BASE;
		//int reg=start->ports[p.port].MODEH;
		//start->ports[p.port].MODEH=((~(0b1111<<((p.pin-8)*4))&start->ports[p.port].MODEH)|(mode <<((p.pin-8)*4)));
		start->ports[p.port].MODEH=((~(0b1111<<((p.pin-8)*4))& (start->ports[p.port].MODEH))|(mode <<((p.pin-8)*4)));

	}
	else{
		volatile gpio_map_t* start=(gpio_map_t*)GPIO_BASE;
		start->ports[p.port].MODEL=((~(0b1111<<((p.pin)*4))& (start->ports[p.port].MODEL))|(mode <<((p.pin)*4)));
	}
}

void IRQ_setup(port_pin_t p){
	volatile gpio_map_t* start =(gpio_map_t*)GPIO_BASE;
	start->IFC=(0b1<<p.pin);

	//int extipsel=start->EXTIPSELH;
	start->EXTIPSELH=((~(0b1111<<((p.pin-8)*4)) & (start->EXTIPSELH))|( p.port << ((p.pin-8)*4)));
	//int extifall=start->EXTIFALL;
	start->EXTIFALL=((start->EXTIFALL)|(0b1<<p.pin));
	//int ien=start->IEN;
	//start->IFC=(0b1<<p.pin);
	//ien=(ien|(0b1<<p.pin));
	start->IEN= ((start->IEN)|(1<<p.pin));
}
void GPIO_ODD_IRQHandler(void){
volatile gpio_map_t* start =(gpio_map_t*)GPIO_BASE;
switch (tilstand) {
case 1:
	count_sekunder+=1;

	break;
case 2:
	count_minutter+=1;
	break;
case 3:
	count_timer +=1;
	break;

default:
	//sekunder+=1;
	//start->ports[GPIO_PORT_E].DOUTSET=1<<(2);
	break;
}
start->IFC=0b1<<9;
}
void GPIO_EVEN_IRQHandler(void){
volatile gpio_map_t* start =(gpio_map_t*)GPIO_BASE;
	switch(tilstand){
	case 1:
		tilstand+=1;
		break;
	case 2:
		tilstand+=1;
		break;
	case 3:
		tilstand+=1;
		break;
	case 4:




		break;
	case 5:
		start->ports[GPIO_PORT_E].DOUTCLR=1<<(2);
		tilstand=1;



	}
	start->IFC=0b1<<10;
}

void systick_setup(void){
volatile systick_map* start=(systick_map*)SYSTICK_BASE;
	start->CTRL=(start->CTRL|0b111);
	start->LOAD=FREQUENCY;
	start->VAL=0;

}

void SysTick_Handler(void){
volatile gpio_map_t* start =(gpio_map_t*)GPIO_BASE;
if(tilstand==4){
if(count_sekunder!=0){
count_sekunder-=1;
}
if(count_sekunder==0){
	if(count_minutter!=0){
		count_sekunder=59;
		count_minutter-=1;
	}

	if(count_minutter==0){


		if(count_timer==0 && count_sekunder==0){
		start->ports[GPIO_PORT_E].DOUTSET=1<<(2);
		tilstand=5;
	}
		if(count_timer!=0){
			count_minutter=59;
			count_timer-=1;
		}
	}

}
}
}
int main(void) {
    init();
    tilstand=1;
    count_sekunder=0;
    count_minutter=0;
    count_timer=0;
    char timestamp[7];
    port_pin_t led0={GPIO_PORT_E,2};
    //port_pin_t led1={GPIO_PORT_E,3};
    port_pin_t pb0={GPIO_PORT_B,9};
    port_pin_t pb1={GPIO_PORT_B,10};
    //volatile gpio_map_t* start =(gpio_map_t*)GPIO_BASE;


    setup_gpio_port(pb0,GPIO_MODE_INPUT,1);
    setup_gpio_port(led0,GPIO_MODE_OUTPUT,0);
    setup_gpio_port(pb1,GPIO_MODE_INPUT,1);
    //start->ports[GPIO_PORT_E].DOUTSET=1<<(2);
    IRQ_setup(pb0);
    IRQ_setup(pb1);
    systick_setup();
    //lcd_write("HEI");
    // Skriv din kode her...
    while (1){
    	//start->ports[GPIO_PORT_E].DOUTSET=1<<(2);

    	time_to_string(timestamp,count_timer,count_minutter,count_sekunder);
    	lcd_write(timestamp);

    }

    return 0;
}

