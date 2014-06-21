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

static int8_t trainerMode = 0;

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

static void reportKeyToTrainer(uint8_t keyIdx, uint8_t shiftIdx)
{
	if(shiftIdx == 15)
	{
		switch(keyIdx)
		{
			case 35: usb_keyboard_press(KEY_1, 0); break;
			case 36: usb_keyboard_press(KEY_2, 0); break;
			case 37: usb_keyboard_press(KEY_3, 0); break;
		}
		
		return;
	}
	
	uint8_t shiftKeyMask[6];
	getShiftKeyMaskFromShiftIdx(shiftIdx, shiftKeyMask);
	if(shiftKeyMask[0])
	{
		usb_keyboard_press(KEY_L, KEY_SHIFT); usb_keyboard_press(KEY_1, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	if(shiftKeyMask[1])
	{
		usb_keyboard_press(KEY_L, KEY_SHIFT); usb_keyboard_press(KEY_2, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	if(shiftKeyMask[2])
	{
		usb_keyboard_press(KEY_L, KEY_SHIFT); usb_keyboard_press(KEY_3, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	if(shiftKeyMask[3])
	{
		usb_keyboard_press(KEY_R, KEY_SHIFT); usb_keyboard_press(KEY_1, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	if(shiftKeyMask[4])
	{
		usb_keyboard_press(KEY_R, KEY_SHIFT); usb_keyboard_press(KEY_2, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	if(shiftKeyMask[5])
	{
		usb_keyboard_press(KEY_R, KEY_SHIFT); usb_keyboard_press(KEY_3, 0); usb_keyboard_press(KEYPAD_PLUS, 0);
	}
	switch(keyIdx)
	{
		case calcKeyIdx(0, 2): usb_keyboard_press(KEY_3, 0); break;
		case calcKeyIdx(0, 3): usb_keyboard_press(KEY_4, 0); break;
		case calcKeyIdx(0, 4): usb_keyboard_press(KEY_5, 0); break;
		case calcKeyIdx(0, 5): usb_keyboard_press(KEY_6, 0); break;
		case calcKeyIdx(0, 6): usb_keyboard_press(KEY_7, 0); break;
		case calcKeyIdx(0, 7): usb_keyboard_press(KEY_8, 0); break;
		
		case calcKeyIdx(1, 0): usb_keyboard_press(KEY_Q, KEY_SHIFT); break;
		case calcKeyIdx(1, 1): usb_keyboard_press(KEY_W, KEY_SHIFT); break;
		case calcKeyIdx(1, 2): usb_keyboard_press(KEY_E, KEY_SHIFT); break;
		case calcKeyIdx(1, 3): usb_keyboard_press(KEY_R, KEY_SHIFT); break;
		case calcKeyIdx(1, 4): usb_keyboard_press(KEY_T, KEY_SHIFT); break;
		case calcKeyIdx(1, 5): usb_keyboard_press(KEY_Y, KEY_SHIFT); break;
		case calcKeyIdx(1, 6): usb_keyboard_press(KEY_U, KEY_SHIFT); break;
		case calcKeyIdx(1, 7): usb_keyboard_press(KEY_I, KEY_SHIFT); break;
		case calcKeyIdx(1, 8): usb_keyboard_press(KEY_O, KEY_SHIFT); break;
		case calcKeyIdx(1, 9): usb_keyboard_press(KEY_P, KEY_SHIFT); break;

		case calcKeyIdx(2, 0): usb_keyboard_press(KEY_A, KEY_SHIFT); break;
		case calcKeyIdx(2, 1): usb_keyboard_press(KEY_S, KEY_SHIFT); break;
		case calcKeyIdx(2, 2): usb_keyboard_press(KEY_D, KEY_SHIFT); break;
		case calcKeyIdx(2, 3): usb_keyboard_press(KEY_F, KEY_SHIFT); break;
		case calcKeyIdx(2, 4): usb_keyboard_press(KEY_G, KEY_SHIFT); break;
		case calcKeyIdx(2, 5): usb_keyboard_press(KEY_H, KEY_SHIFT); break;
		case calcKeyIdx(2, 6): usb_keyboard_press(KEY_J, KEY_SHIFT); break;
		case calcKeyIdx(2, 7): usb_keyboard_press(KEY_K, KEY_SHIFT); break;
		case calcKeyIdx(2, 8): usb_keyboard_press(KEY_L, KEY_SHIFT); break;
		case calcKeyIdx(2, 9): 
			usb_keyboard_press(KEY_S, KEY_SHIFT); 
			usb_keyboard_press(KEY_E, KEY_SHIFT);
			usb_keyboard_press(KEY_M, KEY_SHIFT);
			usb_keyboard_press(KEY_I, KEY_SHIFT);
			usb_keyboard_press(KEY_C, KEY_SHIFT);
			usb_keyboard_press(KEY_O, KEY_SHIFT);
			usb_keyboard_press(KEY_L, KEY_SHIFT);
			usb_keyboard_press(KEY_O, KEY_SHIFT);
			usb_keyboard_press(KEY_N, KEY_SHIFT);
			break;

		case calcKeyIdx(3, 0): usb_keyboard_press(KEY_Z, KEY_SHIFT); break;
		case calcKeyIdx(3, 1): usb_keyboard_press(KEY_X, KEY_SHIFT); break;
		case calcKeyIdx(3, 2): usb_keyboard_press(KEY_C, KEY_SHIFT); break;
		case calcKeyIdx(3, 3): usb_keyboard_press(KEY_V, KEY_SHIFT); break;
		case calcKeyIdx(3, 4): usb_keyboard_press(KEY_B, KEY_SHIFT); break;
		case calcKeyIdx(3, 5): usb_keyboard_press(KEY_N, KEY_SHIFT); break;
		case calcKeyIdx(3, 6): usb_keyboard_press(KEY_M, KEY_SHIFT); break;
		case calcKeyIdx(3, 7): 
			usb_keyboard_press(KEY_C, KEY_SHIFT); 
			usb_keyboard_press(KEY_O, KEY_SHIFT);
			usb_keyboard_press(KEY_M, KEY_SHIFT);
			usb_keyboard_press(KEY_M, KEY_SHIFT);
			usb_keyboard_press(KEY_A, KEY_SHIFT);
			break;
		case calcKeyIdx(3, 8): 
			usb_keyboard_press(KEY_P, KEY_SHIFT); 
			usb_keyboard_press(KEY_E, KEY_SHIFT);
			usb_keyboard_press(KEY_R, KEY_SHIFT);
			usb_keyboard_press(KEY_I, KEY_SHIFT);
			usb_keyboard_press(KEY_O, KEY_SHIFT);
			usb_keyboard_press(KEY_D, KEY_SHIFT);
			break;
		case calcKeyIdx(3, 9): 
			usb_keyboard_press(KEY_S, KEY_SHIFT);
			usb_keyboard_press(KEY_L, KEY_SHIFT);
			usb_keyboard_press(KEY_A, KEY_SHIFT);
			usb_keyboard_press(KEY_S, KEY_SHIFT);
			usb_keyboard_press(KEY_H, KEY_SHIFT);
			break;
	
		default: usb_keyboard_press(KEY_SLASH, KEY_SHIFT); break;
	}
	
	usb_keyboard_press(KEY_ENTER, 0); 
}

void pressKey(uint8_t keyIdx)
{
	Mapping mapping;
	
	print("pressKey: key "); phex(keyIdx); print(", shift "); phex(curShiftIdx); print(".\n");
	
	if(trainerMode)
	{
		print("in trainer mode.\n");
		reportKeyToTrainer(keyIdx, curShiftIdx);
		return;
	}
	
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
	if(trainerMode)
	{
		return;
	}
	
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

void setTrainerMode()
{
	if(trainerMode == 0)
	{
		print("Entering trainer mode.\n");
		trainerMode = 1;
		flushState();
	}
}
