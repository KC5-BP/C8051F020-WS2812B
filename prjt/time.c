
// Title			: 	Timers_Wait_Xms
// Source		: 	Timers_Wait_Xms.c
/* Author		: 	Kévin - Claude - Simon,
						BOUGNON - PEIGNE*/
// Creation		: 	14.03.2017
// Last_Update	:	16.03.2018
/* Description	: 	Insert the library for the Timers.*/

/*========================================================================================>
=========================================================================================*/

#include <c8051f020.h>		// Include definition folder SFR
#include "base_sfr.h"		// Include Base(define, sbit, sfr,...)
#include "time.h"				// Include Timers function and Waiting Function.

// Gloabl value :
//uint8 byGV_Count_Timer = 0;		// Start value of counter to 0.

void disableTimers(void) {
	TR0 = 0;
	//TR1 = 0;
	TR2 = 0;
	//T3_STOP()
	//T4_STOP()
}

void enableTimers(void) {
	TR0 = 1;
	//TR1 = 1;
	TR2 = 1;
	//T3_START()
	//T4_START()
}

//========================================================================
//================================\InterruptTimer0/=======================================>
//=================================================
void Timer0_ISR (void)		interrupt 1
// Description :	Timer0 interrupt trigger every 1ms.
// Date 			:	24.03.2017
// Input			:	Nothing
// Output	 	:	Nothing
{	// Var. Dec. : 
	static xdata uint16 ui16_Cnt_INIT = 0;	// Counter for Init.
	static xdata uint8 ui8_Cnt_TASKS = 100;	// Counter for Service Tasks.
	
	// Reset counter value from 63'691.
	// For a time of 1ms.
	TL0 = 0xCB;
	TH0 = 0xF8;
	
  // 2[s] = 2'000[ms] : 1[ms] * 1998 (0 -> 1999 => 2000 iterations + 
																									// One increasing before changing state).
	if(ui16_Cnt_INIT < (2000 - 2))
	{
		ui16_Cnt_INIT++;
	}
	else
	{
		if(++ui8_Cnt_TASKS >= 10)	// Go to Service Tasks every 10ms.
		{	// Update State of the State Machine :
			UpdateState(APP_STATE_SERVICE_TASKS);
			
			// Reset Service Tasks counter :
			ui8_Cnt_TASKS = 0;
		}
		else
		{ /* Nothing, because we pre-increase the counter in condition already. */ }
	}
	
// 	// Reset counter value from 63'691.
// 	// For a time of 1ms.
// 	TL0 = 0xCB;
// 	TH0 = 0xF8;
}
//------------------------------------------------->
void Timer0_init (void)
// Fonction
// Description	:	Initialization of Interrupt with time of ...[ms/us].
// Date 			:	01.03.2018
// Input			:	Nothing
// Output	 	:	Nothing
{
	T0_CLK_DIV_12();	// SYSCLK/12 = 1.8432 [MHz].
	T0_SYSCLK_INT();	// System Internal clock.
//	XBR1 |= BIT1; 		// External clock enable in Crossbar.
	T0_GATE0_INT();	// Internal command.
// XBR1 |= BIT2;		// External Input enable in Crossbar.
	T0_MOD_16BITS();	// Mode Counter 16 bits.
		// Flag autoreset but if needed : TF0.
	ET0 = 1;					// Activate Interrupt Timer0 or not.
	PT0 = 1;					// Priority 0 = LOW, 1 = HIGH.
	TL0 = 0xCB;				// Start value set to "65'536-((WishedTime*22118400)/Div)"
	TH0 = 0xF8;				// Div = 12 ; 47'104-0xB800 for 10ms.//63'692-0xF8CC for 1ms.
								// 			  65'351-0xFF47 for 100us.//65'517-0xFFED for 10us.
								// 			  65'534-0xFFFE for 1us.
								// Change if you need a different application.
	/* Start Out with TR0 = 1; || or possibly HERE !.*/
}



// //========================================================================
// //================================\InterruptTimer1/=======================================>
// //=================================================
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
