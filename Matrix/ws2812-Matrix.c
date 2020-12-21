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
/*========================================================================================>
=========================================================================================*/
// Linker to : ...
#include <c8051f020.h>		// ... definition folder SFR 
													// (Like Port Definition "P5", "P6", etc...)
// ... C51/C166 routines that instructs the compiler to generate intrinsic code | _nop_():
#include <intrins.h>
#include "Base_SFR.h"	// ... Base(define, sbit, sfr,...)
#include "Time.h"		// ... Timers function and Waiting Function.
#include "NeoPixel.h"	// ... Timers function and Waiting Function.
//#include "KBP_Matrix_Letters.h"	// ... Base (SFR, sbit, define, var. type, etc...)

//-- GLOBAL VARIABLES	INIT:------------------------------->
xdata tstr_NeoPixel Strip[MAX_LED] = {{0, 0, 0}, 0, 0};  // Initialize it to '0'.
																				// & the Position is a Var. that can be not used.
//xdata NeoPixel Matrix[MAX_LED] = {{0, 0, 0}, 0};  // Initialize it to '0'.
																				// & the Position is a Var. that can be not used.
//NeoPixel* Strip[MAX_LED];
//int16 MatFormat.i16_MatOri = (0);
xdata tstr_Offset Offset = {0, 0};
xdata tstr_MatrixFormat MatFormat = {0, 0};


