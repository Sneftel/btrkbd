/*
 * BtrKbd.c
 *
 * Created: 11/12/2013 21:05:03
 *  Author: Ben
 */ 

#include <avr/io.h>

#include "portdefs.h"
#include "util.h"
#include "keymatrix.h"
#include "kbd.h"
#include "usb_keyboard_debug.h"
#include "reset.h"

int main(void)
{
	initPins();
	initKbd();
	usb_init();
	
    while(1)
    {
		checkReset();
		scanMatrix();
		tickKbd();
    }
}
