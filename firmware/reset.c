/*
 * reset.c
 *
 * Created: 2/9/2014 11:24:16
 *  Author: Ben
 */ 

#include "keymatrix.h"
#include "kbd.h"
#include <avr/interrupt.h>
#include <util/delay.h>

static const char RESET_KEYS[] = {
	calcKeyIdx(0,2), calcKeyIdx(0,4), calcKeyIdx(0,5), calcKeyIdx(0,7)
};

static const char INHIBIT_KEYS[] = {
	calcKeyIdx(0,3), calcKeyIdx(0,6)
};

static char shouldReset()
{
	char NUM_RESET_KEYS = sizeof(RESET_KEYS)/sizeof(*RESET_KEYS);
	char NUM_INHIBIT_KEYS = sizeof(INHIBIT_KEYS)/sizeof(*INHIBIT_KEYS);
	int i;
	for(i=0; i<NUM_RESET_KEYS; i++)
	{
		if(!isKeyDown(RESET_KEYS[i]))
		{
			return 0;
		}
	}
	for(i=0; i<NUM_INHIBIT_KEYS; i++)
	{
		if(isKeyDown(INHIBIT_KEYS[i]))
		{
			return 0;
		}
	}
	
	return 1;
}

static void doReset()
{
	cli();
// disable watchdog, if enabled
// disable all peripherals
	UDCON = 1;
	USBCON = (1<<FRZCLK);  // disable USB
	UCSR1B = 0;
_delay_ms(5);
#if defined(__AVR_AT90USB162__)                // Teensy 1.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0;
    TIMSK0 = 0; TIMSK1 = 0; UCSR1B = 0;
    DDRB = 0; DDRC = 0; DDRD = 0;
    PORTB = 0; PORTC = 0; PORTD = 0;
    asm volatile("jmp 0x3E00");
#elif defined(__AVR_ATmega32U4__)              // Teensy 2.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
    DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
    PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0x7E00");
#elif defined(__AVR_AT90USB646__)              // Teensy++ 1.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
    DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
    PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0xFC00");
#elif defined(__AVR_AT90USB1286__)             // Teensy++ 2.0
    EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
    TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
    DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
    PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
    asm volatile("jmp 0x1FC00");
#endif 
}

void checkReset()
{
	if(shouldReset())
	{
		doReset();
	}
}