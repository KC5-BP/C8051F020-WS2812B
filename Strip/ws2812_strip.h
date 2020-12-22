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
// Linker to (if needed) : ..


// Library Definition :
// Beginning of definition...
#ifndef  __ws281x_strip__
#define  __ws281x_strip__

//===================================================
//================================\Header's_Definitions/==================================>
#define MAX_LEDS 300 // Number of LED used.

// Basic Color Intensities :
#define BRIGHT_MAX 0xFF	// Value to make light a LED (MAX Intensity).
#define BRIGHT_MIN 0x00	// Value to make light a LED (MIN Intensity).

#define BRIGHT_MID 0x36	// Value to make light a LED (MID Intensity).
#ifndef BRIGHT_MID
#define BRIGHT_MID 0x10	// Value to make light a LED (MID Intensity).
#endif

#define STEP_COLOR	0x05	// Value to In-/De- crease the Intensity of a Color.

//-- GLOBAL TYPE		:------------------------------->
// Description : Creation of color type based R-G-B ..
typedef struct {
   unsigned char Red;
   unsigned char Green;
   unsigned char Blue;
}color; // .. named color.

// Description : Creation of the definition of one LED (WS281x) ..
typedef struct {
   color colorPix;	      // color (Red, Green, Blue),
   unsigned char status;   // status (ON : 1 / OFF : 0),
   unsigned int pos;       // And a Position.
}pixel; // .. named pixel.

//-- GLOBAL VARIABLES	:------------------------------->
// Declare an array of pixels defining the Matrix. Initialization in the 'C' file.
extern xdata pixel strip[MAX_LEDS];

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

// .. sending ONE bit of A specific color.
#define	SEND_TO_LED(COLOR, MASK) { if((COLOR & MASK) != 0) {SEND1();} else{SEND0();} }

//-------------------------------------------------------------------------------------->
// Function Prototypes :
/* Description :  Filling color into the specific strip position.
 * Last_Update :  22.12.2020
 * Input		   :	addressStrip, 6bytes - address of the strip
 *						newColor, 3bytes - color to set
 *						position, 2bytes - position in the strip to set the color.
 * Output	 	:	Nothin'                                                                  */
extern void pixel_SetColor(pixel* addressStrip, color newColor, \
													 unsigned int position);
/* Description :	Send the 24bits color of one pixel.
 * Last_Update :  22.12.2020
 * Input			:	red, 1byte.
 *						green, 1byte.
 *						blue, 1byte.
 * Output	 	:	Nothin'                                                                  */
void pixel_Show(unsigned char red, unsigned char green, unsigned char blue);


/* Description :	Clear color & status on all the strip + display it (or shut it).
 * Last_Update :  21.12.2020
 * Input		   :	addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                                  */
extern void strip_Clear(pixel* addressStrip);
/* Description :	Clear ONLY status on all the strip, to keep the color if needed.
 * Last_Update :  21.12.2020
 * Input		   :	addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                                  */
extern void strip_StatusReset(pixel* addressStrip);
/* Description :	Show the entire strip through output pin.
 * Last_Update :  21.12.2020
 * Input		   :	addressStrip, 6bytes - address of the strip
 * Output	 	:	Nothin'                                                                  */
extern void strip_Show(pixel* addressStrip);


// NeoPix of only a specific Color and Fading it.
//void NeoPix_Fader(NeoPixel* _AddNeoPix, Color _NewCol, uint16 _ui16_Pos, uint16 _ui16_Speed);
// Board of only a specific Color on a specific position.
extern void strip_LimitPos(pixel* addressStrip, color newColor, \
														unsigned int _Begin, unsigned int _End);
// Reversed every status of the strip.
extern void strip_StatusInverter(pixel* _addMatrix);


#endif
// End of definition.
