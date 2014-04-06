/*
 * util.h
 *
 * Created: 11/12/2013 23:21:33
 *  Author: Ben
 */ 


#ifndef UTIL_H_
#define UTIL_H_

#define DDR_REG(port) DDR ## port
#define PORT_REG(port) PORT ## port
#define PIN_REG(port) PIN ## port



#define SET_BIT(port, bit) do { (port) |= (1 << (bit)); } while(0)
#define CLR_BIT(port, bit) do { (port) &= ~(1 << (bit)); } while(0)
#define BIT_IS_SET(port, bit) ((((uint8_t)(port)) >> ((uint8_t)(bit))) & 0x1)

#define IO_SET_INPUT_AUX(port, bit) CLR_BIT(DDR_REG(port), bit)
#define IO_SET_AS_INPUT(io) IO_SET_INPUT_AUX(io)

#define IO_SET_OUTPUT_AUX(port, bit) SET_BIT(DDR_REG(port), bit)
#define IO_SET_AS_OUTPUT(io) IO_SET_OUTPUT_AUX(io)

#define IO_OUTPUT_0_AUX(port, bit) CLR_BIT(PORT_REG(port), bit)
#define IO_OUTPUT_0(io) IO_OUTPUT_0_AUX(io)

#define IO_OUTPUT_1_AUX(port, bit) SET_BIT(PORT_REG(port), bit)
#define IO_OUTPUT_1(io) IO_OUTPUT_1_AUX(io)

#define IO_GET_INPUT_AUX(port, bit) BIT_IS_SET(PIN_REG(port), bit)
#define IO_GET_INPUT(io) IO_GET_INPUT_AUX(io)


#ifdef ENABLE_DEBUG_OUTPUT
#include "print.h"
#else
#define print(x)
#define phex(x)
#endif



#endif /* UTIL_H_ */