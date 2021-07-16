/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : base_sfr
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : None
|x|  \|\| | | , /    | |
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2017.03.14
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Insert the base for all the projects.
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
// .. definition folder SFR (Like Port Definition "P5", "P6", "TR0", etc...)
#include <c8051f020.h>
#include "base_sfr.h"

//-- GLOBAL VARIABLES INIT : ----------------------------->
APP_STATES StateOfTheMachine = APP_STATE_INIT;

//===================================================
//===============================\FUNCTIONS'_Definition/================================>
//======================================================================================>
void SYSCLK_Init(void) {
    uint8 i;        // Counter for the starting delay of the oscillator.

    OSCXCN = 0x67;  // Starting of the External Oscillator (22.1184MHz ).

    for (i = 0; i < 255; ++i)   { /* Waiting start oscillator. */ }

    while ( !(OSCXCN & BIT7) )  { /* Waiting for stabilization of the oscillator. */ }

    OSCICN = 0x88;  // Select external oscillator for SYSCLK
                    // AND enable the oscillation error detector.
}

//======================================================================================>
void Ports_Init(void) {
    #define BYC_OPEN_DRAIN 	0x00	// More likely for Inputs
    #define BYC_PUSH_PULL 	0xFF	// and Outputs.

//-- PORT 0 : -------------------------------------->
    // Digital Outputs:
    //P0MDOUT = BYC_OPEN_DRAIN ;	// Port P0 in Open Drain(default state)
    //P0MDOUT = 0xE1;		// UART0-Tx : Output on .0
    // UART0-Rx : Input on .1
    // Then Half Input .2 -> .4
    // Then Half Output .5 -> .7
    //P0 = 0xFF ;		// Enable Port P0 reading on the first HALF.(default state 0xFF)

    // Digital inputs:
    // P0MDOUT = 0x15 ;				// Port P0 in Open Drain(default state)
    // P0 = 0xFF ;						// Enable Port P0 reading(valeur au reset FF)

//-- PORT 1 : -------------------------------------->
    // Digital Outputs:
    //P1MDOUT = BYC_OPEN_DRAIN ;	// Port P1 in Open Drain(default state)
    //P1MDOUT = BYC_PUSH_PULL;		// Port P1 in Push Pull
    //P1 = 0xFF ;						// Enable Port P1 reading(default state 0xFF)

    // Digital inputs:
    //P1MDOUT = BYC_OPEN_DRAIN ;		// Port P1 in Open Drain(default state)
    //P1 = 0xFF ;							// Enable Port P1 reading(valeur au reset FF)

    // Analog Inputs:
    //P1MDOUT = BYC_OPEN_DRAIN ;	// Port P1 in Open Drain(default state)
    //P1 = 0xFF;						// high impedance
    //P1MDIN = 0;							// weak pull up and digital input of P1 disabled

//-- PORT 2 : -------------------------------------->
    // Digital Outputs:
    //P2MDOUT = BYC_OPEN_DRAIN ;	// Port P2 in Open Drain(default state)
    P2MDOUT = 0xF0;					// Half Input .0 -> .3
    // Half Output .4 -> .7 with .5 for Strip IN (Out of CPU).
    P2 = 0x0F ;							// Enable Port P2 reading on the Inputs.

    // Digital inputs:
    //P2MDOUT = BYC_OPEN_DRAIN ;	// Port P2 in Open Drain(default state)
    //P2 = 0xFF ;						// Enable Port P2 reading(default state 0xFF)

//-- PORT 3 : -------------------------------------->
    // Digital Outputs:
    //P3MDOUT = BYC_OPEN_DRAIN ;	// Port P3 in Open Drain(default state)
    //P3MDOUT = BYC_PUSH_PULL;		// Port P3 in Push Pull
    //P3 = 0x00 ;						// Enable Port P3 reading(default state 0xFF)

    // Digital inputs:
    //P3MDOUT = BYC_OPEN_DRAIN ;	// Port P3 in Open Drain(default state)
    //P3 = 0xFF ;						// Enable Port P3 reading(default state 0xFF)

    XBR2 &= (~BIT7); 	//active weak pull-up for Ports 0 - 3 (default state)
    //XBR2 |= (BIT7); //Disable weak pull-up
    XBR2 |= (BIT6); 	// XBARE: enable crossbar (active toutes les I/O)

//-- PORT 4-7 : ------------------------------------>
    //P74OUT = 0000$0011Y;					// 4 x 2 nibble bits Open Drain
    //P74OUT = 00000011Y;
    P74OUT = 0x03;
    //		bit 7 - 6: High and low nibble bits Port 7
    //		bit 5 - 4: High and low nibble bits Port 6
    //		bit 3 - 2: High and low nibble bits Port 5
    //		bit 1 - 0: High and low nibble bits Port 4
    // P4 : Push-Pull.
    //P74OUT = 0b11111111;	// 4 x 2 nibble bits en Push Pull

    P4 = 0x00 ;			// Disable Port P4 reading, 'cause it's Outputs (default state 0xFF).
    P5 = 0xFF ;			// Enable Port P5 reading(default state 0xFF).
    P6 = 0xFF ;			// Enable Port P6 reading(default state 0xFF).
    //P7 = 0xFF ;			// Enable Port P7 reading(default state 0xFF).//Not used, so unmanaged.

    #undef BYC_OPEN_DRAIN
    #undef BYC_PUSH_PULL
}

//======================================================================================>
void SYS_Init(void) {
    SYSCLK_Init();
    WATCHDOG_DISABLE()
    PORTS_Init();
}

//======================================================================================>
void updateAppState(APP_STATES NewState) {
    StateOfTheMachine = NewState;
}
