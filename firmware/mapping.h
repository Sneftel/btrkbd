/*
 * mapping.h
 *
 * Created: 11/12/2013 23:41:07
 *  Author: Ben
 */ 


#ifndef MAPPING_H_
#define MAPPING_H_

#include <avr/common.h>


/* Mapping flags */
#define MAPPING_FLAG_STICKY 1 /* Modifier keys become sticky, until shift keys changed or an incompatible key is pressed */
#define MAPPING_FLAG_PASSIVE 2 /* Leaves modifier keys unchanged */
#define MAPPING_FLAG_MOD_ONLY 4 /* Changes modifiers, but has no actual key output */

typedef struct Mapping_t {
	uint8_t key;
	uint8_t modifiers : 4;
	uint8_t flags : 4;
	uint8_t undoKey;
	uint8_t undoModifiers : 4;
} Mapping;

#define NUM_SHIFT_INDICES 16

Mapping getMapping(uint8_t shiftIdx, uint8_t keyIdx);
char hasMapping(uint8_t shiftIdx, uint8_t keyIdx);

#endif /* MAPPING_H_ */