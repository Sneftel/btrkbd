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

#define SLOT_PASSIVE_MODIFIER -2

static int8_t keySlots[40];
static int8_t slotKeys[6];

static uint8_t curShiftIdx = 0;


static void flushState()
{
	/* Reset all slots */
	for(uint8_t i=0; i<40; i++)
	{
		keySlots[i] = -1;
	}
	for(uint8_t i=0; i<6; i++)
	{
		slotKeys[i] = -1;
	}
		
	/* Un-press all keys */
	for(uint8_t i=0; i<6; i++)
	{
		keyboard_keys[i] = 0;
	}
		
	/* Reset modifiers */
	keyboard_modifier_keys = 0;
	
	passiveModifiers = 0;

	usb_keyboard_send();
}

static void checkShift()
{
	uint8_t newShiftIdx = getCurShiftMask();
	if(curShiftIdx != newShiftIdx)
	{
		flushState();	
		curShiftIdx = newShiftIdx;			
	}
}

void initKbd()
{
	for(uint8_t i=0; i<6; i++)
	{
		slotKeys[i] = -1;
	}
	for(uint8_t i=0; i<40; i++)
	{
		keySlots[i] = -1;
	}
}

void tickKbd()
{
	checkShift();
}

static uint8_t findFreeSlot()
{
	for(uint8_t i=0; i<6; i++)
	{
		if(slotKeys[i] == -1)
		{
			return i;
		}
	}
	return -1;
}

static int8_t findSlotMatchingKey(uint8_t keycode)
{
	for(uint8_t i=0; i<6; i++)
	{
		if(keyboard_keys[i] == keycode)
		{
			return i;
		}
	}
	return -1;		
}

void pressKey(uint8_t keyIdx)
{
	Mapping mapping;

	print("pressKey: key "); phex(keyIdx); print(", shift "); phex(curShiftIdx); print(".\n");
	
	if(!hasMapping(curShiftIdx, keyIdx))
	{
		print("no mapping\n");
		return;
	}

	mapping = getMapping(curShiftIdx, keyIdx);
	
	if(mapping.flags & MAPPING_FLAG_MOD_ONLY)
	{
		passiveModifiers |= mapping.modifiers;
		print("adding passive modifier, new mask is"); phex(passiveModifiers); print(".\n");
		keySlots[keyIdx] = SLOT_PASSIVE_MODIFIER;
		return;
	}
	
	/* See whether the keycode is already pressed */
	if(findSlotMatchingKey(mapping.key) != -1)
	{
		print("keycode was already pressed\n");
		return;
	}
	
	int8_t slot = findFreeSlot();
	
	if(slot == -1)
	{
		print("No free slots.\n");
		/* reuse slot 0 */
		return;
	}

	uint8_t modifiers = mapping.flags & MAPPING_FLAG_PASSIVE ? passiveModifiers : mapping.modifiers;
	print("Setting slot "); phex(slot); print(" to keycode "); phex(mapping.key); print(", with modifier mask "); phex(modifiers); print(".\n");
	
	slotKeys[slot] = keyIdx;
	keySlots[keyIdx] = slot;
	
	keyboard_keys[slot] = mapping.key;
	keyboard_modifier_keys = modifiers;
	usb_keyboard_send();
}

static uint8_t anyKeysPressed()
{
	for(uint8_t i=0; i<40; i++)
	{
		if(keySlots[i] != -1)
		{
			return 1;
		}
	}
	return 0;
}

void releaseKey(uint8_t keyIdx)
{
	uint8_t slot = keySlots[keyIdx];
	if(slot == -1)
	{
		/* Keypress was dead */
	}
	else if(slot == SLOT_PASSIVE_MODIFIER)
	{
		Mapping mapping = getMapping(curShiftIdx, keyIdx);
		if((mapping.flags & MAPPING_FLAG_MOD_ONLY)) /* should always be the case */
		{
			passiveModifiers &= ~mapping.modifiers;
			print("removing passive modifier, new mask is"); phex(passiveModifiers); print(".\n");
			keyboard_modifier_keys = passiveModifiers;
		}
	}
	else
	{
		keyboard_keys[slot] = 0;
		keySlots[keyIdx] = -1;
		slotKeys[slot] = -1;
	}
	
	usb_keyboard_send();
	
	if(!anyKeysPressed())
	{
		print("all keys are up, flushing state.\n");
		flushState();
	}
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
