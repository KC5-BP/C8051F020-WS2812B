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
posType pixel_RecoverPosition(unsigned char posX, unsigned char posY)
{	// Var. Dec. :
    posType ui16_returnPos = 0;

    // Condition about exceeding/equaling the MAX_COLU or MAX_LINE.
    /*if (MAX_COLU < posX)  posX = MAX_COLU - 1;
    if (MAX_LINE < posY)  posY = MAX_LINE - 1;*/

    switch(formatDisplay.matrixView)
    {
        case 0:         case 360:
        case (-360):    default :
            if( !(posY % 2) )   // Even lines on the matrix physical view.
                ui16_returnPos = posX / MAX_COLU * MAX_LEDS + posX % MAX_COLU + posY * MAX_COLU;
            else
                ui16_returnPos = posX / MAX_COLU * MAX_LEDS + (posY + 1) * MAX_COLU - posX % MAX_COLU - 1;
            break;

        case 90:
        case (-270):
            if( !(posX % 2) )   // Even columns on the matrix physical view.
                ui16_returnPos = MAX_LEDS - posX * MAX_LINE - posY - 1;
            else
                ui16_returnPos = MAX_LEDS - (posX + 1) * MAX_LINE + posY;
            break;

        case 180:
        case (-180):
            if((posY % 2) == 0) // Even reverted lines on the matrix physical view.
                ui16_returnPos = MAX_LEDS - (posY + 1) * MAX_COLU + posX;
            else
                ui16_returnPos = MAX_LEDS - posY * MAX_COLU - posX - 1;
            break;

        case 270:
        case (-90):
            if( !(posX % 2) )   // Even reverted columns on the matrix physical view.
                ui16_returnPos = (posX + 1) * MAX_LINE - posY - 1;
            else
                ui16_returnPos = (posX * MAX_LINE) + posY;
            break;
    }
    return ui16_returnPos; // Return Pos.
}

//-------------------------------------------------------------------------------------->
#ifdef __kc5_bp_matrix_alphanum__
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

