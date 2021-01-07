/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_strip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : Timers_Wait_Xms.c
|x|  \|\| | | , /    | |        >           (One of my own first c file)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation: 2019.11.21
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                Declarations of RGB's ws2812b LEDs usage funct. .
|x|        /|| |  /
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
// .. definition folder SFR (Like Port Definition "P5", "P6", TR0, etc...)
#include <c8051f020.h>
// .. created header for ws2812b functions usage.
#include "ws2812_strip.h"

//-- GLOBAL VARIABLES INIT : ----------------------------->
xdata pixel strip[MAX_LEDS] = {{0, 0, 0}, 0};   // Initialize everything to 0.

//-------------------------------------------------------------------------------------->
void pixel_Set(pixel* addressStrip, color newColor, unsigned int position)
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
void pixel_Reset(pixel* addressStrip, unsigned int position)
{   // Var. Dec. :
    color black = {0, 0, 0};

    // "position" validity ..
    if(position < MAX_LEDS)
    {   // Go to the wanted LED position.
        addressStrip += position;
        // Set "color" values to black.
        addressStrip->colorPix = black;
        // Set status to '0' (OFF).
        addressStrip->status = 0;
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
    SEND_COLOR_BIT(green, BIT7); SEND_COLOR_BIT(green, BIT6);
    SEND_COLOR_BIT(green, BIT5); SEND_COLOR_BIT(green, BIT4);
    SEND_COLOR_BIT(green, BIT3); SEND_COLOR_BIT(green, BIT2);
    SEND_COLOR_BIT(green, BIT1); SEND_COLOR_BIT(green, BIT0);
    // Sending RED.
    SEND_COLOR_BIT(red, BIT7); SEND_COLOR_BIT(red, BIT6);
    SEND_COLOR_BIT(red, BIT5); SEND_COLOR_BIT(red, BIT4);
    SEND_COLOR_BIT(red, BIT3); SEND_COLOR_BIT(red, BIT2);
    SEND_COLOR_BIT(red, BIT1); SEND_COLOR_BIT(red, BIT0);
    // Sending BLUE.
    SEND_COLOR_BIT(blue, BIT7); SEND_COLOR_BIT(blue, BIT6);
    SEND_COLOR_BIT(blue, BIT5); SEND_COLOR_BIT(blue, BIT4);
    SEND_COLOR_BIT(blue, BIT3); SEND_COLOR_BIT(blue, BIT2);
    SEND_COLOR_BIT(blue, BIT1); SEND_COLOR_BIT(blue, BIT0);
}

//-------------------------------------------------------------------------------------->
void strip_Show(pixel* addressStrip)
{   // Var. Dec. :
    xdata unsigned int i;

    // Disable Timer to avoid interrupting Sending "Packets" :
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
    // Enable / Re-activate Timer after "Packets" Sent :
    TR0 = 1;
}

void strip_Off(pixel* addressStrip)
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
            addressRecovery->colorPix = tmpColor;
            addressRecovery->status = 1;
        }
        else
        {
            addressRecovery->colorPix.Red = BRIGHT_MIN;
            addressRecovery->colorPix.Green = BRIGHT_MIN;
            addressRecovery->colorPix.Blue = BRIGHT_MIN;
            addressRecovery->status = 0;
        }
        addressRecovery++; // Increase address for next position changes ..
    }
}

void strip_ChainedLeds(pixel* addressStrip, color newColor, \
                                               unsigned int begin, unsigned int end)
{	// Var. Dec. :
	unsigned int i; // LED Position for filling Board of strip.
	
	for(i = 0; i < MAX_LEDS; i++)
	{
		if((i >= begin) && (i <= end))
		{
			addressStrip->colorPix.Red = newColor.Red;
			addressStrip->colorPix.Green = newColor.Green;
            addressStrip->colorPix.Blue = newColor.Blue;
            addressStrip->status = 1;
		}
		else { /* Before first use, don't forget to clear strip's status. */ }
		addressStrip++;
	}
}