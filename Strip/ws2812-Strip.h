/*-->	Title	:	KBP_NeoPixel
		  |
		  '->	Source	:	Timers_Wait_Xms.h (One of my own first header file)
		  |
		  '->	Creation	:	21.11.2019
		  :			|
		  :			'->	Last Update	:	26.11.2019
		  |
		  '->	Description	:	Give the ability to use NeoPixels func. from
								an external 'C' file.
*/
/*========================================================================================>
=========================================================================================*/
// Linker to : ...
#include "Base_SFR.h"	// ... Base (SFR, sbit, define, var. type, etc...)

// Library Definition :
// Beginning of definition...
#ifndef	__NeoPixel__
#define	__NeoPixel__

//===================================================
//================================\Header's_Definitions/==================================>

// In case of Strips :
#define	MAX_LED			300	// Number of LED used.

// Basic Color Intensities :
#define	BRIGHT_MAX	0xFF	// Value to make light a LED (MAX Intensity).
#define	BRIGHT_MIN	0x00	// Value to make light a LED (MIN Intensity).

#define	BRIGHT_MID	0x36	// Value to make light a LED (MID Intensity).
#ifndef BRIGHT_MID	// Define BRIGHT_MID if not defined befor1e ..
#define	BRIGHT_MID	0x10	// Value to make light a LED (MID Intensity).
#endif

#define STEP_COLOR	0x05	// Value to In-/De- crease the Intensity of a Color.

//-- GLOBAL TYPE		:------------------------------->
// In Header file in case of needed in external file.
typedef struct {
	// Creation of the definition of a LED (WS281x)...
	uint8	ui8_Red;
	uint8	ui8_Green;
	uint8	ui8_Blue;
}tstr_Color; //... named tstr_Color.

typedef struct {
	// Creation of the definition of a LED (WS281x)...
	tstr_Color	Col_Pix;	// Color (Red, Green, Blue),
	uint8 ui8_Status;	// Status (ON / OFF), 
	uint16	ui16_Pos;	// And a Position.
}tstr_NeoPixel; //... named tstr_NeoPixel.

enum enum_Color
{	Null = 0,
	Red,
	Green,
  Blue,
	RedGreen,
	RedBlue,
	GreenBlue,
	All };

//-- GLOBAL VARIABLES	:------------------------------->
// Declare a Pointer to the Matrix of NeoPixels and Initialize it in the 'C' file.
extern xdata tstr_NeoPixel Strip[MAX_LED];

//-- GLOBAL MACROS ..	:------------------------------->
// .. sending DATA '0' in Manchester  / Timing : 0 > 0.4[us] | 1 > 0.8[us] +- 150[ns] :
// _nop_() is in <intrins.h> and waste a time machine, like a delay but scaling
// on the CPU Clock.
#define SEND0()	{\
						SBIT_OUT_STRIP = 1;\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						SBIT_OUT_STRIP = 0;\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
					}
// .. sending DATA '1' in Manchester  / Timing : 0 > 0.85[us] | 1 > 0.45[us] +- 150[ns] :
#define SEND1()	{\
						SBIT_OUT_STRIP = 1;\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						SBIT_OUT_STRIP = 0;\
						_nop_();\
						_nop_();\
						_nop_();\
					}
// .. for sending bit to bit for each color in a iteration.
#define	SEND_TO_LED(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//---------------------------------------------------------------------------------------->
// Function Prototypes :
// Recover the position of a NeoPix in a Matrix, depending on the orientation.
//extern uint16 NeoPix_PosRecov(int8 _i8_PosX, int8 _i8_PosY);
// NeoPix of only a specific Color on a Specific Pos.
extern void NeoPix_SetColor(tstr_NeoPixel* _addNeoPix, tstr_Color _newCol, uint16 _ui16_Pos);
// Send ONE group of 24bits, must be called in an iteration loop of the nbr of LEDs.
void NeoPix_Show(uint8 _Red, uint8 _Green, uint8 _Blue);

// Clear the Display.
extern void Matrix_Clear(tstr_NeoPixel* _addMatrix);
// Reset Status about Activ LEDs.
extern void Matrix_ResetStatus(tstr_NeoPixel* _addMatrix);
// Set position for printing char on Matrix.
//extern void Matrix_SetPos(uint8 _ui8_Col, uint8 _ui8_Lin);
// Or just give the Board Address and Send it Fully :
extern void Matrix_Show(tstr_NeoPixel* _addMatrix);

// Printing characters on Matrix.
//extern void Matrix_Print(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, const char* _CharToWrite, ...);
// NeoPix of only a specific Color and Fading it.
//void NeoPix_Fader(NeoPixel* _AddNeoPix, Color _NewCol, uint16 _ui16_Pos, uint16 _ui16_Speed);
// Board of only a specific Color on a specific position.
extern void Matrix_LimitPos(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, uint16 _Begin, uint16 _End);
// Reversed every status of the LED's group.
extern void Matrix_InvertArray(tstr_NeoPixel* _addMatrix);


#endif
// End of definition.
