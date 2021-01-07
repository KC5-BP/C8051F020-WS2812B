/*,..-----------------------------------------------------.
 /´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |       > Title : ws2812_strip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \     > Src : Timers_Wait_Xms.h
|x|  \|\| | | , /    | |    >   (One of my own
|x|     '`'\|._ |   / /         first header file)
|x|         '\),/  / |      > Creation: 2019.11.21
|x|           |/.-"_/       > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,        > Description :
|x|        ',/ |   /            Give the ability to use
|x|        /|| |  /             RGB's LEDs ws2812b funct.
|x|     |\| |/ |- |             from associated 'C' file.
|x| .-,-/ | /  '/-"
|x| -\|/-/\/ ^,'|
|x| _-     |  |/
|x|  .  --"  /
|x| /--__,.-/
 \`\____________________________________________________/
  \______________________________________________________*/
/*======================================================================================>
=======================================================================================*/
// Linker to (if needed) : ..


// Header's Definition :
// Beginning of definition...
#ifndef __ws2812_strip__
#define __ws2812_strip__

//===================================================
//================================\Header's_Definitions/================================>
#define MAX_LEDS 300 // Number of LED used.

// Basic Color Intensities :
#define BRIGHT_MAX 0xFF // Value to make light a LED (MAX Intensity).
#define BRIGHT_MIN 0x00 // Value to make light a LED (MIN Intensity).

#define BRIGHT_MID 0x36 // Value to make light a LED (MID Intensity).
#ifndef BRIGHT_MID
#define BRIGHT_MID 0x10 // Value to make light a LED (MID Intensity).
#endif

#define STEP_COLOR 0x02 // Value to In-/De- crease the Intensity of a Color.

//-- GLOBAL TYPE		: -------------------------------->
// Description : Creation of color type based R-G-B ..
typedef struct {
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
}color; // .. named color.

// Description : Creation of the definition of one LED (WS281x) ..
typedef struct {
    color colorPix;         // .. with a color,
    unsigned char status;   // And a Status (ON : 1 / OFF : 0).
}pixel; // .. named pixel.

//-- GLOBAL VARIABLES	: -------------------------------->
// Declare an array of pixels defining the Strip. Initialization in the 'C' file.
extern xdata pixel strip[MAX_LEDS];

//-- GLOBAL MACROS ..	: -------------------------------->
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
#define SEND_TO_LED(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//-------------------------------------------------------------------------------------->
// Function Prototypes :
/* Description  :   Filling color into the specific strip position.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 *					newColor, 3bytes - color to set
 *					position, 2bytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_SetColor(pixel* addressStrip, color newColor, unsigned int position);
/* Description  :   Toggling status into the specific strip position.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, 2bytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                            */
extern void pixel_StatusToggle(pixel* addressStrip, unsigned int position);
/* Description  :   Send the 24bits color of one pixel.
 * Last_Update  :   2021.01.06
 * Input		:   red, 1byte.
 *					green, 1byte.
 *					blue, 1byte.
 * Output	 	:	Nothin'                                                            */
void pixel_Show(unsigned char red, unsigned char green, unsigned char blue);

//-------------------------------------------------------------------------------------->
/* Description  :   Show the entire strip through output pin.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void strip_Show(pixel* addressStrip);
/* Description  :   Clear Color & Status on all the strip + display it (or shut it down).
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void strip_Clear(pixel* addressStrip);
/* Description  :   Clear ONLY status on all the strip, to keep the color in memory.
 * Last_Update  :   2021.01.06
 * Input	    :   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void strip_StatusReset(pixel* addressStrip);
/* Description  :   Reverse every LED status on all the strip, but keep the colors
 *                  in memory.
 * Last_Update  :   2021.01.06
 * Input	    :   addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                            */
extern void strip_Inverter(pixel* addressStrip);
/* Description  :   Set a chain of LEDs to a specific color.
 * Last_Update  :   2021.01.06
 * Input		:   addressStrip, 6bytes - address of the strip
 *					position, 2bytes - position in the strip to set the color.
 *					begin, 2bytes - FIRST position of the chain to light up.
 *					end, 2bytes - LAST position of the chain to light up.
 * Output	 	:	Nothin'                                                            */
extern void strip_ChainedLeds(pixel* addressStrip, color newColor, \
                                                unsigned int begin, unsigned int end);

#endif
// End of definition.