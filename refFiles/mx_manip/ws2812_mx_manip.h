/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_matrix
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : Timers_Wait_Xms.h
|x|  \|\| | | , /    | |        >           (One of my own first c file)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation: 2019.11.21
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                Give the ability to use RGB's LEDs ws2812b
|x|        /|| |  /                 functions from associated 'C' file.
|x|     |\| |/ |- |
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
// .. C51/C166 routines that instructs the compiler to generate intrinsic code | _nop_():
#include <intrins.h>
// .. Base (SFR, sbit, define, var. type, etc...)
#include "../F11-NeoPix_StripPrButt/base_sfr.h"

// Header's Definition :
// Beginning of definition...
#ifndef __ws2812_mx_manip__
#define __ws2812_mx_manip__
//===================================================
//================================\HEADER'S_Definitions/================================>

//---------------------------------\MATRIX_Definitions/---------------------------------.
#define NBR_OF_MATRIX 1
// Type of Matrix ...
#define MAX_LINE 16
#define MAX_COLU 16

// '-> Characters displaying (Starting point in the char array
//                                          '-> look in kc5_bp_matrix_alphanum) :
#define OFF_CHAR_DIM_5X7	30
#define OFF_CHAR_DIM_4X7	24
#define OFF_CHAR_DIM_3X7	18
#define OFF_CHAR_DIM_2X7	12

#define OFF_CHAR_DIM_4X6	20

#define OFF_CHAR_DIM_7X6	35

//-- GLOBAL TYPE		: -------------------------------->
//-- TEXT Offset    : ------>
typedef struct {
    unsigned char column;   // X val. for Column.
    unsigned char line;     // Y val. for Line.
}offsetText; // Creation of the definition of the text's offset named << offsetText >>.

//-- MATRIX Format  : ------>
typedef struct {
    unsigned int matrixView;    // (16b) Matrix orientation : 0-90-... in degrees.
    unsigned char txtFont;      // Font used like, Classic, Time Number or Mob style.
}matrixFormat; // Creation of the definition of the MATX param. named << matrixFormat >>.

//-- ENUM Type          : -------------------------------->
enum enum_nbrFont
{	numberClassic = 0,
    numberTime,
    numberMob };

//-- GLOBAL VARIABLES	: -------------------------------->
// Declare a structure about the Orientation and Txt that'll be displayed on the matrix.
extern xdata matrixFormat matrixDisplay;

//-- GLOBAL MACROS ..   : -------------------------------->

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
/* Description :  Recovered the position of a LED depending of the Line and Column given.
 *	               + the Matrix orientation take in count through cst (matrixDisplay).
 * Last_Update :  2021.01.11
 * Input       :  posX,	1byte - Column (from 1 instead of 0)
 *             :  posY,	1byte - Line (from 1 instead of 0)
 * Output      :      , xbytes - Position from 1 to MAX_LEDS                           */
extern unsigned int pixel_RecoverPosition(unsigned char posX, unsigned char posY);

//-------------------------------------------------------------------------------------->
/* Description  :   Set position where to start displaying text.
 * Last_Update  :   2021.01.11
 * Input        :   column, 1byte - Column (from 1 instead of 0)
 *              :   line, 1byte - Line (from 1 instead of 0)
 * Output       :   Nothin'                                                            */
extern void matrix_SetTextPosition(unsigned char column, unsigned char line);
/* Description  :   Filling status of addressMatrix to be displayed.
 * Last_Update  :   2021.01.11
 * Input        :   addressMatrix, 6bytes - address of the matrix
 *					newColor, 3bytes - color to set
 *					_CharToWrite, const char* - Chain of characters that
 *					                        will be analyzed to fill the array.
 * Output       :   Nothin'                                                            */
extern void matrix_Print(pixel* addressMatrix, color newColor, \
												 const char* _CharToWrite, ...);
/* Description  :   Replace switch{} case{} to light up the "matrix_Print()" Fn.
 * Last_Update  :   2021.01.19
 * Input        :   charToPrint, const char* - Actual character that
 *					                                will be analyzed to fill the array.
 *					column, unsigned char* - To modify the actual value instead
 *					                                of copying it and returning it.
 *					line, unsigned char* - To modify the actual value instead
 *					                                of copying it and returning it.
 * Output       :   Nothin'                                                            */
//void char_SetColumnAndLine(const char* charToPrint, unsigned char* column, unsigned char* line);

#endif
// End of definition.
