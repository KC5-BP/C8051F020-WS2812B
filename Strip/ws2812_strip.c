/*-->	Title	:	ws2812_strip
		  |
		  '->	Source	:	Timers_Wait_Xms.h (One of my own first header file)
		  |
		  '->	Creation	:	21.11.2019
		  :			|
		  :			'->	Last Update	:	December 2020
		  |
		  '->	Description	:	Declarations of NeoPixels Usage functions.
*/
/*======================================================================================>
=======================================================================================*/
// Linker to : ..
// .. definition folder SFR (Like Port Definition "P5", "P6", etc...)
#include <c8051f020.h>
// .. C51/C166 routines that instructs the compiler to generate intrinsic code | _nop_():
#include <intrins.h>
// .. Base (SFR, sbit, define, var. type, etc...)
#include "../F11-NeoPix_StripPrButt/base_sfr.h"
// .. Timers function and Waiting Function.
//#include "../F11-NeoPix_StripPrButt/time.h"
// .. created header for NeoPixel management.
#include "ws2812_strip.h"

//-- GLOBAL VARIABLES	INIT:----------------------------->
xdata pixel strip[MAX_LEDS] = {{0, 0, 0}, 0};    // Initialize Strip to '0'.

void pixel_SetColor(pixel* addressStrip, color newColor, unsigned int position)
{   // Go to the wanted LED position.
    addressStrip += (position - 1);

    // Set "color" values :
    addressStrip->colorPix = newColor;

    // Set the status to ON if color different of "black" :
    if((newColor.Red == 0) && (newColor.Green == 0) && (newColor.Blue == 0))
    {   // Complement : " == " operator not possible in C.
        addressStrip->status = 0;
    }
    else
    {
        addressStrip->status = 1;
    }
}

void pixel_Show(unsigned char red, unsigned char green, unsigned char blue)
{   // For the WS2812b, the order is High bit to low AND Green - Red - Blue.
    // Sending GREEN.
    SEND_TO_LED(green, BIT7);
    SEND_TO_LED(green, BIT6);
    SEND_TO_LED(green, BIT5);
    SEND_TO_LED(green, BIT4);
    SEND_TO_LED(green, BIT3);
    SEND_TO_LED(green, BIT2);
    SEND_TO_LED(green, BIT1);
    SEND_TO_LED(green, BIT0);
    // Sending RED.
    SEND_TO_LED(red, BIT7);
    SEND_TO_LED(red, BIT6);
    SEND_TO_LED(red, BIT5);
    SEND_TO_LED(red, BIT4);
    SEND_TO_LED(red, BIT3);
    SEND_TO_LED(red, BIT2);
    SEND_TO_LED(red, BIT1);
    SEND_TO_LED(red, BIT0);
    // Sending BLUE.
    SEND_TO_LED(blue, BIT7);
    SEND_TO_LED(blue, BIT6);
    SEND_TO_LED(blue, BIT5);
    SEND_TO_LED(blue, BIT4);
    SEND_TO_LED(blue, BIT3);
    SEND_TO_LED(blue, BIT2);
    SEND_TO_LED(blue, BIT1);
    SEND_TO_LED(blue, BIT0);
}

//-------------------------------------------------------------------------------------->
void strip_Show(pixel* addressStrip)
{   // Var. Dec. :
    xdata uint16 i;

    // Disable Timer to avoid interrupting Sending Paquets :
    TR0 = 0;

    for(i = 0; i < MAX_LEDS; i++)
    {
        if(addressStrip->status != 0)
        {
            pixel_Show(addressStrip->colorPix.Red, addressStrip->colorPix.Green, \
                        addressStrip->colorPix.Blue);
        }
        else
        {
            pixel_Show(BRIGHT_MIN, BRIGHT_MIN, BRIGHT_MIN);
        }
        addressStrip++;
    }

    // Enable / Re-activate Timer after Packets Sent :
    TR0 = 1;
}

void strip_Clear(pixel* addressStrip)
{   // Var. Dec. :
    unsigned int i;
    pixel* addressSave = addressStrip;

    for(i = 0; i < MAX_LEDS; i++)
    {	// Clear ALL Color at the original address ...
        addressStrip->colorPix.Red = BRIGHT_MIN;
        addressStrip->colorPix.Green = BRIGHT_MIN;
        addressStrip->colorPix.Blue = BRIGHT_MIN;

        addressStrip->status = 0; // Clear status ..

        addressStrip++;  // Increase address for clearing next position.
    }

    strip_Show(addressSave);
}

void strip_StatusReset(pixel* addressStrip)
{   // Var. Dec. :
    unsigned int i;

    for(i = 0; i < MAX_LEDS; i++)
    {   // Clear Status to keep color in the original address ...
        addressStrip->status = 0;

        addressStrip++;  // Increase address for clearing next position.
    }
}

void strip_StatusInverter(pixel* addressStrip)
{	// Var. Dec. :
    /*xdata uint16 i;	// LED Position for filling Board of neoPix.
    //xdata uint16 j;	// Address recovering counter of the matrix.
    // To avoid problems about the parameters, I prefered to work with copies.
    //pixel* AddSave = addressStrip;
    xdata color tmp = {0, 0, 0};

    for(i = 0; i < MAX_LEDS; i++)
    {
        //if((addressStrip->colorPix.Red != 0) || (addressStrip->colorPix.Green != 0) ||\
             (addressStrip->colorPix.Blue != 0))
        if(addressStrip->status != 0)
        {
            tmp.Red = addressStrip->colorPix.Red;
            tmp.Green = addressStrip->colorPix.Green;
            tmp.Blue = addressStrip->colorPix.Blue;
            break;
        }
        addressStrip++;
    }

// 	for(i = 0; i < j; i++)
// 	{
// 		addressStrip--;
// 	}

    for(i = 0; i < MAX_LEDS; i++)
    {
        if(addressStrip[i].status == 0)
        {
            addressStrip[i].status = 1;
            addressStrip[i].colorPix.Red = tmp.Red;
            addressStrip[i].colorPix.Green = tmp.Green;
            addressStrip[i].colorPix.Blue = tmp.Blue;
        }
        else
        {
            addressStrip[i].status = 0;
            addressStrip[i].colorPix.Red = BRIGHT_MIN;
            addressStrip[i].colorPix.Green = BRIGHT_MIN;
            addressStrip[i].colorPix.Blue = BRIGHT_MIN;
        }
        //addressStrip++;
    }*/
}

void strip_LimitPos(pixel* addressStrip, color newColor, \
											    unsigned int _Begin, unsigned int _End)
{	// Var. Dec. :
	uint16 i;	// LED Position for filling Board of neoPix.
	// To avoid problems about the parameters, I prefered to work with copies.
	pixel* AddSave = addressStrip;
	
	for(i = 0; i < MAX_LEDS; i++)
	{
		if((i >= _Begin) && (i <= _End))
		{
			addressStrip->colorPix.Red = newColor.Red;
			addressStrip->colorPix.Green = newColor.Green;
			addressStrip->colorPix.Blue = newColor.Blue;
		}
		else
		{
			addressStrip->colorPix.Red = BRIGHT_MIN;
			addressStrip->colorPix.Green = BRIGHT_MIN;
			addressStrip->colorPix.Blue = BRIGHT_MIN;
		}
		addressStrip++;
	}
	strip_Show(AddSave);
}

