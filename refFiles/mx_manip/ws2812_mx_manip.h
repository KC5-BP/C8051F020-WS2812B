/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_mx_manip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : ws2812_matrix.h
|x|  \|\| | | , /    | |            >   (first version of the actual header)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2019.11.21 (Initial creation date)
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Give the ability to use RGB's LEDs ws2812b
|x|        /|| |  /                 >   functions from associated 'C' file
|x|     |\| |/ |- |                 >   for matrix display.
|x| .-,-/ | /  '/-"
|x| -\|/-/\/ ^,'|
|x| _-     |  |/
|x|  .  --"  /
|x| /--__,.-/
.\`\__________________________________________________________________________________/´/
..`____________________________________________________________________________________´
========================================================================================>
=======================================================================================*/
// Linker to : ..
// .. created header for ws2812b led usage.
#include "../led_manip/ws2812_led_manip.h"  // For the << pixel >> struct.

//===================================================
//================================\HEADER'S_Definitions/================================>
#ifndef __WS2812_MX_MANIP__
#define __WS2812_MX_MANIP__    // __WS2812_MX_MANIP__ BEGIN

//---------------------------------\MATRIX_Definitions/---------------------------------.
#define NBR_OF_MATRIX 1
// Matrix's dimensions (in pixels)
#define MAX_LINE 16
#define MAX_COLU 16

//-- GLOBAL TYPE		: -------------------------------->
//-- TEXT Offset    : ------>
typedef struct {
	unsigned char column;   // X val. for Column.
	unsigned char line;     // Y val. for Line.
} offsetText;

//-- MATRIX Format  : ------>
typedef struct {
	unsigned int orientation;    // (16b) Matrix orientation : 0-90-... in degrees.
	unsigned char font;    // Font used like, Classic, Time Number or Mob style.
} matrixFormat;

//-- ENUM Type          : -------------------------------->
enum font { CLASSIC, TIME, MOB };

//-- GLOBAL VARIABLES	: -------------------------------->
// Declare a structure about the orientation and font that'll be displayed on the matrix.
extern xdata matrixFormat formatDisplay;

//-- GLOBAL MACROS ..   : -------------------------------->

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
//======================================================================================>
/* Description :  Recovered the position of a LED depending of the Line and Column given.
 *	            	+ the Matrix orientation take in count through cst (formatDisplay).*/
extern posType pixel_RecoverPosition(unsigned char posX, unsigned char posY);

//======================================================================================>
/* Description  :   Set position where to start displaying text.					   */
extern void matrix_SetTextPosition(unsigned char column, unsigned char line);
//======================================================================================>
/* Description  :   Filling status of addressMatrix to display text.				   */
extern void matrix_Print(pixel* addressMatrix, color newColor, \
															const char* _CharToWrite);

#endif    // __WS2812_MX_MANIP__ END
