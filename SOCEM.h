#ifndef _SOCEM_
#define _SOCEM_

#include "stm32f4xx.h"
#include <stdio.h>

namespace SOCEM {
	
	//Human readable PORT_NAME type
	enum PORT_NAME {PORTA=0, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG};	
	enum OUTPUT_TYPE {PUSH_PULL=0, OPEN_DRAIN};
	enum PIN_STATE {OFF=0, ON=1, HIZ=-1};
	enum PIN_SPEED {LOW=0, MEDIUM=1, HIGH=2, VHIGH=3};
	enum PIN_PUPD {NONE=0, PULL_UP, PULL_DOWN, RESERVED};

	static const uint16_t RCC_ENABLE_BIT[] = {
				RCC_AHB1ENR_GPIOAEN, RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOAEN, RCC_AHB1ENR_GPIODEN,
				RCC_AHB1ENR_GPIOEEN, RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOGEN
	};

	static GPIO_TypeDef* GPIOType[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG };

	class STM32F429ZI {
		public:
		
	};

	class GPIOPort : public STM32F429ZI {
	public:

	protected:
		GPIO_TypeDef* which;
		enum PINMODE { DIGITAL_INPUT=0, DIGITAL_OUTPUT=1, ALTERNATE_FUNCTION=2, ANALOG=3 };
		
	protected:
		uint16_t rcc_enable_bit(PORT_NAME pn) {
				return RCC_ENABLE_BIT[pn];
		}
		
	public:
		GPIOPort(PORT_NAME port) {
				RCC->AHB1ENR |= this->rcc_enable_bit(port);
				switch (port) {
					case PORTA:
					case PORTB:
					case PORTC:					
					case PORTD:					
					case PORTE:					
					case PORTF:					
					case PORTG:
							this->which = GPIOType[port];
							break;
					default:
							this->which = NULL;
							break;
				}
		}
	};


	class DigitalOut : public GPIOPort {
	protected:
		uint8_t bit;
	public:
		DigitalOut(PORT_NAME port, uint8_t bit, PIN_STATE initialValue = OFF) : GPIOPort(port), bit(bit) {
				//Set mode to GPIO output			
				this->setPinMode(DIGITAL_OUTPUT);
		
				//Output Mode
				this->setOutputType(PUSH_PULL);
			
				//Set initial value
				this->setOutputData(initialValue);

				//Initial output speed
				this->setOutputSpeed(LOW);
				
				//Pull-up/down resistors
				this->setPullUpDownResistor(NONE);
				
		}
		
		void setPinMode(PINMODE mode) {	
			uint32_t modeBits = mode << (this->bit << 1);
			uint32_t bitMask = 3 << (this->bit << 1);
			this->which->MODER = (this->which->MODER & ~bitMask) | modeBits;
		}
		
		void setOutputType(OUTPUT_TYPE t) {
			uint32_t msk = (1 << bit);	
			switch (t) {
					case PUSH_PULL:
						this->which->OTYPER &= ~msk;
						break;
					case OPEN_DRAIN:
						this->which->OTYPER |= msk;
						break;
				}
		}
		
		void setOutputData(PIN_STATE s) {
			uint32_t msk = (1 << bit);
			switch (s) {
				case OFF:
				case HIZ:
						this->which->ODR &= ~msk;
						break;
				case ON:
						this->which->ODR |= msk;
						break;
			}
		}
		
		void setOutputSpeed(PIN_SPEED s) {
				uint32_t bitMask = 3 << (bit << 1);
				uint32_t bits    = s << (bit << 1);
				this->which->OSPEEDR = (this->which->OSPEEDR & ~bitMask) | bits;
		}
		
		void setPullUpDownResistor(PIN_PUPD r) {
				uint32_t bitMask = 3 << (bit << 1);
				uint32_t bits = r << (bit << 1);
				this->which->PUPDR = (this->which->PUPDR  & ~bitMask) | bits;
		}
		
		void operator =(uint8_t state) {
			if (state == 0) {
				this->which->BSRR |= 0x00010000 << this->bit;
			} else {
				this->which->BSRR |= 1 << this->bit;
			}
		}
	};

	void delay() {
			//This is horrible
			for (unsigned long x=0; x<1000000L; x++);
	}
}
#endif
