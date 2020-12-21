/*-->	Title	:	KBP_NeoPixel_SM
		  |
		  '->	Source	:	Timers_Wait_Xms.h (One of my own first header file)
		  |
		  '->	Creation	:	21.11.2019
		  :			|
		  :			'->	Last Update	:	March 2020
		  |
		  '->	Description	:	Declarations of NeoPixels Usage functions.
*/
/*======================================================================================>
=======================================================================================*/
// Linker to : ...
#include <c8051f020.h>		// ... definition folder SFR
													// (Like Port Definition "P5", "P6", etc...)
// ... C51/C166 routines that instructs the compiler to generate intrinsic code | _nop_():
#include <intrins.h>
//#include "../F12-NeoPix_MatrixPrButt/base_sfr.h"	// ... Base (SFR, sbit, define, var. type, etc...)
//#include "../F12-NeoPix_MatrixPrButt/time.h"		// ... Timers function and Waiting Function.
#include "ws2812_matrix.h"	// ... Timers function and Waiting Function.
#include "kc5_bp_matrix_alphanum.h"	// ... Personal alphabet and number's font.

//-- GLOBAL VARIABLES	INIT:------------------------------->
xdata pixel matrix[MAX_LEDS] = {{0, 0, 0}, 0, 0};   // Initialize it to '0'.
																				// & the Position is a Var. that can be unused.
xdata offsetText Offset = {0, 0};
xdata matrixFormat matrixDisplay = {0, 0};

/* Graphic     :
 *	Initial : 0/+-360 degrees :
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
unsigned int pixel_PosRecovery(unsigned char posX, unsigned char posY)
{	// Var. Dec. :
   unsigned char ui8_newX = 0, ui8_newY = 0;
   unsigned char ui8_X = 0, ui8_Y = 0;
   unsigned int ui16_matOffset = 0;
   unsigned int ui16_returnPos = 0;

	// Condition about starting from 1, instead of 0.
	if(posX == 0) { ui8_X = 1; } else { ui8_X = posX;}
	if(posY == 0) { ui8_Y = 1; } else { ui8_Y = posY;}

	switch(matrixDisplay.matrixView)
	{
		case 0:
		case 360:
		case (-360):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((posY % 2) == 0)
			{
				if(ui8_X <= MAX_COLUMN)
				{
					ui8_newX = MAX_COLUMN - ui8_X;
					ui16_matOffset = 0;
				}
				else
				{
					if(ui8_X <= (2 * MAX_COLUMN))
					{
						ui8_newX = (2 * MAX_COLUMN) - ui8_X;
						ui16_matOffset = MAX_COLUMN * MAX_LINE;
					}
					else
					{
						ui8_newX = (3 * MAX_COLUMN) - ui8_X;
						ui16_matOffset = 2 * MAX_COLUMN * MAX_LINE;
					}
				}
			}
			else
			{
				if(ui8_X <= MAX_COLUMN)
				{
					ui8_newX = (ui8_X - 1);
					ui16_matOffset = 0;
				}
				else
				{
					if(ui8_X <= (2 * MAX_COLUMN))
					{
						ui8_newX = (ui8_X - MAX_COLUMN - 1);
						ui16_matOffset = MAX_COLUMN * MAX_LINE;
					}
					else
					{
						ui8_newX = (ui8_X - (2 * MAX_COLUMN) - 1);
						ui16_matOffset = 2 * MAX_COLUMN * MAX_LINE;
					}
				}
			}
			ui16_returnPos = (((ui8_Y - 1) * MAX_COLUMN) + ui8_newX) + ui16_matOffset;
			break;


		case 90:
		case (-270):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((posX % 2) == 0)
			{
				ui8_newY = (MAX_LINE - 1) - posY;
			}
			else
			{
				ui8_newY = posY;
			}
			ui16_returnPos = (((posX - 1) * MAX_COLUMN) + ui8_newY);
			break;

		case 180:
		case (-180):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((posY % 2) == 0)
			{
				ui8_newX = MAX_COLUMN - ui8_X;
			}
			else
			{
				ui8_newX = (ui8_X - 1);
			}
			ui16_returnPos = (((ui8_Y - 1) * MAX_COLUMN) + ui8_newX);
			break;


		case 270:
		case (-90):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((posX % 2) == 0)
			{
				ui8_newY = (MAX_LINE - 1) - posY;
			}
			else
			{
				ui8_newY = posY;
			}
			ui16_returnPos = (((posX - 1) * MAX_COLUMN) + ui8_newY); // Checker les PosRecov en rotation.
			break;

		default :	/* State of every undefined case, so not supposed to come here. */	break;
	}
	return ui16_returnPos; // Return Pos.
}

