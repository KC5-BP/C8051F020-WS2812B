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
// .. definition folder SFR (Like Port Definition "P5", "P6", "TR0", etc...)
#include <c8051f020.h> // <= Would delete it ...
// .. created header for ws2812b led usage.
#include "../led_manip/ws2812_led_manip.h"  // For the << pixel >> struct.
// .. created header for ws2812b matrix usage.
#include "ws2812_mx_manip.h"
// .. character's definitions.
#include "characters/kc5_bp_matrix_alphanum.h"
// .. character's BIG definitions (Letter's height : X14).
//#include "characters/kc5_bp_matrix_alpha_big.h"

//-- GLOBAL VARIABLES INIT : ----------------------------->
xdata offsetText Offset = {0, 0};
//xdata offsetText Offset = (offsetText) { .column = 0, .line = 0 };
xdata matrixFormat formatDisplay = {0, CLASSIC};
//xdata matrixFormat formatDisplay = { .orientation = 0, .font = CLASSIC };

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
#ifdef __KC5_BP_MATRIX_ALPHANUM__
//======================================================================================>
/* Description  :   Retrieve Width and Height of a character.						   */
void retrieveWidthAndHeightOf(const char* character, \
                                            unsigned char* width, unsigned char* height);
//======================================================================================>
unsigned char getPixelStatusFromChar(const char* character, unsigned int x, unsigned int y, unsigned char width);
#endif
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
#ifdef __KC5_BP_MATRIX_ALPHANUM__
void matrix_SetTextPosition(unsigned char column, unsigned char line) {
	// COLUMN validity ..
	/*if(column >= MAX_COLU) Offset.column = MAX_COLU - 1;
	else Offset.column = column;

	// LINE validity ..
	if(line >= MAX_LINE) Offset.line = MAX_LINE - 1;
	else Offset.line = line;*/  // Problem with sliding if condition lept because force to MAX.

	Offset.column = column;
	Offset.line = line;
}
//======================================================================================>
void matrix_Print(pixel* addressMatrix, color newColor, const char* _CharToWrite) {
	xdata unsigned int _x, _y;
	xdata unsigned char charWidth, charHeight;
	const char* character;
	xdata posType pixelPos = 0;

	for(character = _CharToWrite; *character != '\0'; character++) {
		retrieveWidthAndHeightOf(character, &charWidth, &charHeight);

		for(_y = 0; _y < charHeight; ++_y) {
			for(_x = 0; _x < charWidth; ++_x) {
				pixelPos = pixel_RecoverPosition(_x + Offset.column, _y + Offset.line);
				// Condition about possible value for displaying or not.
				if(pixelPos >= ((MAX_COLU * MAX_LINE) * NBR_OF_MATRIX))
					continue;   // Restart loop with following value of j.

				switch(*character) {
					case ' ':
						addressMatrix[pixelPos].status = cBoard_Space[_x + _y * charWidth];
						break;
					case '!':
						addressMatrix[pixelPos].status = cBoard_ExclaMark[_x + _y * charWidth];
						break;
					//case '\"':
					case '#':
						//addressMatrix[pixelPos].status = cBoard_HashtagV1[_x + _y * charWidth];
						addressMatrix[pixelPos].status = cBoard_HashtagV2[_x + _y * charWidth];
						break;
					//case '$':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case '%':
						addressMatrix[pixelPos].status = cBoard_xMult[_x + _y * charWidth];
						break;
					case '&':
						addressMatrix[pixelPos].status = cBoard_Ampersand[_x + _y * charWidth];
						break;
					case '\'':
						addressMatrix[pixelPos].status = cBoard_Apost[_x + _y * charWidth];
						break;
					case '(':
						addressMatrix[pixelPos].status = cBoard_OpenBracket[_x + _y * charWidth];
						break;
					case ')':
						addressMatrix[pixelPos].status = cBoard_CloseBracket[_x + _y * charWidth];
						break;
					case '*':
						addressMatrix[pixelPos].status = cBoard_Asterisk[_x + _y * charWidth];
						break;
					case '+':
						addressMatrix[pixelPos].status = cBoard_Plus[_x + _y * charWidth];
						break;
					case ',':
						addressMatrix[pixelPos].status = cBoard_Comma[_x + _y * charWidth];
						break;
					case '-':
						addressMatrix[pixelPos].status = cBoard_Hyphen[_x + _y * charWidth];
						break;
					case '.':
						addressMatrix[pixelPos].status = cBoard_Dot[_x + _y * charWidth];
						break;
					case '/':
						addressMatrix[pixelPos].status = cBoard_Slash[_x + _y * charWidth];
						break;

					case '0':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_0_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_0_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_0_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '1':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_1_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_1_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_1_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '2':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_2_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_2_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_2_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '3':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_3_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_3_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_3_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '4':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_4_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_4_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_4_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '5':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_5_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_5_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_5_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '6':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_6_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_6_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_6_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '7':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_7_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_7_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_7_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '8':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_8_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_8_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_8_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '9':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_9_Val[_x + _y * charWidth];
								break;
							case TIME:
								addressMatrix[pixelPos].status = cBoard_9_Time[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_9_Mob[_x + _y * charWidth];
								break;
						}
						break;

					case ':':
						addressMatrix[pixelPos].status = cBoard_Colon[_x + _y * charWidth];
						break;
					case ';':
						addressMatrix[pixelPos].status = cBoard_SemiColon[_x + _y * charWidth];
						break;
					//case '<':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case '=':
						addressMatrix[pixelPos].status = cBoard_Equal[_x + _y * charWidth];
						break;
					//case '>':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case '?':
						switch(formatDisplay.font) {
							case CLASSIC:
								addressMatrix[pixelPos].status = cBoard_QuestMark[_x + _y * charWidth];
								break;
							case MOB:
							default :
								addressMatrix[pixelPos].status = cBoard_QuestMark_Mob[_x + _y * charWidth];
								break;
						}
						break;
					case '@':
						addressMatrix[pixelPos].status = cBoard_At[_x + _y * charWidth];
						break;

					case 'A':
						addressMatrix[pixelPos].status = cBoard_A[_x + _y * charWidth];
						break;
					case 'B':
						addressMatrix[pixelPos].status = cBoard_B[_x + _y * charWidth];
						break;
					case 'C':
						addressMatrix[pixelPos].status = cBoard_C[_x + _y * charWidth];
						break;
					case 'D':
						addressMatrix[pixelPos].status = cBoard_D[_x + _y * charWidth];
						break;
					case 'E':
						addressMatrix[pixelPos].status = cBoard_E[_x + _y * charWidth];
						break;
					case 'F':
						addressMatrix[pixelPos].status = cBoard_F[_x + _y * charWidth];
						break;
					case 'G':
						addressMatrix[pixelPos].status = cBoard_G[_x + _y * charWidth];
						break;
					case 'H':
						addressMatrix[pixelPos].status = cBoard_H[_x + _y * charWidth];
						break;
					case 'I':
						addressMatrix[pixelPos].status = cBoard_I[_x + _y * charWidth];
						break;
					case 'J':
						addressMatrix[pixelPos].status = cBoard_J[_x + _y * charWidth];
						break;
					case 'K':
						addressMatrix[pixelPos].status = cBoard_K[_x + _y * charWidth];
						break;
					case 'L':
						addressMatrix[pixelPos].status = cBoard_L[_x + _y * charWidth];
						break;
					case 'M':
						addressMatrix[pixelPos].status = cBoard_M[_x + _y * charWidth];
						break;
					case 'N':
						addressMatrix[pixelPos].status = cBoard_N[_x + _y * charWidth];
						break;
					case 'O':
						addressMatrix[pixelPos].status = cBoard_O[_x + _y * charWidth];
						break;
					case 'P':
						addressMatrix[pixelPos].status = cBoard_P[_x + _y * charWidth];
						break;
					case 'Q':
						addressMatrix[pixelPos].status = cBoard_Q[_x + _y * charWidth];
						break;
					case 'R':
						addressMatrix[pixelPos].status = cBoard_R[_x + _y * charWidth];
						break;
					case 'S':
						addressMatrix[pixelPos].status = cBoard_S[_x + _y * charWidth];
						break;
					case 'T':
						addressMatrix[pixelPos].status = cBoard_T[_x + _y * charWidth];
						break;
					case 'U':
						addressMatrix[pixelPos].status = cBoard_U[_x + _y * charWidth];
						break;
					case 'V':
						addressMatrix[pixelPos].status = cBoard_V[_x + _y * charWidth];
						break;
					case 'W':
						addressMatrix[pixelPos].status = cBoard_W[_x + _y * charWidth];
						break;
					case 'X':
						addressMatrix[pixelPos].status = cBoard_X[_x + _y * charWidth];
						break;
					case 'Y':
						addressMatrix[pixelPos].status = cBoard_Y[_x + _y * charWidth];
						break;
					case 'Z':
						addressMatrix[pixelPos].status = cBoard_Z[_x + _y * charWidth];
						break;

					case '[':
						addressMatrix[pixelPos].status = cBoard_OpenSqBracket[_x + _y * charWidth];
						break;
					//case '\\':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case ']':
						addressMatrix[pixelPos].status = cBoard_CloseSqBracket[_x + _y * charWidth];
						break;
					case '^':
						addressMatrix[pixelPos].status = cBoard_Heart[_x + _y * charWidth];
						break;
					case '_':
						addressMatrix[pixelPos].status = cBoard_Underscore[_x + _y * charWidth];
						break;
					//case '`':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case '{':
						addressMatrix[pixelPos].status = cBoard_OpenRoBracket[_x + _y * charWidth];
						break;
					//case '|':
					//  addressMatrix[pixelPos].status = ;
					//  break;
					case '}':
						addressMatrix[pixelPos].status = cBoard_CloseRoBracket[_x + _y * charWidth];
						break;
					case '~':
						addressMatrix[pixelPos].status = cBoard_Tilda[_x + _y * charWidth];
						break;

					case '\n':
						// Reset X
						Offset.column = 0;
						// Decrease Line of Height + 1 line to make a space.
						Offset.line -= 8;
						break;

					//State of every undefined case, so not supposed to come here.
					default :   break;
				}

				//addressMatrix[pixelPos].status = getPixelStatusFromChar(character, _x, _y, charWidth);

				// Coloring only One char at a Time :
				if(addressMatrix[pixelPos].status != 0)
					addressMatrix[pixelPos].colorPix = newColor;
				else
					addressMatrix[pixelPos].colorPix = BLACK;
			}
		}
		// Space auto management while entering multiple char. except with '\n'
		if(*character != '\n')
			Offset.column += (charWidth + 1);
	}
	//leds_Show(addressMatrix); // Crash with it and I need to find out why ...
}

