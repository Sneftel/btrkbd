/*
 * kbd.h
 *
 * Created: 11/12/2013 23:22:17
 *  Author: Ben
 */ 


#ifndef KBD_H_
#define KBD_H_

#include <avr/io.h>

void initKbd();

void reportKeyDown(uint8_t keyIdx);
void reportKeyUp(uint8_t keyIdx);

char isKeyDown(uint8_t keyIdx);

#endif /* KBD_H_ */