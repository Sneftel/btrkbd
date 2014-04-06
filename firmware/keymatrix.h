/*
 * keymatrix.h
 *
 * Created: 11/12/2013 23:33:36
 *  Author: Ben
 */ 


#ifndef KEYMATRIX_H_
#define KEYMATRIX_H_

#include <stdint.h>

#define NUM_ROWS 4
#define NUM_COLS 10

void initPins();

void scanMatrix();

char immediateIsDown(char keyIdx);


uint8_t getShiftIdxForShiftKeys(char l1, char l2, char l3, char r1, char r2, char r3);

uint8_t getCurShiftMask();

#define calcKeyIdx(row, col) ((row)*NUM_COLS + (col))

#endif /* KEYMATRIX_H_ */