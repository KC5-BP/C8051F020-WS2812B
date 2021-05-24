/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : ws2812_led_manip
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : ws2812_strip.c
|x|  \|\| | | , /    | |            >   (first version of the actual header)
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2019.11.21 (Initial creation date)
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Declarations of RGB's ws2812b LEDs usage funct. .
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
// .. assert function().
//#include <assert.h>
// .. definition folder SFR (Like Port Definition "P5", "P6", "TR0", etc...).
#include <c8051f020.h>
// .. created header for ws2812b led usage.
#include "ws2812_led_manip.h"

//-- GLOBAL VARIABLES INIT : ----------------------------->
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_STRIP
    xdata pixel display[MAX_LEDS] = {{0, 0, 0}, OFF};   // Initialize everything to 0.
#endif
#if __LED_ARRANGEMENT == __LED_ARRANGEMENT_MATRX
    xdata pixel display[MAX_LEDS] = {{0, 0, 0}, OFF, 0, 0};    // Initialize everything to 0.
#endif

//-------------------------------------------------------------------------------------->
void pixel_Set(pixel* addressDisplay, color newColor, posType position) {
    //assert(position < MAX_LEDS);    // Check << position >> validity.

    if(position < MAX_LEDS) {
        addressDisplay += position;
        addressDisplay->colorPix = newColor;

        // Set the status to ON if color different of "black" {0, 0, 0} :
        if ((newColor.Red == BRIGHT_MIN)
            && (newColor.Green == BRIGHT_MIN)
            && (newColor.Blue == BRIGHT_MIN)) {
            // Complement : " == " operator not possible on a complete struct in C.
            addressDisplay->status = (char)OFF;
        } else {
            addressDisplay->status = (char)ON;
        }
    }
}

void pixel_Reset(pixel* addressDisplay, posType position) {
    //assert(position < MAX_LEDS);    // Check << position >> validity.
    const color BLACK = {0, 0, 0};

    if(position < MAX_LEDS) {
        addressDisplay += position;
        addressDisplay->colorPix = BLACK;
        addressDisplay->status = (char)OFF;
    }
}

color pixel_GetColor(pixel* addressDisplay, posType position) {
    //assert(position < MAX_LEDS);    // Check << position >> validity.
    return (addressDisplay + position)->colorPix;
}

ledStatus pixel_GetStatus(pixel* addressDisplay, posType position) {
    //assert(position < MAX_LEDS);    // Check << position >> validity.
    return (addressDisplay + position)->status;
}

void pixel_ToggleStatus(pixel* addressDisplay, posType position) {
    //assert(position < MAX_LEDS);    // Check << position >> validity.

    // Go to the wanted PIXEL position.
    addressDisplay += position;
    // & Reverse status (ON->OFF / OFF->ON)
    addressDisplay->status = !addressDisplay->status;
}

void pixel_Show(unsigned char red, unsigned char green, unsigned char blue) {
    // For the WS2812b, the order is High bit to low AND Green - Red - Blue.
    // Sending GREEN.
    SEND_COLOR_BIT(green, BIT7) /*   */ SEND_COLOR_BIT(green, BIT6)
    SEND_COLOR_BIT(green, BIT5) /*   */ SEND_COLOR_BIT(green, BIT4)
    SEND_COLOR_BIT(green, BIT3) /*   */ SEND_COLOR_BIT(green, BIT2)
    SEND_COLOR_BIT(green, BIT1) /*   */ SEND_COLOR_BIT(green, BIT0)
    // Sending RED.
    SEND_COLOR_BIT(red, BIT7) /*   */ SEND_COLOR_BIT(red, BIT6)
    SEND_COLOR_BIT(red, BIT5) /*   */ SEND_COLOR_BIT(red, BIT4)
    SEND_COLOR_BIT(red, BIT3) /*   */ SEND_COLOR_BIT(red, BIT2)
    SEND_COLOR_BIT(red, BIT1) /*   */ SEND_COLOR_BIT(red, BIT0)
    // Sending BLUE.
    SEND_COLOR_BIT(blue, BIT7) /*   */ SEND_COLOR_BIT(blue, BIT6)
    SEND_COLOR_BIT(blue, BIT5) /*   */ SEND_COLOR_BIT(blue, BIT4)
    SEND_COLOR_BIT(blue, BIT3) /*   */ SEND_COLOR_BIT(blue, BIT2)
    SEND_COLOR_BIT(blue, BIT1) /*   */ SEND_COLOR_BIT(blue, BIT0)
}

//-------------------------------------------------------------------------------------->
void leds_Show(pixel* addressDisplay) {
    xdata posType i;

    // Disable Timer to avoid interrupting Sending "Packets".
    TR0 = 0;
    for(i = 0; i < MAX_LEDS; i++) {
        if(addressDisplay->status == (char)ON)
            pixel_Show(addressDisplay->colorPix.Red, addressDisplay->colorPix.Green, \
                                                            addressDisplay->colorPix.Blue);
        else
            pixel_Show(BRIGHT_MIN, BRIGHT_MIN, BRIGHT_MIN);

        addressDisplay++;
    }
    // Enable / Re-activate Timer after "Packets" Sent.
    TR0 = 1;
}

void leds_Off(pixel* addressDisplay) {
    xdata posType i;

    for(i = 0; i < MAX_LEDS; i++)
        pixel_Reset(addressDisplay, i);
    leds_Show(addressDisplay);
}

void leds_ResetStatus(pixel* addressDisplay) {
    posType i;

    for(i = 0; i < MAX_LEDS; i++)
    {   // Clear Status to keep color in the original address ...
        addressDisplay->status = (char)OFF;
        addressDisplay++; // Increase address for clearing next position.
    }
}

void leds_InvertMono(pixel* addressDisplay) {
    posType i;
    color tmpColor; // Color recovered to invert the actual strip's color.

    for(i = 0; i < MAX_LEDS; i++) { // First loop to find the first LED alight that
                                    // will give the colour to set for the invert.
        if(pixel_GetStatus(addressDisplay, i) == (char)ON) {
            // color found => recovering and ending loop.
            tmpColor = pixel_GetColor(addressDisplay, i);
            break;
        }
    }
    for(i = 0; i < MAX_LEDS; i++) { // Second loop to toggle LEDs status.
        if(pixel_GetStatus(addressDisplay, i) == (char)OFF)
            pixel_Set(addressDisplay, tmpColor, i);
        else
            pixel_Reset(addressDisplay, i);
    }
}

void leds_ChainedLeds(pixel* addressDisplay, color newColor, \
                                               posType begin, posType end) {
    //assert(begin < MAX_LEDS);    // Check << begin >> validity.
    //if(end >= MAX_LEDS) end = MAX_LEDS - 1;
    posType i;
	
	for(i = 0; i < MAX_LEDS; i++)
		if((i >= begin) && (i <= end))
		    pixel_Set(addressDisplay, newColor, i);
}