/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_led_manip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : ws2812_strip.h
|x|  \|\| | | , /    | |            >   (first version of the actual header)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2019.11.21 (Initial creation date)
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Give the ability to use RGB's LEDs ws2812b
|x|        /|| |  /                 >   functions from associated 'C' file.
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
#include "../../../F14-ws2812_Fusion/base_sfr.h"

// Header's Definition :
// Beginning of definition...
#ifndef __ws2812_led_manip__
#define __ws2812_led_manip__
//===================================================
//================================\HEADER'S_Definitions/================================>

//----------------------------------\GLOBAL_Definition/---------------------------------.
// '-> BIT Definition : Security in case of omitted "base_sfr.h".
#ifndef BIT0
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#endif

//----------------------------------\OUTPUT_Definition/---------------------------------.
// '-> PORT Definition : Strip dedicated pin on port P2 ..
#define	BYO_WS281x P2
// '-> PIN Definition : .. on pin nbr 4 : /!\ FROM 0 /!\:
    sbit SBIT_OUT_STRIP = BYO_WS281x ^ 4;
// '-> Kind of display ; - __LED_ARRANGEMENT_STRIP is a simple strip.
//                       - __LED_ARRANGEMENT_MATRX is a matrix display (16x16 for ex. ).
#define __LED_ARRANGEMENT_STRIP 0
#define __LED_ARRANGEMENT_MATRX 1
// '-> Select the type of display that will free to use some functions :
#define __LED_ARRANGEMENT __LED_ARRANGEMENT_STRIP
//#define __LED_ARRANGEMENT __LED_ARRANGEMENT_MATRX
//---------------------------------\DSIPLAY_Definitions/--------------------------------.
// '-> NUMBER of LEDs presents on the strip :
#define MAX_LEDS 256
#if MAX_LEDS < 256
    typedef unsigned char posType;  // 8bits value.
#elif MAX_LEDS < 65536
    typedef unsigned int posType;   // 16bits value.
#else
    typedef unsigned long posType;  // 32bits value.
#endif

// '-> COLORS Intensities values :
#define BRIGHT_MAX 0xFF // MAX Intensity : 60mA for 1 LED at "TRUE" white (R-G-B to 255).
#define BRIGHT_MIN 0x00 // MIN Intensity : LED turned "off".

#define BRIGHT_MID 0x36 // MID Intensity : Average usage to avoid over-consumption.
#ifndef BRIGHT_MID
#define BRIGHT_MID 0x10 // MID Intensity : Alternated value (just comment upper line).
#endif

//-- GLOBAL TYPE		: -------------------------------->
//-- COLOR          : ------>
typedef struct {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
}color; // Creation of a color type based R-G-B named << color >>.

//-- LED / PIXEL (STRIP) : ->
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_STRIP
    typedef struct {
        color colorPix;         // A color  (see structure above)
        unsigned char status;   // A status (OFF : 0 / ON : 1).
    }pixel; // Creation of the definition of one LED (WS281x) named << pixel >>.
#endif
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_MATRX
    typedef struct {
        color colorPix;         // A color  (see structure above)
        unsigned char status;   // A status (OFF : 0 / ON : 1).
        unsigned char x;    // X value about the matrix.
        unsigned char y;    // Y value about the matrix.
    }pixel; // Creation of the definition of one LED (WS281x) named << pixel >>.
#endif

//-- GLOBAL VARIABLES	: -------------------------------->
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_STRIP
    // Declare an array of << pixel >> defining the LED's strip. Init. in the 'C' file.
    extern xdata pixel strip[MAX_LEDS];
#endif
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_MATRX
    // Declare an array of << pixel >> defining the LED's matrix. Init. in the 'C' file.
    extern xdata pixel matrix[MAX_LEDS];
#endif

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
 * Input		:   addressStrip, 6bytes - address of the strip
 *					newColor, 3bytes - color to set
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_Set(pixel* addressStrip, color newColor, posType position);
/* Description  :   Resetting color & status to the specific LED position.
 * Last_Update  :   2021.01.07
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_Reset(pixel* addressStrip, posType position);
/* Description  :   Recovering << color >> of a specific LED.
 * Last_Update  :   2021.01.07
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern color pixel_GetColor(pixel* addressStrip, posType position);
/* Description  :   Recovering << status >> of a specific LED.
 * Last_Update  :   2021.01.07
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern unsigned char pixel_GetStatus(pixel* addressStrip, posType position);
/* Description  :   Toggling status into the specific strip position.
 * Last_Update  :   2021.01.07
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, xbytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_ToggleStatus(pixel* addressStrip, posType position);
/* Description  :   Send the 24bits color (one by one) of a pixel.
 * Last_Update  :   2021.01.07
 * Input		:   red, 1byte.
 *					green, 1byte.
 *					blue, 1byte.
 * Output	 	:	Nothin'                                                            */
void pixel_Show(unsigned char red, unsigned char green, unsigned char blue);

//-------------------------------------------------------------------------------------->
/* Description  :   Show the entire strip through dedicated output pin.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void leds_Show(pixel* addressStrip);
/* Description  :   Clear COLOR & STATUS on the strip + display it (shut down).
 * Last_Update  :   2021.01.07
 * Input		:   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void leds_Off(pixel* addressStrip);
/* Description  :   Clear ONLY status on the strip, to keep the color in memory.
 * Last_Update  :   2021.01.07
 * Input	    :   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void leds_ResetStatus(pixel* addressStrip);
/* Description  :   Reverse every LED status on all the strip, but keep the colors
 *                  in memory.
 * Last_Update  :   2021.01.06
 * Input	    :   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void leds_Inverter(pixel* addressStrip);
/* Description  :   Set a chain of LEDs to a specific color.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, 2bytes - position in the strip to set the color.
 *					begin, 2bytes - FIRST position of the chain to light up.
 *					end, 2bytes - LAST position of the chain to light up.
 * Output	 	:	Nothin'                                                            */
extern void leds_ChainedLeds(pixel* addressStrip, color newColor, \
                                                posType begin, posType end);

#endif
// End of definition.