void retrieveWidthAndHeightOf(const char* character, \
										unsigned char* width, unsigned char* height) {
	switch(*character) {
		// Letters : ---------------------------------------------------->
		case 'A':	case 'B':	case 'C':   case 'D':	case 'E':   case 'F':   case 'G':
		case 'H':	case 'J':   case 'K':   case 'L':   case 'N':   case 'O':	case 'P':
		case 'Q':	case 'R':	case 'S':   case 'U':   case 'X':	case 'Y':	case 'Z':
		case '_':
			*width = 4;
			*height = 7;
			break;
		case 'I':case 'T':
		case ' ':	case '.':   case ',':   case '!':
		case ':':	case '=':   case '*':   case '-':
		case '+':	case '%':   /*case '°':*/   case '/':
			*width = 3;
			*height = 7;
			break;
		case 'M':	case 'V':	case 'W':
		case '(':	case ')':   case '{':   case '}':
		case '[':	case ']':   case '~':   case '@':
		case '#':	case '&':
			*width = 5;
			*height = 7;
			break;

		// Numbers + Style : -------------------------------------------->
		case '0':	case '1':	case '2':	case '3':	case '4':
		case '5':	case '6':	case '7':	case '8':	case '9':
			switch(formatDisplay.font) {
				case CLASSIC:
				default:
					*width = 4;
					*height = 7;
					break;
				case TIME:
					*width = 5;
					*height = 7;
					break;
				case MOB:
					*width = 4;
					*height = 6;
					break;
			}
			break;

		// Special Char. : ---------------------------------------------->
		case '\'':
			*width = 2;
			*height = 7;
			break;

		case '^':
			*width = 7;
			*height = 6;
			break;

		case '?':
			switch(formatDisplay.font) {
				case CLASSIC:
					*width = 4;
					*height = 7;
					break;
				case MOB:
					*width = 5;
					*height = 7;
					break;
				/* State of every undefined case, so not supposed to come here. */
				default :	break;
			}
			break;

		/* State of every undefined case, so not supposed to come here. */
		default :   break;
	}
}
/*
unsigned char getPixelStatusFromChar(const char* character, unsigned int x, unsigned int y, unsigned char width) {
 	unsigned char status = 0;
	switch(*character) {
		case ' ':
			status = cBoard_Space[x + y * width];
			break;
		case '!':
			status = cBoard_ExclaMark[x + y * width];
			break;
		//case '\"':
		case '#':
			//status = cBoard_HashtagV1[x + y * width];
			status = cBoard_HashtagV2[x + y * width];
			break;
		//case '$':
		//  status = ;
		//  break;
		case '%':
			status = cBoard_xMult[x + y * width];
			break;
		case '&':
			status = cBoard_Ampersand[x + y * width];
			break;
		case '\'':
			status = cBoard_Apost[x + y * width];
			break;
		case '(':
			status = cBoard_OpenBracket[x + y * width];
			break;
		case ')':
			status = cBoard_CloseBracket[x + y * width];
			break;
		case '*':
			status = cBoard_Asterisk[x + y * width];
			break;
		case '+':
			status = cBoard_Plus[x + y * width];
			break;
		case ',':
			status = cBoard_Comma[x + y * width];
			break;
		case '-':
			status = cBoard_Hyphen[x + y * width];
			break;
		case '.':
			status = cBoard_Dot[x + y * width];
			break;
		case '/':
			status = cBoard_Slash[x + y * width];
			break;

		case '0':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_0_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_0_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_0_Mob[x + y * width];
					break;
			}
			break;
		case '1':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_1_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_1_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_1_Mob[x + y * width];
					break;
			}
			break;
		case '2':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_2_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_2_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_2_Mob[x + y * width];
					break;
			}
			break;
		case '3':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_3_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_3_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_3_Mob[x + y * width];
					break;
			}
			break;
		case '4':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_4_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_4_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_4_Mob[x + y * width];
					break;
			}
			break;
		case '5':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_5_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_5_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_5_Mob[x + y * width];
					break;
			}
			break;
		case '6':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_6_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_6_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_6_Mob[x + y * width];
					break;
			}
			break;
		case '7':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_7_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_7_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_7_Mob[x + y * width];
					break;
			}
			break;
		case '8':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_8_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_8_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_8_Mob[x + y * width];
					break;
			}
			break;
		case '9':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_9_Val[x + y * width];
					break;
				case TIME:
					status = cBoard_9_Time[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_9_Mob[x + y * width];
					break;
			}
			break;

		case ':':
			status = cBoard_Colon[x + y * width];
			break;
		case ';':
			status = cBoard_SemiColon[x + y * width];
			break;
		//case '<':
		//  status = ;
		//  break;
		case '=':
			status = cBoard_Equal[x + y * width];
			break;
		//case '>':
		//  status = ;
		//  break;
		case '?':
			switch(formatDisplay.font) {
				case CLASSIC:
					status = cBoard_QuestMark[x + y * width];
					break;
				case MOB:
				default :
					status = cBoard_QuestMark_Mob[x + y * width];
					break;
			}
			break;
		case '@':
			status = cBoard_At[x + y * width];
			break;

		case 'A':
			status = cBoard_A[x + y * width];
			break;
		case 'B':
			status = cBoard_B[x + y * width];
			break;
		case 'C':
			status = cBoard_C[x + y * width];
			break;
		case 'D':
			status = cBoard_D[x + y * width];
			break;
		case 'E':
			status = cBoard_E[x + y * width];
			break;
		case 'F':
			status = cBoard_F[x + y * width];
			break;
		case 'G':
			status = cBoard_G[x + y * width];
			break;
		case 'H':
			status = cBoard_H[x + y * width];
			break;
		case 'I':
			status = cBoard_I[x + y * width];
			break;
		case 'J':
			status = cBoard_J[x + y * width];
			break;
		case 'K':
			status = cBoard_K[x + y * width];
			break;
		case 'L':
			status = cBoard_L[x + y * width];
			break;
		case 'M':
			status = cBoard_M[x + y * width];
			break;
		case 'N':
			status = cBoard_N[x + y * width];
			break;
		case 'O':
			status = cBoard_O[x + y * width];
			break;
		case 'P':
			status = cBoard_P[x + y * width];
			break;
		case 'Q':
			status = cBoard_Q[x + y * width];
			break;
		case 'R':
			status = cBoard_R[x + y * width];
			break;
		case 'S':
			status = cBoard_S[x + y * width];
			break;
		case 'T':
			status = cBoard_T[x + y * width];
			break;
		case 'U':
			status = cBoard_U[x + y * width];
			break;
		case 'V':
			status = cBoard_V[x + y * width];
			break;
		case 'W':
			status = cBoard_W[x + y * width];
			break;
		case 'X':
			status = cBoard_X[x + y * width];
			break;
		case 'Y':
			status = cBoard_Y[x + y * width];
			break;
		case 'Z':
			status = cBoard_Z[x + y * width];
			break;

		case '[':
			status = cBoard_OpenSqBracket[x + y * width];
			break;
		//case '\\':
		//  status = ;
		//  break;
		case ']':
			status = cBoard_CloseSqBracket[x + y * width];
			break;
		case '^':
			status = cBoard_Heart[x + y * width];
			break;
		case '_':
			status = cBoard_Underscore[x + y * width];
			break;
		//case '`':
		//  status = ;
		//  break;
		case '{':
			status = cBoard_OpenRoBracket[x + y * width];
			break;
		//case '|':
		//  status = ;
		//  break;
		case '}':
			status = cBoard_CloseRoBracket[x + y * width];
			break;
		case '~':
			status = cBoard_Tilda[x + y * width];
			break;

		case '\n':
			// Reset X
			Offset.column = 0;
			// Decrease Line of Height + 1 line to make a space.
			Offset.line -= 8;
			break;

		default :   break;
	}
	return status;
}*/
#endif