void matrix_Print(pixel* addressMatrix, color newColor, const char* _CharToWrite) {
    // Var. Dec. :
	xdata unsigned int j, i;	// X, Y and Filling
    xdata unsigned char Cnt_Col, Cnt_Lin;  // For Char with different : Column size.
	                                        // For Char with different : Line size.
	const char* cP_CharToPrint; // Recover the actual char to print.
	xdata posType NeoPix_Pos = 0;

	for(cP_CharToPrint = _CharToWrite; *cP_CharToPrint != '\0'; cP_CharToPrint++) {
        retrieveHeightWidthOf(cP_CharToPrint, &Cnt_Col, &Cnt_Lin);

		for(i = 0; i < Cnt_Lin; i++) {
			for(j = 0; j < Cnt_Col; j++) {
				NeoPix_Pos = pixel_RecoverPosition((j+Offset.column), i+Offset.line);
				// Condition about possible value for displaying or not.
				if(NeoPix_Pos >= ((MAX_COLU * MAX_LINE) * NBR_OF_MATRIX))
					continue;   // Restart loop with following value of j.

				switch(*cP_CharToPrint)
				{
                    case ' ':
                        addressMatrix[NeoPix_Pos].status = cBoard_Space[j + i * Cnt_Col];
                        break;
                    case '!':
                        addressMatrix[NeoPix_Pos].status = cBoard_ExclaMark[j + i * Cnt_Col];
                        break;
                    //case '\"':
                    case '#':
                        addressMatrix[NeoPix_Pos].status = cBoard_HashtagV1[j + i * Cnt_Col];
                        //addressMatrix[NeoPix_Pos].status = cBoard_HashtagV2[j + i * Cnt_Col];
                        break;
                    //case '$':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '%':
                        addressMatrix[NeoPix_Pos].status = cBoard_xMult[j + i * Cnt_Col];
                        break;
                    case '&':
                        addressMatrix[NeoPix_Pos].status = cBoard_Ampersand[j + i * Cnt_Col];
                        break;
                    case '\'':
                        addressMatrix[NeoPix_Pos].status = cBoard_Apost[j + i * Cnt_Col];
                        break;
                    case '(':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenBracket[j + i * Cnt_Col];
                        break;
                    case ')':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseBracket[j + i * Cnt_Col];
                        break;
                    case '*':
                        addressMatrix[NeoPix_Pos].status = cBoard_Asterisk[j + i * Cnt_Col];
                        break;
                    case '+':
                        addressMatrix[NeoPix_Pos].status = cBoard_Plus[j + i * Cnt_Col];
                        break;
                    case ',':
                        addressMatrix[NeoPix_Pos].status = cBoard_Comma[j + i * Cnt_Col];
                        break;
                    case '-':
                        addressMatrix[NeoPix_Pos].status = cBoard_Hyphen[j + i * Cnt_Col];
                        break;
                    case '.':
                        addressMatrix[NeoPix_Pos].status = cBoard_Dot[j + i * Cnt_Col];
                        break;
                    case '/':
                        addressMatrix[NeoPix_Pos].status = cBoard_Slash[j + i * Cnt_Col];
                        break;

                    case '0':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_0_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '1':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_1_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '2':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_2_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '3':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_3_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '4':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_4_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '5':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_5_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '6':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_6_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '7':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_7_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '8':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_8_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '9':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Val[j + i * Cnt_Col];
                                break;
                            case numberTime:
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Time[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_9_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;

                    case ':':
                        addressMatrix[NeoPix_Pos].status = cBoard_Colon[j + i * Cnt_Col];
                        break;
                    case ';':
                        addressMatrix[NeoPix_Pos].status = cBoard_SemiColon[j + i * Cnt_Col];
                        break;
                    //case '<':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '=':
                        addressMatrix[NeoPix_Pos].status = cBoard_Equal[j + i * Cnt_Col];
                        break;
                    //case '>':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '?':
                        switch(formatDisplay.txtFont)
                        {
                            case numberClassic:
                                addressMatrix[NeoPix_Pos].status = cBoard_QuestMark[j + i * Cnt_Col];
                                break;
                            case numberMob:
                            default :
                                addressMatrix[NeoPix_Pos].status = cBoard_QuestMark_Mob[j + i * Cnt_Col];
                                break;
                        }
                        break;
                    case '@':
                        addressMatrix[NeoPix_Pos].status = cBoard_At[j + i * Cnt_Col];
                        break;

                    case 'A':
						addressMatrix[NeoPix_Pos].status = cBoard_A[j + i * Cnt_Col];
						break;
					case 'B':
						addressMatrix[NeoPix_Pos].status = cBoard_B[j + i * Cnt_Col];
						break;
					case 'C':
						addressMatrix[NeoPix_Pos].status = cBoard_C[j + i * Cnt_Col];
						break;
					case 'D':
						addressMatrix[NeoPix_Pos].status = cBoard_D[j + i * Cnt_Col];
						break;
					case 'E':
						addressMatrix[NeoPix_Pos].status = cBoard_E[j + i * Cnt_Col];
						break;
					case 'F':
						addressMatrix[NeoPix_Pos].status = cBoard_F[j + i * Cnt_Col];
						break;
					case 'G':
						addressMatrix[NeoPix_Pos].status = cBoard_G[j + i * Cnt_Col];
						break;
					case 'H':
						addressMatrix[NeoPix_Pos].status = cBoard_H[j + i * Cnt_Col];
						break;
					case 'I':
						addressMatrix[NeoPix_Pos].status = cBoard_I[j + i * Cnt_Col];
						break;
					case 'J':
						addressMatrix[NeoPix_Pos].status = cBoard_J[j + i * Cnt_Col];
						break;
					case 'K':
						addressMatrix[NeoPix_Pos].status = cBoard_K[j + i * Cnt_Col];
						break;
					case 'L':
						addressMatrix[NeoPix_Pos].status = cBoard_L[j + i * Cnt_Col];
						break;
					case 'M':
						addressMatrix[NeoPix_Pos].status = cBoard_M[j + i * Cnt_Col];
						break;
					case 'N':
						addressMatrix[NeoPix_Pos].status = cBoard_N[j + i * Cnt_Col];
						break;
					case 'O':
						addressMatrix[NeoPix_Pos].status = cBoard_O[j + i * Cnt_Col];
						break;
					case 'P': //
						addressMatrix[NeoPix_Pos].status = cBoard_P[j + i * Cnt_Col];
						break;
					case 'Q':
						addressMatrix[NeoPix_Pos].status = cBoard_Q[j + i * Cnt_Col];
						break;
					case 'R':
						addressMatrix[NeoPix_Pos].status = cBoard_R[j + i * Cnt_Col];
						break;
					case 'S':
						addressMatrix[NeoPix_Pos].status = cBoard_S[j + i * Cnt_Col];
						break;
					case 'T':
						addressMatrix[NeoPix_Pos].status = cBoard_T[j + i * Cnt_Col];
						break;
					case 'U':
						addressMatrix[NeoPix_Pos].status = cBoard_U[j + i * Cnt_Col];
						break;
					case 'V':
						addressMatrix[NeoPix_Pos].status = cBoard_V[j + i * Cnt_Col];
						break;
					case 'W':
						addressMatrix[NeoPix_Pos].status = cBoard_W[j + i * Cnt_Col];
						break;
					case 'X':
						addressMatrix[NeoPix_Pos].status = cBoard_X[j + i * Cnt_Col];
						break;
					case 'Y':
						addressMatrix[NeoPix_Pos].status = cBoard_Y[j + i * Cnt_Col];
						break;
					case 'Z':
						addressMatrix[NeoPix_Pos].status = cBoard_Z[j + i * Cnt_Col];
						break;

                    case '[':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenSqBracket[j + i * Cnt_Col];
                        break;
                    //case '\\':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case ']':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseSqBracket[j + i * Cnt_Col];
                        break;
					case '^':
						addressMatrix[NeoPix_Pos].status = cBoard_Heart[j + i * Cnt_Col];
						break;
                    case '_':
                        addressMatrix[NeoPix_Pos].status = cBoard_Underscore[j + i * Cnt_Col];
                        break;
                    //case '`':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '{':
                        addressMatrix[NeoPix_Pos].status = cBoard_OpenRoBracket[j + i * Cnt_Col];
                        break;
                    //case '|':
                    //  addressMatrix[NeoPix_Pos].status = ;
                    //  break;
                    case '}':
                        addressMatrix[NeoPix_Pos].status = cBoard_CloseRoBracket[j + i * Cnt_Col];
                        break;
                    case '~':
                        addressMatrix[NeoPix_Pos].status = cBoard_Tilda[j + i * Cnt_Col];
                        break;

					case '\n':
					    // Reset X
						Offset.column = 0;
                        // Decrease Line of Height + 1 line to make a space.
						Offset.line -= 8;
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
                    /*addressMatrix[NeoPix_Pos].colorPix = \
                                                (color){BRIGHT_MIN,BRIGHT_MIN,BRIGHT_MIN};*/
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

	//leds_Show(addressMatrix);
}

void retrieveHeightWidthOf(const char* character, \
                                        unsigned char* width, unsigned char* height) {
    // Retrieve the Height & Width of a letter through addresses :
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
            switch(formatDisplay.txtFont)
            {
                case numberClassic:
                default:
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