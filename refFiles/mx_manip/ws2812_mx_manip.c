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
#include "kc5_bp_matrix_alphanum.h"
// .. character's BIG definitions (Letter's height : X14).
//#include "kc5_bp_matrix_alpha_big.h"
//-- GLOBAL VARIABLES INIT : ----------------------------->
xdata offsetText Offset = {0, 0};           // Init. column + line to 0.
xdata matrixFormat formatDisplay = {0, 0};  // Init. matrixView + txtFont to 0.

//-------------------------------------------------------------------------------------->
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
unsigned int pixel_RecoverPosition(unsigned char posX, unsigned char posY)
{	// Var. Dec. :
    posType ui16_returnPos = 0;

    // Condition about starting from 1, instead of 0.
    if (posX == 0)  posX = 1;
    if (posY == 0)  posY = 1;

    switch(formatDisplay.matrixView)
    {
        case 0:
        case 360:
        case (-360):
            if( !(posY % 2) )   // Even lines on the matrix physical view.
                ui16_returnPos = (MAX_COLU - posX) + (posY - 1) * MAX_COLU;
            else
                ui16_returnPos = (posX - 1) + (posY - 1) * MAX_COLU;
            break;

        case 90:
        case (-270):
            if( !(posX % 2) )   // Even columns on the matrix physical view.
                ui16_returnPos = (MAX_LEDS - posX * MAX_LINE) + posY - 1;
            else
                ui16_returnPos = MAX_LEDS - posY - (posX - 1) * MAX_LINE;
            break;

        case 180:
        case (-180):
            if((posY % 2) == 0) // Even reverted lines on the matrix physical view.
                ui16_returnPos = MAX_LEDS - posX - (posY - 1) * MAX_COLU;
            else
                ui16_returnPos = MAX_LEDS - MAX_COLU + posX - 1 - (posY - 1) * MAX_COLU;
            break;

        case 270:
        case (-90):
            if( !(posX % 2) )   // Even reverted columns on the matrix physical view.
                ui16_returnPos = (posX - 1) * MAX_LINE + posY - 1;
            else
                ui16_returnPos = (posX * MAX_LINE - 1) - posY + 1;
            break;

        /* State of every undefined case, so not supposed to come here. */
        default :   break;
    }
    return ui16_returnPos; // Return Pos.
}

//-------------------------------------------------------------------------------------->
#ifdef __kc5_bp_matrix_alphanum__
void matrix_SetTextPosition(unsigned char column, unsigned char line) {
    // COLUMN validity ..
    if(column > MAX_COLU) Offset.column = MAX_COLU;
    else Offset.column = column;
    // LINE validity ..
    if(line > MAX_COLU) Offset.line = MAX_LINE;
    else Offset.line = line;
}

