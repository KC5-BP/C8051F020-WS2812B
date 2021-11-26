/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_mx_manip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : ws2812_matrix.c
|x|  \|\| | | , /    | |            >   (first version of the actual header)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2019.11.21 (Initial creation date)
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                Declarations of RGB's ws2812b LEDs usage funct. .
|x|        /|| |  /                 as a matrix purpose.
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
// .. created header for ws2812b matrix usage.
#include "ws2812_mx_manip.h"

// .. definition folder SFR (Like Port Definition "P5", "P6", "TR0", etc...)
#include <c8051f020.h> // <= Would delete it ...
// .. created header for ws2812b led usage.
#include "ws2812_led_manip.h"  // For the << pixel >> struct.

//-- GLOBAL VARIABLES INIT : ----------------------------->
xdata matrixFormat formatDisplay = {0};
//xdata matrixFormat formatDisplay = { .orientation = 0, .font = CLASSIC };

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>

//===================================================
//===============================\FUNCTIONS'_Definition/================================>
//======================================================================================>
/*	Initial : 0/+-360 degrees :
 *          33 :  / - - - - - - - -  -  -  -  -  -  -  /
 * 			32 :  \ - - - - - - - -  -  -  -  -  -  -  \.
 * 			 1 :  - - - - - - - - -  -  -  -  -  -  -  /
 * Starting LED-> 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
 * Rotated : 90/-270 degrees :
 *       |   |   |   |   |   |   |   |   |   |  |  |  |  |  | | : 3
 *       |   |   |   |   |   |   |   |   |   |  |  |  |  |  | | : 2
 *       |   \   /   \   /   \   /   \   /   \  /  \  /  \  / | : 1
 *      256 225 224 193 192 161 160 129 128 97 96 65 64 33 32 1  <- Starting LED.
 * Rotated : 180/-180 degrees :
 *       \   -   -   -   -   -   -   -   -   -   -   -   -   -   -   \ : 224
 * 		/   -   -   -   -   -   -   -   -   -   -   -   -   -   -   / : 225
 * 		\   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - : 256
 *      241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256
 * Rotated : 270/-90 degrees :
 *       |  |  |  |  |  |  |   |   |   |   |   |   |   |   |   | : 243
 *       |  |  |  |  |  |  |   |   |   |   |   |   |   |   |   | : 242
 *       \  /  \  /  \  /  \   /   \   /   \   /   \   /   \   / : 241
 *      16 17 48 49 80 81 112 113 144 145 176 177 208 209 240 241                      */
posType pixel_RecoverPosition(unsigned char posX, unsigned char posY) {
	posType ui16_returnPos = 0;
	// Condition about exceeding/equaling the MAX_COLU or MAX_LINE.
	/*if (MAX_COLU < posX)  posX = MAX_COLU - 1;
	if (MAX_LINE < posY)  posY = MAX_LINE - 1;*/
	switch (formatDisplay.orientation) {
		case 0: case 360: case (-360):
		default :
			if (!(posY % 2))   // Even lines on the matrix physical view.
				ui16_returnPos = posX /
								 MAX_COLU * MAX_LEDS + posX % MAX_COLU + posY * MAX_COLU;
			else
				ui16_returnPos = posX /
								 	MAX_COLU * MAX_LEDS + (posY + 1) * MAX_COLU -
								 		posX % MAX_COLU - 1;
			break;

		case 90: case (-270):
			if (!(posX % 2))   // Even columns on the matrix physical view.
				ui16_returnPos = MAX_LEDS - posX * MAX_LINE - posY - 1;
			else
				ui16_returnPos = MAX_LEDS - (posX + 1) * MAX_LINE + posY;
			break;

		case 180: case (-180):
			if ((posY % 2) == 0) // Even reverted lines on the matrix physical view.
				ui16_returnPos = MAX_LEDS - (posY + 1) * MAX_COLU + posX;
			else
				ui16_returnPos = MAX_LEDS - posY * MAX_COLU - posX - 1;
			break;

		case 270: case (-90):
			if (!(posX % 2))   // Even reverted columns on the matrix physical view.
				ui16_returnPos = (posX + 1) * MAX_LINE - posY - 1;
			else
				ui16_returnPos = (posX * MAX_LINE) + posY;
			break;
	}
	return ui16_returnPos;
}

//======================================================================================>
