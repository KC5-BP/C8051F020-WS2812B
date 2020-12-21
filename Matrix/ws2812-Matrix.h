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
/*======================================================================================>
=======================================================================================*/
// Linker to : ...
#include "base_sfr.h"	// ... Base (SFR, sbit, define, var. type, etc...)

// Library Definition :
// Beginning of definition...
#ifndef  __ws281x_matrix__
#define  __ws281x_matrix__

//===================================================
//================================\Header's_Definitions/================================>

// In case of Matrix :
#define MAX_LEDS 256 // Number of LED used with Matrix
#define MATRIX_NBR 1
// Type of Matrix...
#define MAX_LINE 16
#define MAX_COLUMN 16

// Basic Color Intensities :
#define BRIGHT_MAX 0xFF	// Value to make light a LED (MAX Intensity).
#define BRIGHT_MIN 0x00	// Value to make light a LED (MIN Intensity).
#define BRIGHT_MID 0x36	// Value to make light a LED (MID Intensity).
#ifndef BRIGHT_MID
#define BRIGHT_MID 0x10	// Value to make light a LED (MID Intensity).
#endif
#define STEP_COLOR	0x05	// Value to In-/De- crease the Intensity of a Color.

// In case of Text Printing : Char. Aire - Width
#define OFF_CHAR_DIM_5X7	30
#define OFF_CHAR_DIM_4X7	24
#define OFF_CHAR_DIM_3X7	18
#define OFF_CHAR_DIM_2X7	12

#define OFF_CHAR_DIM_4X6	20

#define OFF_CHAR_DIM_7X6	35

//-- GLOBAL TYPE		:------------------------------->
// Description : Creation of color type based R-G-B
typedef struct {
	unsigned char Red;
   unsigned char Green;
   unsigned char Blue;
}color; //... named color.

// Description : Creation of the definition of one LED (WS281x).
typedef struct {
	color colorPix;	      // color (Red, Green, Blue),
	unsigned char status;   // status (ON : 1 / OFF : 0),
	unsigned int pos;       // And a Position.
}pixel; //... named pixel.

// Description : Creation of the offset position for Text Printing.
typedef struct {
   unsigned char column;   // X val. for Column.
   unsigned char line;     // Y val. for Line.
}offsetText; //... named offsetText.

// Description : Matrix format
typedef struct {
   unsigned int matrixView;   // Matrix orientation : 0-90-... degrees
   unsigned char txtFont;     // Font used like, Classic, Time Number or Mob style.
}matrixFormat; //... named matrixFormat.

enum enum_Color
{	Null = 0,
	Red,
	Green,
   Blue,
	RedGreen,
	RedBlue,
	GreenBlue,
	All };

enum enum_numberFont
{	numberClassic = 0,
	numberTime,
	numberMob };

//-- GLOBAL VARIABLES	:------------------------------->
// Declare an array of pixels defining the Matrix. Initialization in the 'C' file.
extern xdata pixel matrix[MAX_LEDS];
//extern unsigned int i16_MatOri;
extern xdata matrixFormat matrixDisplay;

//-- GLOBAL MACROS ..	:------------------------------->
// .. sending DATA '0' in Manchester  / Timing : 0 > 0.4[us] | 1 > 0.8[us] +- 150[ns] :
// _nop_() is in <intrins.h> and waste a time machine, like a delay but scaling
// on the CPU Clock.
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
// .. for sending bit to bit for each color in through iteration.
#define	SEND_TO_LED(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//-------------------------------------------------------------------------------------->
// Function Prototypes :
/* Description :  Recovered the position of a LED depending of the Line and Column given.
 *	               + the Matrix orientation take in count through cst.
 * Last_Update :  21.12.2020
 * Input       :  posX,	1byte - Column (from 1 instead of 0)
 *             :  posY,	1byte - Line (from 1 instead of 0)
 * Output      :      , 2bytes - Position from 1 to MAX_LEDS                           */
extern unsigned int pixel_PosRecovery(unsigned char posX, unsigned char posY);
/* Description :  Filling color into the specific matrix position.
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 *						newColor, 3bytes - color to set
 *						position, 2bytes - position in the matrix to set the color.
 * Output	 	:	Nothin'                                                              */
extern void pixel_SetColor(pixel* addressMatrix, color newColor, unsigned int position);
/* Description :	Send the 24bits color of one pixel.
 * Last_Update :  21.12.2020
 * Input			:	red, 1byte.
 *						green, 1byte.
 *						blue, 1byte.
 * Output	 	:	Nothin'                                                              */
void pixel_Show(unsigned char red, unsigned char green, unsigned char blue);


/* Description :	Clear color & status on all the matrix + display it (or shut it).
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 * Output	 	:	Nothin'                                                              */
extern void matrix_Clear(pixel* addressMatrix);
/* Description :	Clear ONLY status on all the matrix, to keep the color if needed.
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 * Output	 	:	Nothin'                                                              */
extern void matrix_StatusReset(pixel* addressMatrix);
/* Description :	Show the entire matrix through output pin.
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 * Output	 	:	Nothin'                                                              */
extern void matrix_Show(pixel* addressMatrix);


/* Description :	Set position where to start displaying text.
 * Last_Update :  21.12.2020
 * Input       :  posX,	1byte - Column (from 1 instead of 0)
 *             :  posY,	1byte - Line (from 1 instead of 0)
 * Output	 	:	Nothin'                                                              */
extern void matrix_SetTextPos(unsigned char  column, unsigned char line);
/* Description :	Printing text on Matrix.
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 *						newColor, 3bytes - color to set
 *						position, 2bytes - position in the matrix to set the color.
 * Output	 	:	Nothin'                                                              */
extern void matrix_Print(pixel* addressMatrix, color newColor, const char* _CharToWrite, ...);

/* Description :	Setting a group of consecutive LED to a color.
 * Last_Update :  21.12.2020
 * Input		   :	addressMatrix, 6bytes - address of the matrix
 *						newColor, 3bytes - color to set
 *						position, 2bytes - position in the matrix to set the color.
 * Output	 	:	Nothin'                                                              */
extern void strip_LedChain(pixel* addressMatrix, color newColor, unsigned int start, unsigned int end);


#endif
// End of definition.
