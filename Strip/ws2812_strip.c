/*,..-----------------------------------------------------.
 /´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |       > Title : ws2812_strip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \     > Src : Timers_Wait_Xms.c
|x|  \|\| | | , /    | |    >   (One of my own
|x|     '`'\|._ |   / /         first c file)
|x|         '\),/  / |      > Creation: 2019.11.21
|x|           |/.-"_/       > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,        > Description :
|x|        ',/ |   /            Declarations of RGB's
|x|        /|| |  /             ws2812b usage funct. .
|x|     |\| |/ |- |
|x| .-,-/ | /  '/-"
|x| -\|/-/\/ ^,'|
|x| _-     |  |/
|x|  .  --"  /
|x| /--__,.-/
 \`\____________________________________________________/
  \______________________________________________________*/
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
xdata pixel strip[MAX_LEDS] = {{0, 0, 0}, 0};   // Initialize Strip to '0'.

//-------------------------------------------------------------------------------------->
void pixel_SetColor(pixel* addressStrip, color newColor, unsigned int position)
{   // "position" validity ..
    if(position < MAX_LEDS)
    {   // Go to the wanted LED position.
        addressStrip += position;

        // Set "color" values :
        addressStrip->colorPix = newColor;

        // Set the status to ON if color different of "black" :
        if ( (newColor.Red == BRIGHT_MIN)
                && (newColor.Green == BRIGHT_MIN)
                    && (newColor.Blue == BRIGHT_MIN) )
        {   // Complement : " == " operator not possible on a complete struct in C.
            addressStrip->status = 0;
        }
        else
        {
            addressStrip->status = 1;
        }
    }
}

void pixel_StatusToggle(pixel* addressStrip, unsigned int position)
{   // "position" validity ..
    if(position < MAX_LEDS)
    {   // Go to the wanted LED position.
        addressStrip += position;

        // Set the status to ON if color different of "black" :
        if (addressStrip->status != 0)
        {
            addressStrip->status = 0;
        }
        else
        {
            addressStrip->status = 1;
        }
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
    xdata unsigned int i;

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
        addressStrip++; // Increase address for next position changes ..
    }
    // Enable / Re-activate Timer after Packets Sent :
    TR0 = 1;
}

void strip_Clear(pixel* addressStrip)
{   // Var. Dec. :
    unsigned int i;
    pixel* addressSave = addressStrip;

    for(i = 0; i < MAX_LEDS; i++)
    {	// Clear ALL color at the original address ...
        addressStrip->colorPix.Red = BRIGHT_MIN;
        addressStrip->colorPix.Green = BRIGHT_MIN;
        addressStrip->colorPix.Blue = BRIGHT_MIN;
        addressStrip->status = 0; // Clear status ..
        addressStrip++; // Increase address for next position changes ..
    }
    strip_Show(addressSave);
}

void strip_StatusReset(pixel* addressStrip)
{   // Var. Dec. :
    unsigned int i;

    for(i = 0; i < MAX_LEDS; i++)
    {   // Clear Status to keep color in the original address ...
        addressStrip->status = 0;
        addressStrip++; // Increase address for clearing next position.
    }
}

void strip_Inverter(pixel* addressStrip)
{   // Var. Dec. :
    unsigned int i; // LED Position for filling Board of strip.
    color tmpColor; // Color recovered to invert the actual strip's color.
    // Keep a reference to the first position of the strip.
    pixel* addressRecovery = addressStrip;

    for(i = 0; i < MAX_LEDS; i++)   // First loop to find the first LED alight that
    {                                   // will give the colour to set for the
        if(addressStrip->status != 0)       // reversed status ..
        {   // color found => recovering and ending loop.
            tmpColor = addressStrip->colorPix;
            break;
        }
        addressStrip++; // Increase address for clearing next position.
    }
    for(i = 0; i < MAX_LEDS; i++)   // Second loop to toggle LEDs status.
    {
        if(addressRecovery->status == 0)
        {
            addressRecovery->status = 1;
            addressRecovery->colorPix = tmpColor;
        }
        else
        {
            addressRecovery->status = 0;
            addressRecovery->colorPix.Red = BRIGHT_MIN;
            addressRecovery->colorPix.Green = BRIGHT_MIN;
            addressRecovery->colorPix.Blue = BRIGHT_MIN;
        }
        addressRecovery++; // Increase address for next position changes ..
    }
}

void strip_LimitPos(pixel* addressStrip, color newColor, \
											    unsigned int _Begin, unsigned int _End)
{	// Var. Dec. :
	unsigned int i; // LED Position for filling Board of strip.
	
	for(i = 0; i < MAX_LEDS; i++)
	{
		if((i >= _Begin) && (i <= _End))
		{
			addressStrip->colorPix.Red = newColor.Red;
			addressStrip->colorPix.Green = newColor.Green;
            addressStrip->colorPix.Blue = newColor.Blue;
            addressStrip->status = 1;
		}
		else
		{
			addressStrip->colorPix.Red = BRIGHT_MIN;
			addressStrip->colorPix.Green = BRIGHT_MIN;
			addressStrip->colorPix.Blue = BRIGHT_MIN;
            addressStrip->status = 0;
		}
		addressStrip++;
	}
}

