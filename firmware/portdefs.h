/*
 * portdefs.c
 *
 * Created: 11/12/2013 23:20:21
 *  Author: Ben
 */ 
#ifndef PORTDEFS_H
#define PORTDEFS_H

#include <avr/io.h>


#define COL0_IO B,7
#define COL1_IO D,0
#define COL2_IO D,1
#define COL3_IO D,2
#define COL4_IO D,3
#define COL5_IO B,2
#define COL6_IO B,3
#define COL7_IO B,4
#define COL8_IO B,5
#define COL9_IO B,6

#define ROWL0_IO D,4
#define ROWL1_IO D,5
#define ROWL2_IO D,6
#define ROWL3_IO D,7

#define ROWR0_IO B,1
#define ROWR1_IO B,0
#define ROWR2_IO E,7
#define ROWR3_IO E,6

#define SPOWL_IO E,0
#define SPOWR_IO F,0

#define SL1_IO E,1
#define SL2_IO C,0
#define SL3_IO C,1

#define SR1_IO F,1
#define SR2_IO F,2
#define SR3_IO F,3


#endif