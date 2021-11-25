/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : time
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : None
|x|  \|\| | | , /    | |
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : 2017.03.14
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Bodies of timers functions.
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
#include <c8051f020.h>
#include "base_sfr.h"
#include "time.h"

//-- GLOBAL VARIABLES INIT : ----------------------------->
//uint8 byGV_Count_Timer = 0;		// Start value of counter to 0.

//===================================================
//===============================\FUNCTIONS'_Definition/================================>
//======================================================================================>
extern void disableTimers(void) {
    TR0 = 0;
    //TR1 = 0;
    TR2 = 0;
    //TR3_STOP()
    //TR4_STOP()
}
//======================================================================================>
extern void enableTimers(void) {
    TR0 = 1;
    //TR1 = 1;
    TR2 = 1;
    //TR3_START()
    //TR4_START()
}

//======================================================================================>
void Timer0_ISR(void) interrupt 1 {
    static xdata uint16 cntInit = 0;
    static xdata uint8 cntTasks = 100;

    // Reset counter value to 63'691 for a time of 1ms.
    TH0 = 0xF8; TL0 = 0xCB;

    // 2[s] = 2'000[ms] : 1[ms] * 1998 (0 -> 1999 => 2000 iterations +
    // One increasing before changing state).
    if( cntInit < (2000 - 2) )
        ++cntInit;
    else {
        if(++cntTasks >= 10) {
            cntTasks = 0;
            updateAppState(APP_STATE_SERVICE_TASKS);
        }
    }
}
//----------------------------------------------------------------->
void Timer0_init(void) {
    T0_CLK_DIV_12()
    T0_SYSCLK_INT()
//	XBR1 |= BIT1;       // External clock enable in Crossbar.
    T0_GATE0_INT()      // Internal command.
// XBR1 |= BIT2;        // External Input enable in Crossbar.
    T0_MOD_16BITS()
    // Flag autoreset but if needed : Register is TF0.
    ET0 = 1;            // Activate Interrupt Timer0 or not.
    PT0 = 1;            // Priority : 0 = LOW, 1 = HIGH.
    TH0 = 0xF8;         // Start value set to "65'536-((WishedTime*22118400)/Div)"
    TL0 = 0xCB;
    /* Start Out with TR0 = 1; || or possibly HERE !.*/
}

//======================================================================================>
// void Timer1_ISR (void)		interrupt 3
// // Fonction
// // Description :	Execute this part when the interrupt is triggered...
// // Date 			:	24.01.2017
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	// Nothing.
// }
// //------------------------------------------------->
// void Timer1_init (void)
// // Fonction
// // Description	:	Initialization of Interrupt with time of ...[ms/us].
// // Date 			:	01.03.2018
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	T1_CLK_DIV_12();	// SYSCLK/12 = 1.8432 [MHz].
// 	T1_SYSCLK_INT();	// System Internal clock.
// //	XBR1 |= BIT3; 		// External clock enable in Crossbar.
// 	T1_GATE1_INT();	// Internal command.
// // XBR1 |= BIT4;		// External Input enable in Crossbar.
// 	T1_MOD_16BITS();	// Choose Mode 16 bits.
// 		// Flag autoreset but if needed : TF1.
// 	ET1 = 0;					// Activate Interrupt Timer1 or not.
// 	PT1 = 0;					// Priority 0 = LOW, 1 = HIGH.
// 	TL1 = 0xA3;				// Same operation as Timer0.
// 	TH1 = 0xFF;				// "65'536-((WishedTime*22118400)/Div)"
// 								// Change if you need a different application.
// 	/* Start Out with TR1 = 1; || or possibly HERE !.*/
// }

