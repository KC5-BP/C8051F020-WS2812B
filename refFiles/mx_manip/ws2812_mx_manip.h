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

//-- GLOBAL TYPE		: -------------------------------->
//-- TEXT Offset    : ------>
typedef struct {
    unsigned char column;   // X val. for Column.
    unsigned char line;     // Y val. for Line.
} offsetText; // Creation of the definition of the text's offset named << offsetText >>.

//-- MATRIX Format  : ------>
typedef struct {
    unsigned int matrixView;    // (16b) Matrix orientation : 0-90-... in degrees.
    unsigned char txtFont;      // Font used like, Classic, Time Number or Mob style.
} matrixFormat;// Creation of the definition of the MATX param. named << matrixFormat >>.

//-- ENUM Type          : -------------------------------->
enum enum_nbrFont {
    numberClassic = 0,
    numberTime,
    numberMob
};

//-- GLOBAL VARIABLES	: -------------------------------->
// Declare a structure about the Orientation and Txt that'll be displayed on the matrix.
extern xdata matrixFormat formatDisplay;

//-- GLOBAL MACROS ..   : -------------------------------->

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
/* Description :  Recovered the position of a LED depending of the Line and Column given.
 *	                + the Matrix orientation take in count through cst (formatDisplay).
 * Creation     :   To be retrieved ...
 * Input       :  posX,	1byte - Column
 *             :  posY,	1byte - Line
 * Output      :      , xbytes - Position from 0 to MAX_LEDS - 1                       */
extern posType pixel_RecoverPosition(unsigned char posX, unsigned char posY);

//-------------------------------------------------------------------------------------->
/* Description  :   Set position where to start displaying text.
 * Creation     :   To be retrieved ...
 * Input        :   column, 1byte
 *              :   line, 1byte
 * Output       :   Nothin'                                                            */
extern void matrix_SetTextPosition(unsigned char column, unsigned char line);
/* Description  :   Filling status of addressMatrix to be displayed.
 * Creation     :   To be retrieved ...
 * Input        :   addressMatrix, 6bytes - address of the matrix
 *					newColor, 3bytes - color to set
 *					_CharToWrite, const char* - Chain of characters that
 *					                        will be analyzed to fill the array.
 * Output       :   Nothin'                                                            */
extern void matrix_Print(pixel* addressMatrix, color newColor, \
												 const char* _CharToWrite);
/* Description  :   Replace switch{} case{} to light up the "matrix_Print()" Fn.
 * Creation     :   To be retrieved ...
 * Input        :   charToPrint, const char* - Actual character that
 *					                                will be analyzed to fill the array.
 *					column, unsigned char* - To modify the actual value instead
 *					                                of copying it and returning it.
 *					line, unsigned char* - To modify the actual value instead
 *					                                of copying it and returning it.
 * Output       :   Nothin'                                                            */
void retrieveHeightWidthOf(const char* character, \
                                            unsigned char* width, unsigned char* height);

#endif
// End of definition.
