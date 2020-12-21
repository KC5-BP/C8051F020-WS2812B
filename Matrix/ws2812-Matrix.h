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

// In case of Matrix :
//#define	MAX_LED			256	// Number of LED used with Matrix
#define MATRIX_NBR	01
// Type of Matrix...
#define	MAX_LINE		16
#define	MAX_COLUMN	16

// In case of Strips :
#define	MAX_LED			300	// Number of LED used.

// Basic Color Intensities :
#define	BRIGHT_MAX	0xFF	// Value to make light a LED (MAX Intensity).
#define	BRIGHT_MIN	0x00	// Value to make light a LED (MIN Intensity).
//#define	BRIGHT_MID	0x36	// Value to make light a LED (MID Intensity).
#define	BRIGHT_MID	0x10	// Value to make light a LED (MID Intensity).
#define STEP_COLOR	0x05	// Value to In-/De- crease the Intensity of a Color.

// In case of Text Printing : Char. Aire - Width
#define OFF_CHAR_DIM_5X7	30
#define OFF_CHAR_DIM_4X7	24
#define OFF_CHAR_DIM_3X7	18
#define OFF_CHAR_DIM_2X7	12

#define OFF_CHAR_DIM_4X6	20

#define OFF_CHAR_DIM_7X6	35

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

typedef struct {
	// Creation of the offset position for Text Printing...
	uint8 ui8_Colu;	// X val. for Column,
	uint8 ui8_Line;	// Y val. for Line.
}tstr_Offset; //... named tstr_Offset.

typedef struct {
	// Creation of Matrix Parameter...
	int16 i16_MatOri;	// Matrix Orientation : 0-90-...
	int8 i8_TxtFont;	// Font used like, Classic, Time Number or Mob style.
}tstr_MatrixFormat; //... named tstr_MatrixFormat.

enum enum_Color
{	Null = 0,
	Red,
	Green,
  Blue,
	RedGreen,
	RedBlue,
	GreenBlue,
	All };

enum enum_Devices
{	enu_Matrix = 0,
	enu_Strip };

enum enum_NumberFormat
{	enu_Classic = 0,
	enu_Time,
	enu_Mob };

//-- GLOBAL VARIABLES	:------------------------------->
// Declare a Pointer to the Matrix of NeoPixels and Initialize it in the 'C' file.
extern xdata tstr_NeoPixel Strip[MAX_LED];
//extern xdata NeoPixel Matrix[256];
//extern int16 i16_MatOri;
extern xdata tstr_MatrixFormat MatFormat;

//-- GLOBAL MACROS ..	:------------------------------->
// .. sending DATA '0' in Manchester  / Timing : 0 > 0.4[us] | 1 > 0.8[us] +- 150[ns] :
// _nop_() is in <intrins.h> and waste a time machine, like a delay but scaling
// on the CPU Clock.
/* (_OUT_DEVICE == 0) ? (SBIT_OUT_MATRX = 1) : (SBIT_OUT_STRIP = 1); */
/* (_OUT_DEVICE == 0) ? (SBIT_OUT_MATRX = 0) : (SBIT_OUT_STRIP = 0); */
#define SEND0()	{\
						SBIT_OUT_MATRX = 1;\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						_nop_();\
						SBIT_OUT_MATRX = 0;\
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
/* (_OUT_DEVICE == 0) ? (SBIT_OUT_MATRX = 1) : (SBIT_OUT_STRIP = 1); */
/* (_OUT_DEVICE == 0) ? (SBIT_OUT_MATRX = 0) : (SBIT_OUT_STRIP = 0); */
#define SEND1()	{\
						SBIT_OUT_MATRX = 1;\
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
						SBIT_OUT_MATRX = 0;\
						_nop_();\
						_nop_();\
						_nop_();\
					}
// .. for sending bit to bit for each color in a iteration.
#define	SEND_TO_LED(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//---------------------------------------------------------------------------------------->
// Function Prototypes :
// Recover the position of a NeoPix in a Matrix, depending on the orientation.
extern uint16 NeoPix_PosRecov(int8 _i8_PosX, int8 _i8_PosY);
// NeoPix of only a specific Color on a Specific Pos.
extern void NeoPix_SetColor(tstr_NeoPixel* _addNeoPix, tstr_Color _newCol, uint16 _ui16_Pos);
// Send ONE group of 24bits, must be called in an iteration loop of the nbr of LEDs.
void NeoPix_Show(uint8 _Red, uint8 _Green, uint8 _Blue);

// Clear the Display.
extern void Matrix_Clear(tstr_NeoPixel* _addMatrix);
// Reset Status about Activ LEDs.
extern void Matrix_ResetStatus(tstr_NeoPixel* _addMatrix);
// Set position for printing char on Matrix.
extern void Matrix_SetPos(uint8 _ui8_Col, uint8 _ui8_Lin);
// Or just give the Board Address and Send it Fully :
extern void Matrix_Show(tstr_NeoPixel* _addMatrix);

// Printing characters on Matrix.
extern void Matrix_Print(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, const char* _CharToWrite, ...);
// NeoPix of only a specific Color and Fading it.
//void NeoPix_Fader(NeoPixel* _AddNeoPix, Color _NewCol, uint16 _ui16_Pos, uint16 _ui16_Speed);
// Board of only a specific Color on a specific position.
extern void Matrix_LimitPos(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, uint16 _Begin, uint16 _End);


#endif
// End of definition.
