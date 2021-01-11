/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_strip
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
#ifndef __ws2812_strip__
#define __ws2812_strip__
//===================================================
//================================\HEADER'S_Definitions/================================>

//----------------------------------\OUTPUT_Definition/---------------------------------.
// '-> PORT Definition : Strip dedicated pin on port P2 ..
#define	BYO_WS281x P2
// '-> PIN Definition : .. on pin nbr 4 : /!\ FROM 0 /!\:
    sbit SBIT_OUT_STRIP = BYO_WS281x ^ 4;
//---------------------------------\MATRIX_Definitions/---------------------------------.
// '-> NUMBER of LEDs presents on the matrix :
#define MAX_LEDS 256
#if MAX_LEDS < 256
    typedef unsigned char posType;  // 8bits value.
#elif MAX_LEDS < 65536
    typedef unsigned int posType;   // 16bits value.
#else
    typedef unsigned long posType;  // 32bits value.
#endif

#define MATRIX_NBR 1
// Type of Matrix ...
#define MAX_LINE 16
#define MAX_COLU 16

// '-> COLORS Intensities values :
#define BRIGHT_MAX 0xFF // MAX Intensity : 60mA for 1 LED at WHITE ..
#define BRIGHT_MIN 0x00 // MIN Intensity : LED turned "off".

#define BRIGHT_MID 0x36 // MID Intensity : Average usage to avoid over-consumption.
#ifndef BRIGHT_MID
#define BRIGHT_MID 0x10 // MID Intensity : Alternated value (just comment upper line).
#endif

// '-> Characters displaying (Starting point in the char array
//                                          '-> look in kc5_bp_matrix_alphanum) :
#define OFF_CHAR_DIM_5X7	30
#define OFF_CHAR_DIM_4X7	24
#define OFF_CHAR_DIM_3X7	18
#define OFF_CHAR_DIM_2X7	12

#define OFF_CHAR_DIM_4X6	20

#define OFF_CHAR_DIM_7X6	35

//-- GLOBAL TYPE		: -------------------------------->
//-- COLOR          : ------>
typedef struct {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
}color; // Creation of a color type based R-G-B named << color >>.

//-- LED / PIXEL    : ------>
typedef struct {
    color colorPix;         // A color  (see structure above)
    unsigned char status;   // A status (OFF : 0 / ON : 1).
    unsigned char x;    // X value about the matrix.
    unsigned char y;    // Y value about the matrix.
}pixel; // Creation of the definition of one LED (WS281x) named << pixel >>.

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
// Declare an array of << pixel >> defining the LED's matrix. Init. in the 'C' file.
extern xdata pixel matrix[MAX_LEDS];
// Declare a structure about the Orientation and Txt that'll be displayed on the matrix.
extern xdata matrixFormat matrixDisplay;

//-- GLOBAL MACROS ..   : -------------------------------->
// .. sending DATA '0' in Manchester  / Timing : 0 > 0.4[us] | 1 > 0.8[us] +- 150[ns] :
// _nop_() is in <intrins.h> and waste a time machine, like a delay but scaling
// on the CPU Clock.
#define SEND0() {\
                    SBIT_OUT_STRIP = 1;\
	                _nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					SBIT_OUT_STRIP = 0;\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
				}
// .. sending DATA '1' in Manchester  / Timing : 0 > 0.85[us] | 1 > 0.45[us] +- 150[ns] :
#define SEND1()	{\
                    SBIT_OUT_STRIP = 1;\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					_nop_(); _nop_(); _nop_();\
					_nop_();\
					SBIT_OUT_STRIP = 0;\
					_nop_(); _nop_(); _nop_();\
				}
// .. sending ONE bit of A specific color.
#define SEND_COLOR_BIT(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
/* Description  :   Filling color and setting status to the specific LED position.
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					newColor, 3bytes - color to set
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_Set(pixel* addressMatrix, color newColor, posType position);
/* Description  :   Resetting color & status to the specific LED position.
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_Reset(pixel* addressMatrix, posType position);
/* Description  :   Recovering << color >> of a specific LED.
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern color pixel_GetColor(pixel* addressMatrix, posType position);
/* Description  :   Recovering << status >> of a specific LED.
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern unsigned char pixel_GetStatus(pixel* addressMatrix, posType position);
/* Description  :   Toggling status into the specific strip position.
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_ToggleStatus(pixel* addressMatrix, posType position);
/* Description  :   Send the 24bits color (one by one) of a pixel.
 * Last_Update  :   2021.01.07
 * Input		:   red, 1byte.
 *					green, 1byte.
 *					blue, 1byte.
 * Output	 	:	Nothin'                                                            */
void pixel_Show(unsigned char red, unsigned char green, unsigned char blue);
/* Description :  Recovered the position of a LED depending of the Line and Column given.
 *	               + the Matrix orientation take in count through cst (matrixDisplay).
 * Last_Update :  2021.01.11
 * Input       :  posX,	1byte - Column (from 1 instead of 0)
 *             :  posY,	1byte - Line (from 1 instead of 0)
 * Output      :      , xbytes - Position from 1 to MAX_LEDS                           */
extern unsigned int pixel_RecoverPosition(unsigned char posX, unsigned char posY);

//-------------------------------------------------------------------------------------->
/* Description  :   Show the entire strip through dedicated output pin.
 * Last_Update  :   2021.01.06
 * Input		:   addressMatrix, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void matrix_Show(pixel* addressMatrix);
/* Description  :   Clear COLOR & STATUS on the strip + display it (shut down).
 * Last_Update  :   2021.01.07
 * Input		:   addressMatrix, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void matrix_Off(pixel* addressMatrix);
/* Description  :   Clear ONLY status on the strip, to keep the color in memory.
 * Last_Update  :   2021.01.07
 * Input	    :   addressMatrix, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void matrix_ResetStatus(pixel* addressMatrix);
/* Description  :   Reverse every LED status on all the strip, but keep the colors
 *                  in memory.
 * Last_Update  :   2021.01.06
 * Input	    :   addressMatrix, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void matrix_Inverter(pixel* addressMatrix);
/* Description  :   Set a chain of LEDs to a specific color.
 * Last_Update  :   2021.01.06
 * Input		:   addressMatrix, 6bytes - address of the strip
 *					position, 2bytes - position in the strip to set the color.
 *					begin, 2bytes - FIRST position of the chain to light up.
 *					end, 2bytes - LAST position of the chain to light up.
 * Output	 	:	Nothin'                                                            */
//extern void matrix_ChainedLeds(pixel* addressMatrix, color newColor, \
//                                                posType begin, posType end);
//-------------------------------------------------------->
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
 *					const char*, xbytes - Chain of characters that
 *					                        will be analyzed to fill the array.
 * Output       :   Nothin'                                                            */
extern void matrix_Print(pixel* addressMatrix, color newColor, \
												 const char* _CharToWrite, ...);

#endif
// End of definition.
