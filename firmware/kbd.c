/*
 * kbd.c
 *
 * Created: 11/12/2013 23:46:21
 *  Author: Ben
 */ 

#include "kbd.h"
#include "mapping.h"
#include "usb_keyboard_debug.h"
#include "keymatrix.h"
#include "util.h"

enum
{
	STATE_UP = 0,
	STATE_GOING_DOWN = 1,
	STATE_DOWN = 2,
	STATE_GOING_UP = 3
};

static uint8_t state[40] = {STATE_UP};

static uint8_t passiveModifiers = 0;

void initKbd()
{
	
}

void pressKey(uint8_t keyIdx)
{
	Mapping mapping;
	uint8_t i;
	int8_t slot = -1;
	uint8_t shiftMask = getCurShiftMask();

	print("pressKey: key "); phex(keyIdx); print(", shift "); phex(shiftMask); print(".\n");

	if(!hasMapping(shiftMask, keyIdx))
	{
		print("no mapping\n");
		return;
	}		

	mapping = getMapping(shiftMask, keyIdx);
	
	if(mapping.flags & MAPPING_FLAG_MOD_ONLY)
	{
		passiveModifiers |= mapping.modifiers;
		return;
	}

	/* Determine whether the (output) key is already down */
	for(i=0; i<6; i++)
	{
		if(keyboard_keys[i] == mapping.key)
		{
			print("Found match in slot "); phex(i); print(".\n");
			slot = i;
			break;
		}
	}
	
	if(slot == -1)
	{
		/* Find an unused slot */
		for(i=0; i<6; i++)
		{
			if(keyboard_keys[i] == 0)
			{
				print("Found unused slot "); phex(i); print(".\n");
				slot = i;
				break;
			}
		}
	}
	
	if(slot == -1)
	{
		print("Reusing slot 0.\n");
		/* reuse slot 0 */
		slot = 0;
	}
	
	if(keyboard_keys[slot] != 0)
	{
		print("Flushing slot "); phex(slot); print(".\n");
		/* Free up slot */
		keyboard_keys[slot] = 0;
		usb_keyboard_send();
	}
	
	print("Setting slot "); phex(slot); print(" to keycode "); phex(mapping.key); print(", with modifier mask "); phex(mapping.modifiers); print(".\n");
	keyboard_keys[slot] = mapping.key;
	keyboard_modifier_keys = mapping.modifiers;
	usb_keyboard_send();
}

void flushModifiers()
{
	uint8_t slot;
	for(slot=0; slot<6; slot++)
	{
		if(keyboard_keys[slot] != 0)
			return;
	}
	
	keyboard_modifier_keys = 0;
	usb_keyboard_send();
}

void releaseKey(uint8_t keyIdx)
{
	/* Find possible mappings */
	uint8_t shiftIdx, slot;
	Mapping mapping;
	for(shiftIdx=0; shiftIdx<NUM_SHIFT_INDICES; shiftIdx++)
	{
		if(hasMapping(shiftIdx, keyIdx))
		{
			mapping = getMapping(shiftIdx, keyIdx);
			for(slot=0; slot<6; slot++)
			{
				if(keyboard_keys[slot] == mapping.key)
				{
					keyboard_keys[slot] = 0;
					usb_keyboard_send();
				}
			}
		}
	}
	
	flushModifiers();
}

void reportKeyDown(uint8_t keyIdx)
{
	switch(state[keyIdx])
	{
	case STATE_UP:
		state[keyIdx] = STATE_GOING_DOWN;
		break;
			
	case STATE_GOING_DOWN:
		pressKey(keyIdx);
		state[keyIdx] = STATE_DOWN;
		break;
		
	case STATE_DOWN:
		break;
		
	case STATE_GOING_UP:
		break;
	}
}

void reportKeyUp(uint8_t keyIdx)
{
	switch(state[keyIdx])
	{
	case STATE_UP:
		break;
			
	case STATE_GOING_DOWN:
		state[keyIdx] = STATE_UP;
		break;
		
	case STATE_DOWN:
		state[keyIdx] = STATE_GOING_UP;
		break;
		
	case STATE_GOING_UP:
		releaseKey(keyIdx);
		state[keyIdx] = STATE_UP;
		break;
	}
}

char isKeyDown(uint8_t keyIdx)
{
	return state[keyIdx] == STATE_DOWN;
}