void matrix_Print(pixel* addressMatrix, color newColor, const char* _CharToWrite, ...) {
    // Var. Dec. :
	xdata unsigned int j, i;	// X/Y and Filling
    xdata unsigned char Cnt_Col, Cnt_Lin;	// For Char with different : Column size.
	                                        // For Char with different : Line size.
	const char* cP_CharToPrint;	// Recover the actual char to print.
	xdata posType NeoPix_Pos = 0;

	for(cP_CharToPrint = _CharToWrite; *cP_CharToPrint != '\0'; cP_CharToPrint++) {
        retrieveHeightWidthOf(cP_CharToPrint, &Cnt_Col, &Cnt_Lin);

		for(i = 0; i < Cnt_Lin; i++) {
			for(j = 0; j < Cnt_Col; j++) {
				NeoPix_Pos = pixel_RecoverPosition((j+Offset.column), i+Offset.line);
				// Condition about possible value for displaying or not.
				if(NeoPix_Pos >= ((MAX_COLU * MAX_LINE) * NBR_OF_MATRIX))
				{
					continue;   // Restart loop with following value of j.
				}

				switch(*cP_CharToPrint)
				{
                    case ' ':
                        addressMatrix[NeoPix_Pos].status = cBoard_Space[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '!':
                        addressMatrix[NeoPix_Pos].status = cBoard_ExclaMark[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '\"':
                    case '#':
                        addressMatrix[NeoPix_Pos].status = cBoard_HashtagV1[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '$':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '%':
                        addressMatrix[NeoPix_Pos].status = cBoard_xMult[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '&':
                        addressMatrix[NeoPix_Pos].status = cBoard_Ampersand[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '\'':
                        addressMatrix[NeoPix_Pos].status = cBoard_Apost[OFF_CHAR_DIM_2X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '(':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case ')':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '*':
                        addressMatrix[NeoPix_Pos].status = cBoard_Asterisk[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '+':
                        addressMatrix[NeoPix_Pos].status = cBoard_Plus[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case ',':
                        addressMatrix[NeoPix_Pos].status = cBoard_Comma[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '-':
                        addressMatrix[NeoPix_Pos].status = cBoard_Hyphen[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '.':
                        addressMatrix[NeoPix_Pos].status = cBoard_Dot[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '/':
                        addressMatrix[NeoPix_Pos].status = cBoard_Slash[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;

                    case '0':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '1':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '2':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '3':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '4':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '5':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '6':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '7':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '8':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '9':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Val[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Time[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Mob[OFF_CHAR_DIM_4X6 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;

                    case ':':
                        addressMatrix[NeoPix_Pos].status = cBoard_Colon[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case ';':
                        addressMatrix[NeoPix_Pos].status = cBoard_SemiColon[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '<':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '=':
                        addressMatrix[NeoPix_Pos].status = cBoard_Equal[OFF_CHAR_DIM_3X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '>':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '?':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_QuestMark[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_QuestMark_Mob[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                                break;
                        }
                        break;
                    case '@':
                        addressMatrix[NeoPix_Pos].status = cBoard_At[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;

                    case 'A':
						addressMatrix[NeoPix_Pos].status = cBoard_A[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'B':
						addressMatrix[NeoPix_Pos].status = cBoard_B[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'C':
						addressMatrix[NeoPix_Pos].status = cBoard_C[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'D':
						addressMatrix[NeoPix_Pos].status = cBoard_D[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'E':
						addressMatrix[NeoPix_Pos].status = cBoard_E[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'F':
						addressMatrix[NeoPix_Pos].status = cBoard_F[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'G':
						addressMatrix[NeoPix_Pos].status = cBoard_G[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'H':
						addressMatrix[NeoPix_Pos].status = cBoard_H[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'I':
						addressMatrix[NeoPix_Pos].status = cBoard_I[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'J':
						addressMatrix[NeoPix_Pos].status = cBoard_J[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'K':
						addressMatrix[NeoPix_Pos].status = cBoard_K[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'L':
						addressMatrix[NeoPix_Pos].status = cBoard_L[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'M':
						addressMatrix[NeoPix_Pos].status = cBoard_M[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'N':
						addressMatrix[NeoPix_Pos].status = cBoard_N[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'O':
						addressMatrix[NeoPix_Pos].status = cBoard_O[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'P': //
						addressMatrix[NeoPix_Pos].status = cBoard_P[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'Q':
						addressMatrix[NeoPix_Pos].status = cBoard_Q[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'R':
						addressMatrix[NeoPix_Pos].status = cBoard_R[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'S':
						addressMatrix[NeoPix_Pos].status = cBoard_S[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'T':
						addressMatrix[NeoPix_Pos].status = cBoard_T[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'U':
						addressMatrix[NeoPix_Pos].status = cBoard_U[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'V':
						addressMatrix[NeoPix_Pos].status = cBoard_V[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'W':
						addressMatrix[NeoPix_Pos].status = cBoard_W[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'X':
						addressMatrix[NeoPix_Pos].status = cBoard_X[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'Y':
						addressMatrix[NeoPix_Pos].status = cBoard_Y[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;
					case 'Z':
						addressMatrix[NeoPix_Pos].status = cBoard_Z[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
						break;

                    case '[':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenSqBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '\\':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case ']':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseSqBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
					case '^':
						addressMatrix[NeoPix_Pos].status = cBoard_Heart[OFF_CHAR_DIM_7X6 - ((-j) + Cnt_Lin * i)];
						break;
                    case '_':
                        addressMatrix[NeoPix_Pos].status = cBoard_Underscore[OFF_CHAR_DIM_4X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '`':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '{':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenRoBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    //case '|':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '}':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseRoBracket[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;
                    case '~':
                        addressMatrix[NeoPix_Pos].status = cBoard_Tilda[OFF_CHAR_DIM_5X7 - ((-j) + Cnt_Lin * i)];
                        break;

					case '\n':
						Offset.column = 1;
						Offset.line -= 8;	// Decrease Line of Height + 1 line to make a space.
						break;

					/* State of every undefined case, so not supposed to come here. */
					default :   break;
				}
				
				// Coloring only One char at a Time :
				if(addressMatrix[NeoPix_Pos].status != 0)
				{
                    addressMatrix[NeoPix_Pos].colorPix = newColor;
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
		{ /* Nothin', avoid increase Col. after a '\n'. */ }
	}
}

void retrieveHeightWidthOf(const char* character, posType* width, posType* height) {
    // Retrieve the Height & Width of a letter through addresses :
    switch(*character) {
        // Letters : --------------------------------------------------->
        case 'A':	case 'B':	case 'C':	case 'D':	case 'E':   case 'F':
        case 'G':	case 'H':	case 'I':	case 'J':   case 'K':   case 'L':
        case 'N':	case 'O':	case 'P':   case 'Q':	case 'R':	case 'S':
        case 'T':	case 'U':   case 'X':	case 'Y':	case 'Z':
            *width = 4;
            *height = 7;
            break;
        case 'M':	case 'V':	case 'W':
            *width = 5;
            *height = 7;
            break;
            // Numbers + Style : ------------------------------------------->
        case '0':	case '1':	case '2':	case '3':	case '4':
        case '5':	case '6':	case '7':	case '8':	case '9':
            switch(formatDisplay.txtFont)
            {
                case numberClassic:
                    *width = 4;
                    *height = 7;
                    break;
                case numberTime:
                    *width = 5;
                    *height = 7;
                    break;
                case numberMob:
                    *width = 4;
                    *height = 6;
                    break;
                default :	/* State of every undefined case, so not supposed to come here. */	break;
            }
            break;
            // Special Char. : --------------------------------------------->
        case ' ':	case '.':   case ',':   case '!':
        case ':':	case '=':   case '*':   case '-':
        case '+':	case '%':   /*case '°':*/   case '/':
            *width = 3;
            *height = 7;
            break;

        case '\'':
            *width = 2;
            *height = 7;
            break;

        case '(':	case ')':   case '{':   case '}':
        case '[':	case ']':   case '~':   case '@':
        case '#':	case '&':
            *width = 5;
            *height = 7;
            break;

        case '_':
            *width = 4;
            *height = 7;
            break;

        case '^':
            *width = 7;
            *height = 6;
            break;

        case '?':
            switch(formatDisplay.txtFont)
            {
                case numberClassic:
                    *width = 4;
                    *height = 7;
                    break;
                case numberMob:
                    *width = 5;
                    *height = 7;
                    break;
                default :	/* State of every undefined case, so not supposed to come here. */	break;
            }
            break;

        /* State of every undefined case, so not supposed to come here. */
        default :   break;
    }
}
#endif