uint16 NeoPix_PosRecov(int8 _i8_PosX, int8 _i8_PosY)
// Description	:	Recovered the position of a LED with the Line and Column.
//								(Input Value from 1, instead of 0).
// Date 		:	03.11.2019
// Input		:	_iLin,	1byte
//						_iCol,	1byte
// Output	 	:	The position value.
// Representation - Initial : 0/360 :
// 					3 :	 / - - - - - - - -  -  -  -  -  -  -  /  
// 					2 :  \ - - - - - - - -  -  -  -  -  -  -  \.
// 					1 :  - - - - - - - - -  -  -  -  -  -  -  /  
//Starting LED -´1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
//
// Representation - Rotated : 270/-90 :
// | | | | | | | | |  |  |  |  |  |  |  | : 3
// | | | | | | | | |  |  |  |  |  |  |  | : 2
// | \ / \ / \ / \ /  \  /  \  /  \  /  | : 1  
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16`- Starting LED.
{	// Var. Dec. : 
	uint8 ui8_NewX = 0, ui8_NewY = 0;	
	uint8 ui8_X = 0, ui8_Y = 0;
	uint16 ui16_MatOffset = 0;
	uint16 ui16_ReturnPos = 0;
	// Condition about starting from 1, instead of 0.
	if(_i8_PosX == 0) { ui8_X = 1; } else { ui8_X = _i8_PosX;}
	if(_i8_PosY == 0) { ui8_Y = 1; } else { ui8_Y = _i8_PosY;}
	switch(MatFormat.i16_MatOri)
	{
		case 0:
		case 360:
		case (-360):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((_i8_PosY % 2) == 0)
			{
				if(ui8_X <= MAX_COLUMN)
				{
					ui8_NewX = MAX_COLUMN - ui8_X;
					ui16_MatOffset = 0;
				}
				else
				{
					if(ui8_X <= (2 * MAX_COLUMN))
					{
						ui8_NewX = (2 * MAX_COLUMN) - ui8_X;
						ui16_MatOffset = MAX_COLUMN * MAX_LINE;
					}
					else
					{
						ui8_NewX = (3 * MAX_COLUMN) - ui8_X;
						ui16_MatOffset = 2 * MAX_COLUMN * MAX_LINE;
					}
				}
			}
			else
			{
				if(ui8_X <= MAX_COLUMN)
				{
					ui8_NewX = (ui8_X - 1);
					ui16_MatOffset = 0;
				}
				else
				{
					if(ui8_X <= (2 * MAX_COLUMN))
					{
						ui8_NewX = (ui8_X - MAX_COLUMN - 1);
						ui16_MatOffset = MAX_COLUMN * MAX_LINE;
					}
					else
					{
						ui8_NewX = (ui8_X - (2 * MAX_COLUMN) - 1);
						ui16_MatOffset = 2 * MAX_COLUMN * MAX_LINE;
					}
				}
			}
			ui16_ReturnPos = (((ui8_Y - 1) * MAX_COLUMN) + ui8_NewX) + ui16_MatOffset;
			break;
			
			
		case 90:
		case (-270):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((_i8_PosX % 2) == 0)
			{
				ui8_NewY = (MAX_LINE - 1) - _i8_PosY;
			}
			else
			{
				ui8_NewY = _i8_PosY;
			}
			ui16_ReturnPos = (((_i8_PosX-1) * MAX_COLUMN) + ui8_NewY);
			break;
		
		case 180:
		case (-180):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((_i8_PosY % 2) == 0)
			{
				ui8_NewX = MAX_COLUMN - ui8_X;
			}
			else
			{
				ui8_NewX = (ui8_X - 1);
			}
			ui16_ReturnPos = (((ui8_Y - 1) * MAX_COLUMN) + ui8_NewX);
			break;
			
			
		case 270:
		case (-90):
			// Condition about the Line being Even or not.
			// Then, by the Matrix routing, define how the WS281x must be adressed.
			if((_i8_PosX % 2) == 0)
			{
				ui8_NewY = (MAX_LINE - 1) - _i8_PosY;
			}
			else
			{
				ui8_NewY = _i8_PosY;
			}
			ui16_ReturnPos = (((_i8_PosX - 1) * MAX_COLUMN) + ui8_NewY); // Checker les PosRecov en rotation.
			break;
			
		default :	/* State of every undefined case, so not supposed to come here. */	break;
	}
	return ui16_ReturnPos; // Return Pos.
}

void NeoPix_SetColor(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, uint16 _ui16_Pos)
// Description	:	Recovered the position of a LED with the Line and Column.
// Date 		:	03.11.2019
// Input		:	_iLin,	1byte
//						_iCol,	1byte
// Output	 	:	The position value.
{	// Var. Dec. :
				// To avoid problems about the parameters, I prefered to work with copies.
	uint8 Red = _newCol.ui8_Red;
	uint8 Green = _newCol.ui8_Green;
	uint8 Blue = _newCol.ui8_Blue;
	
	_addMatrix += (_ui16_Pos - 1);	// Set values to the selected Position.
	// Set Colors :
	_addMatrix->Col_Pix.ui8_Red = Red;
	_addMatrix->Col_Pix.ui8_Green = Green;
	_addMatrix->Col_Pix.ui8_Blue = Blue;
	// Set the Position value in the Struct. :
	_addMatrix->ui16_Pos = _ui16_Pos;
}

void NeoPix_Show(uint8 _Red, uint8 _Green, uint8 _Blue)
// Description :	Send the initiated Board on the Matrix.
// Date 			:	07.06.2018
// Input			:	_Red,	1byte.
//						_Green,		1byte.
//						_Blue,	1byte.
// Output	 	:	Nothing
{
//-Local Variables--------------------------------->
	uint8  byV_Red = _Red;
	uint8  byV_Green = _Green;
	uint8  byV_Blue = _Blue;

//-Code Function----------------------------------->	
	// For the WS2812b, the order is High bit to low AND Green - Red - Blue.
	// Sending GREEN.
	SEND_TO_LED(byV_Green, BIT7);
	SEND_TO_LED(byV_Green, BIT6);
	SEND_TO_LED(byV_Green, BIT5);
	SEND_TO_LED(byV_Green, BIT4);
	SEND_TO_LED(byV_Green, BIT3);
	SEND_TO_LED(byV_Green, BIT2);
	SEND_TO_LED(byV_Green, BIT1);
	SEND_TO_LED(byV_Green, BIT0);
	// Sending RED.
	SEND_TO_LED(byV_Red, BIT7);
	SEND_TO_LED(byV_Red, BIT6);
	SEND_TO_LED(byV_Red, BIT5);
	SEND_TO_LED(byV_Red, BIT4);
	SEND_TO_LED(byV_Red, BIT3);
	SEND_TO_LED(byV_Red, BIT2);
	SEND_TO_LED(byV_Red, BIT1);
	SEND_TO_LED(byV_Red, BIT0);
	// Sending BLUE.
	SEND_TO_LED(byV_Blue, BIT7);
	SEND_TO_LED(byV_Blue, BIT6);
	SEND_TO_LED(byV_Blue, BIT5);
	SEND_TO_LED(byV_Blue, BIT4);
	SEND_TO_LED(byV_Blue, BIT3);
	SEND_TO_LED(byV_Blue, BIT2);
	SEND_TO_LED(byV_Blue, BIT1);
	SEND_TO_LED(byV_Blue, BIT0);
}

void Matrix_Clear(tstr_NeoPixel* _addMatrix)
// Description :	Shut the matrix down.
// Date 			:	01.10.2019
// Input			:	Nothing.
// Output	 	:	Nothing, but sending data to MATRIX.
{	// Var. Dec. :
	uint16 i;
	tstr_NeoPixel* AddSave = _addMatrix;
	
	for(i = 0; i < MAX_LED; i++)
	{	// Clear ALL Color in the original addresses ...
		_addMatrix->Col_Pix.ui8_Red = BRIGHT_MIN;
		_addMatrix->Col_Pix.ui8_Green = BRIGHT_MIN;
		_addMatrix->Col_Pix.ui8_Blue = BRIGHT_MIN;
		_addMatrix++;
	}
	Matrix_Show(AddSave);
}

void Matrix_ResetStatus(tstr_NeoPixel* _addMatrix)
{	// Var. Dec. :
	uint16 i;
	
	for(i = 0; i < MAX_LED; i++)
	{	// Clear ALL Color & Status in the original addresses ...
		_addMatrix->ui8_Status = 0;
		_addMatrix->Col_Pix.ui8_Red = BRIGHT_MIN;
		_addMatrix->Col_Pix.ui8_Green = BRIGHT_MIN;
		_addMatrix->Col_Pix.ui8_Blue = BRIGHT_MIN;
		_addMatrix++;
	}
}

void Matrix_SetPos(uint8 _ui8_Col, uint8 _ui8_Lin)
// Description	:	Recovered the position of a LED with the Line and Column.
// Date 		:	03.11.2019
// Input		:	_iLin,	1byte
//						_iCol,	1byte
// Output	 	:	The position value.
{
	Offset.ui8_Colu = _ui8_Col;
	Offset.ui8_Line = _ui8_Lin;
}

void Matrix_Show(tstr_NeoPixel* _addMatrix)
// Description :	Send the initiated Board on the Matrix.
// Date 			:	22.03.2020
// Input			:	_StructNeoPix*,	Address of a 4 * 1 byte structure.
// Output	 	:	Nothing
{	// Var. Dec. :
	xdata uint16 i;

	// Disable Timer to avoid interrupting Sending Paquets :
	TR0 = 0;
	
	for(i = 0; i < MAX_LED; i++)
	{
		NeoPix_Show(_addMatrix->Col_Pix.ui8_Red, _addMatrix->Col_Pix.ui8_Green, _addMatrix->Col_Pix.ui8_Blue);
		_addMatrix++;
	}
	
	// Enable / Re-activate Timer after Paquets Sent :
	TR0 = 1;
}

// void Matrix_Print(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, const char* _CharToWrite, ...)
// // Description	:	Recovered the position of a LED with the Line and Column.
// // Date 		:	03.11.2019
// // Input		:	_iLin,	1byte
// //						_iCol,	1byte
// // Output	 	:	The position value.
// {	// Var. Dec. :
// 	xdata uint16 i;	// Y and Filling
// 	xdata uint8 j;	// X
// 	xdata uint8 Cnt_Lin = 0;	// For Char with different : Line
// 	xdata uint8 Cnt_Col = 0;	// For Char with different : Colu
// 	const char* cP_CharToPrint;	// Recover the actual char to print.
// 	xdata uint16 NeoPix_Pos = 0;	// Position recovery from an int to 
// 	xdata uint16 CharReadOrga = 0;	// Organisation about Reading the Char depending on the Matrix' Orientation.
// 	
// 	
// 	//xdata uint8 Test[28] = 0;	// After printing a char, swift for the next one.
// 	
// 	for(cP_CharToPrint = _CharToWrite; *cP_CharToPrint != '\0'; cP_CharToPrint++)
// 	{	
// 		switch(*cP_CharToPrint)
// 		{	// Height & Width can be vary so a first Switch..Case is necessary.
// 			// Letters : --------------------------------------------------->
// 			case 'A':	case 'B':	case 'C':	case 'D':	case 'E': case 'F':	
// 			case 'G':	case 'H':	case 'I':	case 'J': case 'K':	case 'L':	
// 			case 'N':	case 'O':	case 'P': case 'Q':	case 'R':	case 'S':	
// 			case 'T':	case 'U': case 'X':	case 'Y':	case 'Z':
// 				Cnt_Col = 4;
// 				Cnt_Lin = 7;
// 				break;			
// 			case 'M':	case 'V':	case 'W':
// 				Cnt_Col = 5;
// 				Cnt_Lin = 7;
// 				break;
// 			// Numbers + Style : ------------------------------------------->
// 			case '0':	case '1':	case '2':	case '3':	case '4':
// 			case '5':	case '6':	case '7':	case '8':	case '9':
// 				switch(MatFormat.i8_TxtFont)
// 				{
// 					case enu_Classic:
// 						Cnt_Col = 4;
// 						Cnt_Lin = 7;
// 						break;
// 					case enu_Time:
// 						Cnt_Col = 5;
// 						Cnt_Lin = 7;
// 						break;
// 					case enu_Mob:
// 						Cnt_Col = 4;
// 						Cnt_Lin = 6;
// 						break;
// 					default :	/* State of every undefined case, so not supposed to come here. */	break;
// 				}
// 				break;
// 			// Special Char. : --------------------------------------------->
// 			case '.':	case ',':	/*case '!':*/	/*case '?':*/
// 				Cnt_Col = 3;
// 				Cnt_Lin = 7;
// 				break;
// 			
// 			case '^':
// 				Cnt_Col = 7;
// 				Cnt_Lin = 6;
// 				break;
// 			
// 			case '\'':
// 				Cnt_Col = 2;
// 				Cnt_Lin = 7;
// 				break;
// 			
// 			case '!': case '*': case ' ': case ':':
// 				Cnt_Col = 3;
// 				Cnt_Lin = 7;
// 				break;
// 			
// 			case '?':
// 				switch(MatFormat.i8_TxtFont)
// 				{
// 					case enu_Classic:
// 						Cnt_Col = 4;
// 						Cnt_Lin = 7;
// 						break;
// 					case enu_Mob:
// 						Cnt_Col = 5;
// 						Cnt_Lin = 7;
// 						break;
// 					default :	/* State of every undefined case, so not supposed to come here. */	break;
// 				}
// 				break;
// 				
// 			default :	/* State of every undefined case, so not supposed to come here. */	break;
// 		}
// 		
// 		for(i = 0; i < Cnt_Lin; i++)
// 		{	
// 			for(j = 0; j < Cnt_Col; j++)
// 			{	// Same Line :
// 				NeoPix_Pos = NeoPix_PosRecov((j+Offset.ui8_Colu), i+Offset.ui8_Line);
// 				//Test[(i*4) + j] = NeoPix_Pos;
// 				// Condition about possible value for displaying or not.
// 				if(NeoPix_Pos >= ((MAX_COLUMN * MAX_LINE) * MATRIX_NBR))
// 				{
// 					break;
// 				}
// 				else { /* Nothing. */ }		
// 				// Switch about definning how the char Board must be read.
// 				switch(MatFormat.i16_MatOri)
// 				{
// 					case 0:
// 					case 180:
// 					case 360:
// 					case (-180):
// 					case (-360):
// 						CharReadOrga = ((i * Cnt_Col) - j);
// 						break;
// 					case 90:
// 					case 270:
// 					case (-90):
// 					case (-270):
// 						CharReadOrga = (((i + 1) * Cnt_Col) - (j + 1));
// 						break;
// 					default :	/* State of every undefined case, so not supposed to come here. */	break;
// 				}
// 				switch(*cP_CharToPrint)
// 				{
// 					case 'A':
// //_addMatrix[NeoPix_PosRecov((j+Offset.ui8_Colu), i+Offset.ui8_Line)].ui8_Status = cBoard_A[OFF_CHAR_DIM_4X7 - (i * Cnt_Col) + j];
// 					// Careful with the Sign !!! 
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_A[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'B':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_B[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'C':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_C[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'D':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_D[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'E':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_E[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'F':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_F[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'G':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_G[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'H':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_H[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'I':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_I[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'J':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_J[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'K':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_K[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'L':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_L[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'M':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_M[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 						break;
// 					case 'N':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_N[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'O':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_O[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'P': //
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_P[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'Q':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Q[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'R':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_R[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'S':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_S[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'T':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_T[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'U':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_U[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'V':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_V[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 						break;
// 					case 'W':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_W[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 						break;
// 					case 'X':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_X[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'Y':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Y[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case 'Z':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Z[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					
// 					case '0':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_0_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_0_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_0_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '1':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_1_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_1_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_1_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '2':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_2_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_2_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_2_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '3':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_3_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_3_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_3_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '4':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_4_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_4_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_4_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '5':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_5_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_5_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_5_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '6':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_6_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_6_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_6_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '7':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_7_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_7_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_7_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '8':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_8_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_8_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_8_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case '9':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_9_Val[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Time:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_9_Time[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_9_Mob[OFF_CHAR_DIM_4X6 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 						
// 					case '.':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Dot[OFF_CHAR_DIM_3X7 - CharReadOrga];
// 						break;
// 					case ',':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Comma[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 						break;
// 					case '!':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_ExclaMark[OFF_CHAR_DIM_3X7 - CharReadOrga];
// 						break;
// 					case '?':
// 						switch(MatFormat.i8_TxtFont)
// 						{
// 							case enu_Classic:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_QuestMark[OFF_CHAR_DIM_4X7 - CharReadOrga];
// 								break;
// 							case enu_Mob:
// 								_addMatrix[NeoPix_Pos].ui8_Status = cBoard_QuestMark_Mob[OFF_CHAR_DIM_5X7 - CharReadOrga];
// 								break;
// 							default :	/* State of every undefined case, so not supposed to come here. */	break;
// 						}
// 						break;
// 					case ':':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Colon[OFF_CHAR_DIM_3X7 - CharReadOrga];
// 						break;

// 					case '^':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Heart[OFF_CHAR_DIM_7X6 - CharReadOrga];
// 						break;
// 					
// 					case '\'':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Apost[OFF_CHAR_DIM_2X7 - CharReadOrga];
// 						break;
// 					
// 					case '*':
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Asterisk[OFF_CHAR_DIM_3X7 - CharReadOrga];
// 						break;
// 					
// 					case ' ': 
// 						_addMatrix[NeoPix_Pos].ui8_Status = cBoard_Space[OFF_CHAR_DIM_3X7 - CharReadOrga];
// 						break;
// 					
// 					case '\n':
// 						Offset.ui8_Colu = 1;
// 						Offset.ui8_Line -= 8;	// Decrease Line of Height + 1 line to make a space.
// 						//Offset.ui8_Line = 3;
// 						break;
// 					
// 					default :	/* State of every undefined case, so not supposed to come here. */	break;
// 				}
// 				
// 				// Coloring only One char at a Time :
// 				if(_addMatrix[NeoPix_Pos].ui8_Status != 0)
// 				{
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Red = _newCol.ui8_Red;
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Green = _newCol.ui8_Green;
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Blue = _newCol.ui8_Blue;
// 				}
// 				else
// 				{
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Red = BRIGHT_MIN;
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Green = BRIGHT_MIN;
// 					_addMatrix[NeoPix_Pos].Col_Pix.ui8_Blue = BRIGHT_MIN;
// 				}
// 			}
// 		}
// 		if(*cP_CharToPrint != '\n')
// 		{
// 			// Position Increment on X :
// 			Offset.ui8_Colu += (Cnt_Col + 1);
// 		}
// 		else
// 		{ /* Nothin', avoid increase Col. after a \n. */ }
// 	}
//}

// void NeoPix_Fader(NeoPixel* _AddNeoPix, Color _NewCol, uint16 _ui16_Pos, uint16 _ui16_Speed)
// // Description :	Send the initiated Board on the Matrix.
// // Date 			:	07.06.2018
// // Input			:	_StructNeoPix*,	Address of a 4 * 1 byte structure.
// // Output	 	:	Nothing
// {	// Var. Dec. : 
// 	static uint16 ui16_CntSpeed = 0;
// 	// Working with float, because of the Steps, then cast it to "uint8" the Result.
// 	static float f_Red = 0, f_Green = 0, f_Blue = 0;	
// 	float f_Step_R, f_Step_G, f_Step_B;
// 	//uint8 ui8_StateColFader = NeoPix_ColorCond(ui8_Red, ui8_Green, ui8_Blue);
// 	uint16 i = 0;	// Counter for loops.
// 	static uint8 ui8_FlagFirstInit = 0;
// 	
// 	// Cannot initiate a non-static var. to the declaration :
// 	if(ui8_FlagFirstInit <= 0)
// 	{
// 		f_Red = (float)_Red;		f_Green = (float)_Green;		f_Blue = (float)_Blue;
// 		ui8_FlagFirstInit = 1;
// 	}
// 	else
// 	{ /* Nothing, just block multiple redefinitions due to static statement. */ }
// 	
// 	// Conditions about largest Color Val. :
// 	if((f_Red >= (f_Green + 1)) && (f_Red >= (f_Blue + 1)))
// 	{
// 		f_Step_G = _Green / _Red;
// 		f_Step_B = _Blue / _Red;
// 	}
// 	else if((f_Green >= (f_Red + 1)) && (f_Green >= (f_Blue + 1)))
// 	{
// 	}
// 	else if((f_Green >= (f_Red + 1)) && (f_Green >= (f_Blue + 1)))
// 	{
// 	}
// 	
// 	if(ui16_CntSpeed < _Speed)
// 	{	// In/De-crease Speed Counter based on the Calling Speed :
// 		ui16_CntSpeed++;
// 	}
// 	else
// 	{	// Reset Speed Counter :
// 		ui16_CntSpeed = 0;
// 		f_Red = 0;
// 		// Update Values of the Board : 
// 		for(i = 0; i < MAX_LED; i++)
// 		{
// 			_AddNeoPix->ui8_V_Red = Red;
// 			_AddNeoPix->ui8_V_Green = Green;
// 			_AddNeoPix->ui8_V_Blue = Blue;
// 			_AddNeoPix++;
// 		}
// 	}
// }

void Matrix_LimitPos(tstr_NeoPixel* _addMatrix, tstr_Color _newCol, uint16 _Begin, uint16 _End)
{	// Var. Dec. :
	uint16 i;	// LED Position for filling Board of neoPix.
	// To avoid problems about the parameters, I prefered to work with copies.
	tstr_NeoPixel* AddSave = _addMatrix;
	
	for(i = 0; i < MAX_LED; i++)
	{
		if((i >= _Begin) && (i <= _End))
		{
			_addMatrix->Col_Pix.ui8_Red = _newCol.ui8_Red;
			_addMatrix->Col_Pix.ui8_Green = _newCol.ui8_Green;
			_addMatrix->Col_Pix.ui8_Blue = _newCol.ui8_Blue;
		}
		else
		{
			_addMatrix->Col_Pix.ui8_Red = BRIGHT_MIN;
			_addMatrix->Col_Pix.ui8_Green = BRIGHT_MIN;
			_addMatrix->Col_Pix.ui8_Blue = BRIGHT_MIN;
		}
		_addMatrix++;
	}
	Matrix_Show(AddSave);
}