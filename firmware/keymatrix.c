/*
 * keymatrix.c
 *
 * Created: 11/12/2013 23:21:19
 *  Author: Ben
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/iousb1286.h>
#include "keymatrix.h"
#include "util.h"
#include "portdefs.h"
#include "kbd.h"

void initPins()
{
	/* Set all rows and columns to input (Hi-Z) */
	IO_SET_AS_INPUT(COL0_IO);
	IO_SET_AS_INPUT(COL1_IO);
	IO_SET_AS_INPUT(COL2_IO);
	IO_SET_AS_INPUT(COL3_IO);
	IO_SET_AS_INPUT(COL4_IO);
	IO_SET_AS_INPUT(COL5_IO);
	IO_SET_AS_INPUT(COL6_IO);
	IO_SET_AS_INPUT(COL7_IO);
	IO_SET_AS_INPUT(COL8_IO);
	IO_SET_AS_INPUT(COL9_IO);
	IO_SET_AS_INPUT(ROWL0_IO);
	IO_SET_AS_INPUT(ROWL1_IO);
	IO_SET_AS_INPUT(ROWL2_IO);
	IO_SET_AS_INPUT(ROWL3_IO);
	IO_SET_AS_INPUT(ROWR0_IO);
	IO_SET_AS_INPUT(ROWR1_IO);
	IO_SET_AS_INPUT(ROWR2_IO);
	IO_SET_AS_INPUT(ROWR3_IO);
	
	/* Set shift input pins to input */
	IO_SET_AS_INPUT(SL1_IO);
	IO_SET_AS_INPUT(SL2_IO);
	IO_SET_AS_INPUT(SL3_IO);
	IO_SET_AS_INPUT(SR1_IO);
	IO_SET_AS_INPUT(SR2_IO);
	IO_SET_AS_INPUT(SR3_IO);
	
	/* Disable pull-up resistors on rows (also setting output as 0 for later) */
	IO_OUTPUT_0(ROWL0_IO);
	IO_OUTPUT_0(ROWL1_IO);
	IO_OUTPUT_0(ROWL2_IO);
	IO_OUTPUT_0(ROWL3_IO);
	IO_OUTPUT_0(ROWR0_IO);
	IO_OUTPUT_0(ROWR1_IO);
	IO_OUTPUT_0(ROWR2_IO);
	IO_OUTPUT_0(ROWR3_IO);
	
	/* Enable pull-up resistors on column inputs */
	IO_OUTPUT_1(COL0_IO);
	IO_OUTPUT_1(COL1_IO);
	IO_OUTPUT_1(COL2_IO);
	IO_OUTPUT_1(COL3_IO);
	IO_OUTPUT_1(COL4_IO);
	IO_OUTPUT_1(COL5_IO);
	IO_OUTPUT_1(COL6_IO);
	IO_OUTPUT_1(COL7_IO);
	IO_OUTPUT_1(COL8_IO);
	IO_OUTPUT_1(COL9_IO);
	
	/* Set shift power pins low */
	IO_OUTPUT_0(SPOWL_IO);
	IO_OUTPUT_0(SPOWR_IO);
	
	/* Set shift power pins to output */
	IO_SET_AS_OUTPUT(SPOWL_IO);
	IO_SET_AS_OUTPUT(SPOWR_IO);
	
	/* Enable pull-up resistors on shift input pins */
	IO_OUTPUT_1(SL1_IO);
	IO_OUTPUT_1(SL2_IO);
	IO_OUTPUT_1(SL3_IO);
	IO_OUTPUT_1(SR1_IO);
	IO_OUTPUT_1(SR2_IO);
	IO_OUTPUT_1(SR3_IO);
}

void assertRow(uint8_t row)
{
	switch(row)
	{
		case 0: IO_SET_AS_OUTPUT(ROWL0_IO); IO_SET_AS_OUTPUT(ROWR0_IO); break;
		case 1: IO_SET_AS_OUTPUT(ROWL1_IO); IO_SET_AS_OUTPUT(ROWR1_IO); break;
		case 2: IO_SET_AS_OUTPUT(ROWL2_IO); IO_SET_AS_OUTPUT(ROWR2_IO); break;
		case 3: IO_SET_AS_OUTPUT(ROWL3_IO); IO_SET_AS_OUTPUT(ROWR3_IO); break;
	}
}	