void pixel_SetColor(pixel* addressMatrix, color newColor, unsigned int position)
{	// Var. Dec. :
	// To avoid problems about the parameters, I preferred to work with copies.
	unsigned char Red = newColor.Red;
   unsigned char Green = newColor.Green;
   unsigned char Blue = newColor.Blue;

	addressMatrix += (position - 1);	// go to the wanted LED position.

	// Set "color" values :
   addressMatrix->colorPix.Red = Red;
   addressMatrix->colorPix.Green = Green;
   addressMatrix->colorPix.Blue = Blue;

   // Set the position info. in the structure :
   addressMatrix->pos = position;

   // Set the status to ON :
   addressMatrix->status = 1;
}

void pixel_Show(unsigned char red, unsigned char green, unsigned char blue)
{	// Var. Dec. :
   // To avoid problems about the parameters, I preferred to work with copies.
	unsigned char tmpRed = red;
	unsigned char tmpGreen = green;
	unsigned char tmpBlue = blue;

	// For the WS2812b, the order is High bit to low AND Green - Red - Blue.
	// Sending GREEN.
	SEND_TO_LED(tmpGreen, BIT7);
	SEND_TO_LED(tmpGreen, BIT6);
	SEND_TO_LED(tmpGreen, BIT5);
	SEND_TO_LED(tmpGreen, BIT4);
	SEND_TO_LED(tmpGreen, BIT3);
	SEND_TO_LED(tmpGreen, BIT2);
	SEND_TO_LED(tmpGreen, BIT1);
	SEND_TO_LED(tmpGreen, BIT0);
	// Sending RED.
	SEND_TO_LED(tmpRed, BIT7);
	SEND_TO_LED(tmpRed, BIT6);
	SEND_TO_LED(tmpRed, BIT5);
	SEND_TO_LED(tmpRed, BIT4);
	SEND_TO_LED(tmpRed, BIT3);
	SEND_TO_LED(tmpRed, BIT2);
	SEND_TO_LED(tmpRed, BIT1);
	SEND_TO_LED(tmpRed, BIT0);
	// Sending BLUE.
	SEND_TO_LED(tmpBlue, BIT7);
	SEND_TO_LED(tmpBlue, BIT6);
	SEND_TO_LED(tmpBlue, BIT5);
	SEND_TO_LED(tmpBlue, BIT4);
	SEND_TO_LED(tmpBlue, BIT3);
	SEND_TO_LED(tmpBlue, BIT2);
	SEND_TO_LED(tmpBlue, BIT1);
	SEND_TO_LED(tmpBlue, BIT0);
}


void matrix_Clear(pixel* addressMatrix)
{	// Var. Dec. :
	unsigned int i;
	pixel* addressSave = addressMatrix;

	for(i = 0; i < MAX_LEDS; i++)
	{	// Clear ALL Color in the original addresses ...
      addressMatrix->colorPix.Red = BRIGHT_MIN;
      addressMatrix->colorPix.Green = BRIGHT_MIN;
      addressMatrix->colorPix.Blue = BRIGHT_MIN;

      addressMatrix->pos = 0; // Clear position ..

      addressMatrix->status = 0; // Clear status ..

      addressMatrix++;  // Increase address for clearing next position.
	}
	matrix_Show(addressSave);
}