// //========================================================================
// //================================\InterruptTimer2/=======================================>
// //=================================================
// void Timer2_ISR (void)		interrupt 5
// // Fonction
// // Description :	Execute this part when the interrupt is triggered...
// // Date 			:	26.01.2017
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	// Flag reset.
// 	TF2 = 0;			/*EXF2 = External Int.*/
// }
// //------------------------------------------------->
// void Timer2_init (void)
// // Fonction
// // Description	:	Initialization of Interrupt with time of ...[ms/us].
// // Date 			:	26.01.2017
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	T2_CLK_DIV_12();	// SYSCLK/12 = 1.8432 [MHz].
// 	CT2 = 0;				// System Internal clock || (CT2 = 1) External clock.
// //	XBR1 |= BIT5; 		// External clock enable in Crossbar.
// 	EXEN2 = 0;			// 0 = Internal command, 1 = External command. OR T2CON.3.
// // XBR1 |= BIT6;		// External Input enable in Crossbar.
// 	T2_MOD_16BITS_AUTORLD();	// Choose 16bits with AutoRld.
// 		// Flag reset in ISR / TF2 = Internal Int. // EXF2 = External Int.
// 	ET2 = 1;					// Activate Interrupt Timer2 or not.
// 	PT2 = 1;					// Priority 0 = LOW, 1 = HIGH.
// 	T2 = 0xB800;			// Same operation as Timer0.
// 								// "65'536-((WishedTime*22118400)/Div)"
// 								// Change if you need a different application.
// 	RCAP2 = 0xB800;		// Starting Reload value.
// 	/* Start Out with TR2 = 1; || or possibly HERE !.*/
// }

// //========================================================================
// //================================\InterruptTimer3/=======================================>
// //=================================================
// void Timer3_ISR (void)		interrupt 14
// // Fonction
// // Description :	Execute this part when the interrupt appear.
// // Date 			:	26.01.2017
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	// Flag reset / Bit no addressable.
// 	T3_FLAG_RESET();
// }
// //------------------------------------------------->
// void Timer3_init (void)
// // Fonction
// // Description	:	Initialization of Interrupt with time of ...[ms/us].
// // Date 			:	26.01.2017
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	T3_CLK_DIV_12();	// SYSCLK/12 = 1.8432 [MHz].
// 	T3_SYSCLK_INT();	// System Internal clock.
// 			// No condition of Internal/External Reload about TOE-> See page Timer3.
// //	XBR0 |= BIT0;		// External command reload.
// 		// Flag reset in ISR / T3_FLAG_RESET().
// 	T3_INT_ENABLE();	// Activate Interrupt Timer3 or not.
// 	T3_PRIORITY();		// Priority 0 = LOW, 1 = HIGH.
// 	TMR3 = 0x00;		// Same operation as Timer0.
// 							// "65'536-((WishedTime*22118400)/Div)"
// 							// Change if you need a different application.
// 	TMR3RL = 0x00;		// Starting Reload value.
// 			/*		!TIMER 3 HAVE AUTORELOAD MODE ONLY! 	*/
// 	/* Start Out with T3_START() and Stop with T3_STOP() || or possibly Start HERE !.*/
// }

// //========================================================================
// //================================\InterruptTimer4/=======================================>
// //=================================================
// void Timer4_ISR (void)		interrupt 16
// // Fonction
// // Description	:	Execute this part when the interrupt appear.
// // Date 			:	01.03.2018
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	// Flag reset / Bit no addressable.
// 	T4_FLAG_RST_INT();	/*T4_FLAG_RST_EXT() = External Int.*/
// }
// //------------------------------------------------->
// void Timer4_init (void)
// // Fonction
// // Description	:	Initialization of Interrupt with time of ...[ms/us].
// // Date 			:	01.03.2018
// // Input			:	Nothing
// // Output	 	:	Nothing
// {
// 	T4_CLK_DIV_12();	// SYSCLK /12= 1.84320MHz || (|= BIT6 )= 22.1184MHz.
// 	T4_SYSCLK_INT();	// System clock(Internal clock). || (|= BIT1)= External clock.
// //	XBR2 |= BIT3; 		// External clock enable in Crossbar.
// 	T4_GATE4_INT();	// Internal command.
// // XBR2 |= BIT4;		// External Input enable in Crossbar.
// 	T4_MOD_16BITS_AUTORLD();	// Choose 16bits with AutoRld.
// 		// Flag reset in ISR / T4_FLAG_RST_INT() = Internal Int. // .._RST_EXT = External Int.
// 	T3_INT_ENABLE();	// Activate Interrupt Timer4 or not.
// 	T4_PRIORITY();		// Priority 0 = LOW, 1 = HIGH.
// 	T4 = 0x00;				// Same operation as Timer0.
// 								// "65'536-((WishedTime*22118400)/Div)"
// 								// Change if you need a different application.
// 	RCAP4 = 0x00;			// Starting Reload value.
// 	/* Start Out with T4_START() and Stop with T4_STOP() || or possibly Start HERE !.*/
// }
