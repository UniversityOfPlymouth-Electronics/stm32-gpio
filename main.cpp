#include "SOCEM.h"
/*
    LED1        = PB_0,  // LD1 = GREEN
    LED2        = PB_7,  // Blue
    LED3        = PB_14, // Red
    LED4        = PB_0,
    USER_BUTTON = PC_13,
*/

using namespace SOCEM;

int main()
{
	SystemInit();
		
	//Set pin PB0 (LED1 - green) to output
	DigitalOut green(PORTB, 0, ON);
	
	//Set PB7 (LED2 Blue) to output
	DigitalOut blue(PORTB, 7, OFF);
	
	while (1) {
			blue = OFF;
			green = ON;
			delay();
			green = OFF;
			blue = ON;
			delay();
	}
}