void matrix_StatusReset(pixel* addressMatrix)
{	// Var. Dec. :
	unsigned int i;

	for(i = 0; i < MAX_LEDS; i++)
	{	// Clear Status to keep color in the original addresses ...
      addressMatrix->status = 0;

      addressMatrix++;  // Increase address for clearing next position.
	}
}

void matrix_Show(pixel* addressMatrix)
{	// Var. Dec. :
	xdata uint16 i;

	// Disable Timer to avoid interrupting Sending Paquets :
	TR0 = 0;

	for(i = 0; i < MAX_LEDS; i++)
	{
		pixel_Show(addressMatrix->colorPix.Red, addressMatrix->colorPix.Green, addressMatrix->colorPix.Blue);
		addressMatrix++;
	}

	// Enable / Re-activate Timer after Paquets Sent :
	TR0 = 1;
}


void matrix_SetTextPos(unsigned char  column, unsigned char line)
{
   Offset.column = column;
   Offset.line = line;
}

void matrix_Print(pixel* addressMatrix, color newColor, const char* _CharToWrite, ...)
{	// Var. Dec. :
	xdata unsigned int i;	// Y and Filling
	xdata unsigned char j;	// X
	xdata unsigned char Cnt_Lin = 0;	// For Char with different : Line
	xdata unsigned char Cnt_Col = 0;	// For Char with different : Colu
	const char* cP_CharToPrint;	// Recover the actual char to print.
	xdata unsigned int NeoPix_Pos = 0;	// Position recovery from an int to
	xdata unsigned int CharReadOrga = 0;	// Organisation about Reading the Char depending on the Matrix' Orientation.
//
	for(cP_CharToPrint = _CharToWrite; *cP_CharToPrint != '\0'; cP_CharToPrint++)
	{
		switch(*cP_CharToPrint)
		{	// Height & Width of letters can be different so a first Switch..Case is necessary.
			// Letters : --------------------------------------------------->
			case 'A':	case 'B':	case 'C':	case 'D':	case 'E':   case 'F':
			case 'G':	case 'H':	case 'I':	case 'J':   case 'K':   case 'L':
			case 'N':	case 'O':	case 'P':   case 'Q':	case 'R':	case 'S':
			case 'T':	case 'U':   case 'X':	case 'Y':	case 'Z':
				Cnt_Col = 4;
				Cnt_Lin = 7;
				break;
			case 'M':	case 'V':	case 'W':
				Cnt_Col = 5;
				Cnt_Lin = 7;
				break;
			// Numbers + Style : ------------------------------------------->
			case '0':	case '1':	case '2':	case '3':	case '4':
			case '5':	case '6':	case '7':	case '8':	case '9':
				switch(matrixDisplay.txtFont)
				{
 					case numberClassic:
						Cnt_Col = 4;
						Cnt_Lin = 7;
						break;
					case numberTime:
						Cnt_Col = 5;
						Cnt_Lin = 7;
						break;
					case numberMob:
						Cnt_Col = 4;
						Cnt_Lin = 6;
						break;
					default :	/* State of every undefined case, so not supposed to come here. */	break;
				}
				break;
			// Special Char. : --------------------------------------------->
			case ' ':	case '.':   case ',':   case '!':
			case ':':	case '=':   case '*':   case '-':
			case '+':	case '%':   /*case '°':*/   case '/':
				Cnt_Col = 3;
				Cnt_Lin = 7;
				break;

         case '\'':
				Cnt_Col = 2;
				Cnt_Lin = 7;
				break;

        case '(':	case ')':   case '{':   case '}':
        case '[':	case ']':   case '~':   case '@':
        case '#':	case '&':
				Cnt_Col = 5;
				Cnt_Lin = 7;
				break;

        case '_':
				Cnt_Col = 4;
				Cnt_Lin = 7;
				break;

			case '^':
				Cnt_Col = 7;
				Cnt_Lin = 6;
				break;

			case '?':
				switch(matrixDisplay.txtFont)
				{
					case numberClassic:
						Cnt_Col = 4;
						Cnt_Lin = 7;
						break;
					case numberMob:
						Cnt_Col = 5;
						Cnt_Lin = 7;
						break;
					default :	/* State of every undefined case, so not supposed to come here. */	break;
				}
				break;

			default :	/* State of every undefined case, so not supposed to come here. */	break;
		}

		for(i = 0; i < Cnt_Lin; i++)
		{
			for(j = 0; j < Cnt_Col; j++)
			{	// Same Line :
				NeoPix_Pos = pixel_PosRecovery((j+Offset.column), i+Offset.line);
				// Condition about possible value for displaying or not.
				if(NeoPix_Pos >= ((MAX_COLUMN * MAX_LINE) * MATRIX_NBR))
				{
					break;
				}
				else { /* Nothing. */ }
				// Switch about definning how the char Board must be read.
				switch(matrixDisplay.matrixView)
				{
					case 0:
					case 180:
					case 360:
					case (-180):
					case (-360):
						CharReadOrga = ((i * Cnt_Col) - j);
						break;
					case 90:
					case 270:
					case (-90):
					case (-270):
						CharReadOrga = (((i + 1) * Cnt_Col) - (j + 1));
						break;
					default :	/* State of every undefined case, so not supposed to come here. */	break;
				}

				switch(*cP_CharToPrint)
				{
					case 'A':
						addressMatrix[NeoPix_Pos].status = cBoard_A[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'B':
						addressMatrix[NeoPix_Pos].status = cBoard_B[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'C':
						addressMatrix[NeoPix_Pos].status = cBoard_C[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'D':
						addressMatrix[NeoPix_Pos].status = cBoard_D[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'E':
						addressMatrix[NeoPix_Pos].status = cBoard_E[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'F':
						addressMatrix[NeoPix_Pos].status = cBoard_F[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'G':
						addressMatrix[NeoPix_Pos].status = cBoard_G[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'H':
						addressMatrix[NeoPix_Pos].status = cBoard_H[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'I':
						addressMatrix[NeoPix_Pos].status = cBoard_I[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'J':
						addressMatrix[NeoPix_Pos].status = cBoard_J[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'K':
						addressMatrix[NeoPix_Pos].status = cBoard_K[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'L':
						addressMatrix[NeoPix_Pos].status = cBoard_L[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'M':
						addressMatrix[NeoPix_Pos].status = cBoard_M[OFF_CHAR_DIM_5X7 - CharReadOrga];
						break;
					case 'N':
						addressMatrix[NeoPix_Pos].status = cBoard_N[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'O':
						addressMatrix[NeoPix_Pos].status = cBoard_O[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'P': //
						addressMatrix[NeoPix_Pos].status = cBoard_P[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'Q':
						addressMatrix[NeoPix_Pos].status = cBoard_Q[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'R':
						addressMatrix[NeoPix_Pos].status = cBoard_R[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'S':
						addressMatrix[NeoPix_Pos].status = cBoard_S[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'T':
						addressMatrix[NeoPix_Pos].status = cBoard_T[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'U':
						addressMatrix[NeoPix_Pos].status = cBoard_U[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'V':
						addressMatrix[NeoPix_Pos].status = cBoard_V[OFF_CHAR_DIM_5X7 - CharReadOrga];
						break;
					case 'W':
						addressMatrix[NeoPix_Pos].status = cBoard_W[OFF_CHAR_DIM_5X7 - CharReadOrga];
						break;
					case 'X':
						addressMatrix[NeoPix_Pos].status = cBoard_X[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'Y':
						addressMatrix[NeoPix_Pos].status = cBoard_Y[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
					case 'Z':
						addressMatrix[NeoPix_Pos].status = cBoard_Z[OFF_CHAR_DIM_4X7 - CharReadOrga];
						break;
//
					case '0':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_0_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_0_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_0_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '1':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_1_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_1_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_1_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '2':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_2_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_2_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_2_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '3':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_3_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_3_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_3_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '4':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_4_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_4_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_4_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '5':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_5_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_5_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_5_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '6':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_6_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_6_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_6_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '7':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_7_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_7_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_7_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '8':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_8_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_8_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_8_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case '9':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_9_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberTime:
								addressMatrix[NeoPix_Pos].status = cBoard_9_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_9_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;

					case '.':
						addressMatrix[NeoPix_Pos].status = cBoard_Dot[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;
					case ',':
						addressMatrix[NeoPix_Pos].status = cBoard_Comma[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;
					case '!':
						addressMatrix[NeoPix_Pos].status = cBoard_ExclaMark[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;
					case '?':
						switch(matrixDisplay.txtFont)
						{
							case numberClassic:
								addressMatrix[NeoPix_Pos].status = cBoard_QuestMark[OFF_CHAR_DIM_4X7 - CharReadOrga];
								break;
							case numberMob:
								addressMatrix[NeoPix_Pos].status = cBoard_QuestMark_Mob[OFF_CHAR_DIM_5X7 - CharReadOrga];
								break;
							default :	/* State of every undefined case, so not supposed to come here. */	break;
						}
						break;
					case ':':
						addressMatrix[NeoPix_Pos].status = cBoard_Colon[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;

					case '^':
						addressMatrix[NeoPix_Pos].status = cBoard_Heart[OFF_CHAR_DIM_7X6 - CharReadOrga];
						break;

					case '\'':
						addressMatrix[NeoPix_Pos].status = cBoard_Apost[OFF_CHAR_DIM_2X7 - CharReadOrga];
						break;

					case '*':
						addressMatrix[NeoPix_Pos].status = cBoard_Asterisk[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;

					case ' ':
						addressMatrix[NeoPix_Pos].status = cBoard_Space[OFF_CHAR_DIM_3X7 - CharReadOrga];
						break;

					case '\n':
						Offset.column = 1;
						Offset.line -= 8;	// Decrease Line of Height + 1 line to make a space.
						//Offset.line = 3;
						break;
					
					default :	/* State of every undefined case, so not supposed to come here. */	break;
				}
				
				// Coloring only One char at a Time :
				if(addressMatrix[NeoPix_Pos].status != 0)
				{
					addressMatrix[NeoPix_Pos].colorPix.Red = newColor.Red;
					addressMatrix[NeoPix_Pos].colorPix.Green = newColor.Green;
					addressMatrix[NeoPix_Pos].colorPix.Blue = newColor.Blue;
				}
				else
				{
					addressMatrix[NeoPix_Pos].colorPix.Red = BRIGHT_MIN;
					addressMatrix[NeoPix_Pos].colorPix.Green = BRIGHT_MIN;
					addressMatrix[NeoPix_Pos].colorPix.Blue = BRIGHT_MIN;
				}
			}
		}
         // Space auto management while entering multiple char.
		if(*cP_CharToPrint != '\n')
		{
			// Position Increment on X :
			Offset.column += (Cnt_Col + 1);
		}
		else
		{ /* Nothin', avoid increase Col. after a \n. */ }
	}
}

void strip_LedChain(pixel* addressMatrix, color newColor, unsigned int start, unsigned int end)
{	// Var. Dec. :
   unsigned int i;
   pixel* addressSave = addressMatrix;

	for(i = 0; i < MAX_LEDS; i++)
	{
		if((i >= start) && (i <= end))
		{
         addressMatrix->colorPix.Red = newColor.Red;
         addressMatrix->colorPix.Green = newColor.Green;
         addressMatrix->colorPix.Blue = newColor.Blue;
		}
		else
		{
         addressMatrix->colorPix.Red = BRIGHT_MIN;
         addressMatrix->colorPix.Green = BRIGHT_MIN;
         addressMatrix->colorPix.Blue = BRIGHT_MIN;
		}
    addressMatrix++;
	}
	matrix_Show(addressSave);
}
