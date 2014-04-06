/*
 * mapping.c
 *
 * Created: 11/12/2013 23:42:58
 *  Author: Ben
 */ 

#include "mapping.h"
#include "usb_keyboard_debug.h"
#include <avr/pgmspace.h>

#define K(k) { KEY_ ## k, 0, 0, KEY_BACKSPACE, 0 }
#define ShK(k) {KEY_ ## k, KEY_SHIFT, 0, KEY_BACKSPACE, 0}
#define CtK(k) {KEY_ ## k, KEY_CTRL, 0, KEY_BACKSPACE, 0}

#define PK(k) { KEY_ ## k, 0, MAPPING_FLAG_PASSIVE, 0, 0 }
#define MK(k) { 0, KEY_ ## k, MAPPING_FLAG_MOD_ONLY, 0, 0 }

const static Mapping keyMappings[16][40] PROGMEM = {
/* Unshifted */
{
	{ },		{ },		K(ESC),		K(TAB),		{ },			{ },		{ },		K(BACKSPACE),{ },		{ },
	K(Q),		K(W),		K(E),		K(R),		K(T),			K(Y),		K(U),		K(I),		K(O),		K(P),
	K(A),		K(S),		K(D),		K(F),		K(G),			K(H),		K(J),		K(K),		K(L),		K(SEMICOLON),
	K(Z),		K(X),		K(C),		K(V),		K(B),			K(N),		K(M),		{ },		K(SPACE),	K(ENTER),
},

/* L1 (symbols) */
{
	{ },		{ },		{ },		{ },		{ },			{ },		{ },		K(BACKSPACE), { },		{ },
	ShK(1),		ShK(2),		ShK(3),		ShK(4),		ShK(5),			{ },		K(MINUS),	K(EQUAL),	ShK(MINUS),	ShK(EQUAL),
	ShK(6),		ShK(7),		ShK(8),		ShK(9),		ShK(0),			{ },		K(LEFT_BRACE),K(RIGHT_BRACE),ShK(LEFT_BRACE),ShK(RIGHT_BRACE),
	K(SLASH),	K(BACKSLASH),ShK(SLASH),ShK(BACKSLASH),{ },			{ },		K(COMMA),	K(PERIOD),	ShK(COMMA),	ShK(PERIOD),
},

/* L2 (numbers, symbols) */
{
	{ },		{ },		{ },		{ },		{ },			{ },		{ },		K(BACKSPACE),{ },		{ },
	{ },		{ },		{ },		{ },		{ },			{ },		K(7),		K(8),		K(9),		{ },
	{ },		{ },		{ },		{ },		{ },			K(PERIOD),	K(4),		K(5),		K(6),		{ },
	{ },		{ },		{ },		{ },		{ },			K(0),		K(1),		K(2),		K(3),		{ },
},

/* L3 */
{ },

/* R1 (shift) */
{
	{ },		{ },		{ },		{ },		{ },			{ },		{ },		K(BACKSPACE), { },		{ },
	ShK(Q),		ShK(W),		ShK(E),		ShK(R),		ShK(T),			ShK(Y),		ShK(U),		ShK(I),		ShK(O),		ShK(P),
	ShK(A),		ShK(S),		ShK(D),		ShK(F),		ShK(G),			ShK(H),		ShK(J),		ShK(K),		ShK(L),		ShK(SEMICOLON),
	ShK(Z),		ShK(X),		ShK(C),		ShK(V),		ShK(B),			ShK(N),		ShK(M),		{ },		ShK(MINUS),	{ },
},

/* L1+R1 (commands) */
{
	CtK(Q),		{ },		{ },		{ },		{ },			{ },		{ },		{ },		{ },		{ },
	{ },		CtK(S),		{ },		CtK(F),		{ },			{ },		{ },		{ },		{ },		{ },
	{ },		{ },		{ },		{ },		{ },			{ },		{ },		{ },		{ },		{ },
	CtK(Z),		CtK(X),		CtK(C),		CtK(V),		{ },			{ },		{ },		{ },		{ },		{ },
},

/* L2+R1 */
{ },

/* L3+R1 */
{ },

/* R2 (navigation) */
{
	{ },		{ },		{ },		{ },		{ },			{ },		{ },		{ },		{ },		{ },
	{ },		{ },		{ },		{ },		{ },			{ },		PK(HOME),	PK(UP),		PK(PAGE_UP),{ },
	MK(SHIFT),	MK(CTRL),	MK(ALT),	{ },		{ },			{ },		PK(LEFT),	PK(DOWN),	PK(RIGHT),	{ },
	{ },		{ },		{ },		{ },		{ },			{ },		PK(END),	{ },		PK(PAGE_DOWN),{ },
},

/* L1+R2 */
{

},

/* L2+R2 (commands) */
{ },

/* L3+R2 */
{ },

/* R3 */
{ },

/* L1+R3 */
{ },

/* L2+R3 */
{ },

/* L3+R3 */
{ }
	
};

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

