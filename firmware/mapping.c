/*
 * mapping.c
 *
 * Created: 11/12/2013 23:42:58
 *  Author: Ben
 */ 

#include "mapping.h"
#include "usb_keyboard_debug.h"
#include <avr/pgmspace.h>

#include "mappings.inc"

Mapping getMapping(uint8_t shiftIdx, uint8_t keyIdx)
{
	Mapping m;
	memcpy_P(&m, &(keyMappings[shiftIdx][keyIdx]), sizeof(Mapping));
	return m;
}

char hasMapping(uint8_t shiftIdx, uint8_t keyIdx)
{
	Mapping m = getMapping(shiftIdx, keyIdx);
	return m.key > 0 || (m.flags & MAPPING_FLAG_MOD_ONLY);
}