void unassertRow(uint8_t row)
{
	switch(row)
	{
		case 0: IO_SET_AS_INPUT(ROWL0_IO); IO_SET_AS_INPUT(ROWR0_IO); break;
		case 1: IO_SET_AS_INPUT(ROWL1_IO); IO_SET_AS_INPUT(ROWR1_IO); break;
		case 2: IO_SET_AS_INPUT(ROWL2_IO); IO_SET_AS_INPUT(ROWR2_IO); break;
		case 3: IO_SET_AS_INPUT(ROWL3_IO); IO_SET_AS_INPUT(ROWR3_IO); break;
	}
}	

int colIsAsserted(uint8_t col)
{
	switch(col)
	{
		case 0: return !IO_GET_INPUT(COL0_IO);
		case 1: return !IO_GET_INPUT(COL1_IO);
		case 2: return !IO_GET_INPUT(COL2_IO);
		case 3: return !IO_GET_INPUT(COL3_IO);
		case 4: return !IO_GET_INPUT(COL4_IO);
		case 5: return !IO_GET_INPUT(COL5_IO);
		case 6: return !IO_GET_INPUT(COL6_IO);
		case 7: return !IO_GET_INPUT(COL7_IO);
		case 8: return !IO_GET_INPUT(COL8_IO);
		case 9: return !IO_GET_INPUT(COL9_IO);
	}
	
	return 0;
}

void scanMatrix()
{
	for(uint8_t row=0; row<NUM_ROWS; row++)
	{
		assertRow(row);
		_delay_us(10);
		for(uint8_t col=0; col<NUM_COLS; col++)
		{
			uint8_t key = calcKeyIdx(row, col);
			if(colIsAsserted(col))
			{
				reportKeyDown(key);
			}
			else
			{
				reportKeyUp(key);
			}
		}
		unassertRow(row);
	}
}

char immediateIsDown(char keyIdx)
{
	char row = keyIdx / NUM_COLS;
	char col = keyIdx % NUM_COLS;
	
	assertRow(row);
	_delay_us(10);
	char isDown = colIsAsserted(col);
	unassertRow(row);
	return isDown;
}

uint8_t getShiftIdxForShiftKeys(char l1, char l2, char l3, char r1, char r2, char r3)
{
	char shiftIdx;
	if(l3) shiftIdx = 3;
	else if(l2) shiftIdx = 2;
	else if(l1) shiftIdx = 1;
	else shiftIdx = 0;
	
	if(r3) shiftIdx |= 12;
	else if(r2) shiftIdx |= 8;
	else if(r1) shiftIdx |= 4;
	
	return shiftIdx;
}

uint8_t getCurShiftMask()
{
	char l1 = !IO_GET_INPUT(SL1_IO);
	char l2 = !IO_GET_INPUT(SL2_IO);
	char l3 = !IO_GET_INPUT(SL3_IO);
	char r1 = !IO_GET_INPUT(SR1_IO);
	char r2 = !IO_GET_INPUT(SR2_IO);
	char r3 = !IO_GET_INPUT(SR3_IO);
	
	return getShiftIdxForShiftKeys(l1, l2, l3, r1, r2, r3);
}

void getShiftKeyMaskFromShiftIdx(uint8_t shiftIdx, uint8_t shiftKeyMaskOut[6])
{
	for(uint8_t i=0; i<6; i++)
	{
		shiftKeyMaskOut[i] = 0;
	}
	
	{
		uint8_t leftShift = shiftIdx & 0x3;
		if(leftShift != 0)
		{
			shiftKeyMaskOut[leftShift-1] = 1;
		}
	}
	
	{
		uint8_t rightShift = shiftIdx >> 2;
		if(rightShift != 0)
		{
			shiftKeyMaskOut[3 + rightShift-1] = 1;
		}
	}
}