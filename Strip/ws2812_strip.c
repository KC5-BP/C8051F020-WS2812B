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
#include "../F11-NeoPix_StripPrButt/base_sfr.h"	// ... Base (SFR, sbit, define, var. type, etc...)
#include "../F11-NeoPix_StripPrButt/time.h"		// ... Timers function and Waiting Function.
#include "ws2812_strip.h"	// ... Timers function and Waiting Function.

//-- GLOBAL VARIABLES	INIT:------------------------------->
xdata pixel strip[MAX_LEDS] = {{0, 0, 0}, 0, 0};   // Initialize it to '0'.
// & the Position is a Var. that can be unused.

void pixel_SetColor(pixel* addressStrip, color newColor, unsigned int position)
{	// Var. Dec. :
   // To avoid problems about the parameters, I preferred to work with copies.
   unsigned char Red = newColor.Red;
   unsigned char Green = newColor.Green;
   unsigned char Blue = newColor.Blue;

   addressStrip += (position - 1);	// go to the wanted LED position.

   // Set "color" values :
   addressStrip->colorPix.Red = Red;
   addressStrip->colorPix.Green = Green;
   addressStrip->colorPix.Blue = Blue;

   // Set the position info. in the structure :
   addressStrip->pos = position;

   // Set the status to ON :
   addressStrip->status = 1;
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


void strip_Clear(pixel* addressStrip)
{	// Var. Dec. :
   unsigned int i;
   pixel* addressSave = addressStrip;

   for(i = 0; i < MAX_LEDS; i++)
   {	// Clear ALL Color in the original addresses ...
      addressStrip->colorPix.Red = BRIGHT_MIN;
      addressStrip->colorPix.Green = BRIGHT_MIN;
      addressStrip->colorPix.Blue = BRIGHT_MIN;

      addressStrip->pos = 0; // Clear position ..

      addressStrip->status = 0; // Clear status ..

      addressStrip++;  // Increase address for clearing next position.
   }
   strip_Show(addressSave);
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

void strip_Show(pixel* addressStrip)
{	// Var. Dec. :
   xdata uint16 i;

   // Disable Timer to avoid interrupting Sending Paquets :
   TR0 = 0;

   for(i = 0; i < MAX_LEDS; i++)
   {
      pixel_Show(addressStrip->colorPix.Red, addressStrip->colorPix.Green, addressStrip->colorPix.Blue);
      addressStrip++;
   }

   // Enable / Re-activate Timer after Paquets Sent :
   TR0 = 1;
}


void Matrix_LimitPos(pixel* _addMatrix, color newColor, unsigned int _Begin, unsigned int _End)
{	// Var. Dec. :
	uint16 i;	// LED Position for filling Board of neoPix.
	// To avoid problems about the parameters, I prefered to work with copies.
	pixel* AddSave = _addMatrix;
	
	for(i = 0; i < MAX_LEDS; i++)
	{
		if((i >= _Begin) && (i <= _End))
		{
			_addMatrix->colorPix.Red = newColor.Red;
			_addMatrix->colorPix.Green = newColor.Green;
			_addMatrix->colorPix.Blue = newColor.Blue;
		}
		else
		{
			_addMatrix->colorPix.Red = BRIGHT_MIN;
			_addMatrix->colorPix.Green = BRIGHT_MIN;
			_addMatrix->colorPix.Blue = BRIGHT_MIN;
		}
		_addMatrix++;
	}
	strip_Show(AddSave);
}

void Matrix_InvertArray(pixel* _addMatrix)
{	// Var. Dec. :
	xdata uint16 i;	// LED Position for filling Board of neoPix.
	xdata uint16 j;	// Address recovering counter of the matrix.
	// To avoid problems about the parameters, I prefered to work with copies.
	pixel* AddSave = _addMatrix;
	xdata color tmp = {0, 0, 0};

	for(i = 0; i < MAX_LEDS; i++)
	{
		if((_addMatrix->colorPix.Red != 0) || (_addMatrix->colorPix.Green != 0) || (_addMatrix->colorPix.Blue != 0))
		{
			tmp.Red = _addMatrix->colorPix.Red;
			tmp.Green = _addMatrix->colorPix.Green;
			tmp.Blue = _addMatrix->colorPix.Blue;
			break;
		}
		_addMatrix++;
	}

	for(i = 0; i < j; i++)
	{
		_addMatrix--;
	}
	
	for(i = 0; i < MAX_LEDS; i++)
	{
		if(_addMatrix->status == 0)
		{
			_addMatrix->status = 1;
			_addMatrix->colorPix.Red = tmp.Red;
			_addMatrix->colorPix.Green = tmp.Green;
			_addMatrix->colorPix.Blue = tmp.Blue;
		}
		else
		{
			_addMatrix->status = 0;
			_addMatrix->colorPix.Red = BRIGHT_MIN;
			_addMatrix->colorPix.Green = BRIGHT_MIN;
			_addMatrix->colorPix.Blue = BRIGHT_MIN;
		}
		_addMatrix++;
	}
}