
// Title			: 	Convert_AD_DA
// Source		: 	Convert_AD_DA.h
/* Author		: 	Kévin - Claude - Simon, 
						BOUGNON - PEIGNE*/
// Creation		: 	24.03.2017
// Last_Update	:	09.12.2017
/* Description	: 	Insert the library for theConverter AD_DA configuration.*/
							
/*========================================================================================>
=========================================================================================*/

#include "Base_SFR.h"	// Include SFR, sbit, define, etc...

// Library Definition :
// Beginning of definition...
#ifndef	__AD_DA__
#define	__AD_DA__

// Define of the library.
// Inputs of the Converter 12bits are all connected on P0.
// Inputs of the Converter 8bits are all connected on P1.
#define VAL_MAX_X 	100	// Max value before set to 0.
#define VAL_START_X	120	// Starting value of X.
#define VAL_START_Y	120	// Starting value of Y.
#define VAL_0			0		// 0 Value.

//========================================================================
//=============================\Interrupt_ADC0_12bits/====================================>
//=================================================
//REFERENCE
#define	ADC0_REF_ENABLE() 	{REF0CN |= (BIT0 | BIT1);}	// Activate the reference.
#define	ADC0_TEMP_SENS()		{REF0CN |= BIT2;}		// Temp Sensor : 0=Disable / 1=Enable.
#define	ADC0_REF_2_4V() 		{REF0CN &= ~BIT4;}		// Reference select.
#define	ADC0_REF_0TO2_4V() 	{REF0CN |= BIT4;}		/* Ref fixed to 0 = 2.4V basic 
																				  / 1 = 0V -> 2.4V via DAC0.*/
//INPUT
#define	ADC0_DIFF_NONE()		{AMX0CF &= ~(BIT0 | BIT1 | BIT2 | BIT3);}
#define	ADC0_DIFF_0()			{AMX0CF |= BIT0;\
										 AMX0CF &= ~(BIT1 | BIT2 | BIT3);}	// IN+ .0 / IN- .1
#define	ADC0_DIFF_1()			{AMX0CF |= BIT1;\
										 AMX0CF &= ~(BIT0 | BIT2 | BIT3);}	// IN+ .2 / IN- .3
#define	ADC0_DIFF_2()			{AMX0CF |= BIT2;\
										 AMX0CF &= ~(BIT0 | BIT1 | BIT3);}	// IN+ .4 / IN- .5
#define	ADC0_DIFF_3()			{AMX0CF |= BIT3;\
										 AMX0CF &= ~(BIT0 | BIT1 | BIT2);}	// IN+ .6 / IN- .7
#define	ADC0_INPUT_USED_0()	{AMX0SL &= ~(BIT0 | BIT1 | BIT2);}// By default Input 0 used..
																				// BIT 0 -> 2 Input to convert.
#define	ADC0_INPUT_USED_1()	{AMX0SL |= BIT0;}
#define	ADC0_TP_SENS_USED()	{AMX0SL |= BIT3;}					// BIT 3 Temp sensor select.
//PARAMETERS
#define	ADC0_GAIN_1()		{ADC0CF &= ~(BIT0 | BIT1 | BIT2);}	
#define	ADC0_GAIN_2()		{ADC0CF |= BIT0;\
									 ADC0CF &= ~(BIT1 | BIT2);}
#define	ADC0_GAIN_4()		{ADC0CF |= BIT1;\
									 ADC0CF &= ~(BIT0 | BIT2);}
#define	ADC0_GAIN_8()		{ADC0CF |= (BIT0 | BIT1);\
									 ADC0CF &= ~BIT2;}					// Gain select.
#define	ADC0_GAIN_16()		{ADC0CF |= BIT2;\
									 ADC0CF &= ~(BIT0 | BIT1);}
#define	ADC0_GAIN_0_5()	{ADC0CF |= (BIT2 | BIT1);\
									 ADC0CF &= ~BIT0;}
#define	ADC0_SPEED_COM()	{ADC0CF |= (BIT6 | BIT4);\
									 ADC0CF &= ~(BIT7 | BIT5 | BIT3);}
																			//AD0SC=((SYSCLK/SAR1Clock) - 1).
																			// SAR0Clock = 2[MHz] => 10.
//INTERRUPT PARAMETERS
#define	ADC0_STG_BIT()		{AD0LJST = 0;}	// Strong bit on Right position.
#define	ADC0_COMP_FLAG()	{AD0WINT = 0;}	// Compare flag interrupt.
#define	ADC0_TRIGGER()		{AD0CM0 = 1;\
									 AD0CM1 = 1;}	// 00 > Write to AD0BUSY.
														// 01 > Overflow Timer3.
														// 10 > Ext.Input.CNVSTR(Cross.XBR2.0).
														// 11 > Overflow Timer2.
#define	ADC0_ENABLE_CONV()	{AD0EN = 1;}		// Activate converter.
#define	ADC0_ENABLE_INT()		{EIE2 |= BIT1;}	// Enable Interrupt.
#define	ADC0_PRIORITY()		{EIP2 |= BIT1;}	// Priority level / 1 = HIGH, 0 = LOW.
//FLAG
#define	ADC0_FLAG_RESET()		{AD0INT = 0;}		// Flag reset to 0.
//========================================================================
//==============================\Interrupt_ADC1_8bits/====================================>
//=================================================
//REFERENCE
#define	ADC1_REF_ENABLE()	{REF0CN |= (BIT0 | BIT1);}	// Activate the reference.
#define	ADC1_REF_2_4V() 	{REF0CN &= ~BIT3;}			// Reference select.
#define	ADC1_REF_3_3V() 	{REF0CN |= BIT3;}
//INPUT
#define	ADC1_INPUT_USED()	{AMX1SL &= ~(BIT0 | BIT1 | BIT2);}	// By default Input 0 used..
//PARAMETERS
#define	ADC1_GAIN_0_5()	{ADC1CF &= ~(BIT0 | BIT1);}
#define	ADC1_GAIN_1()		{ADC1CF |= BIT0;\
									 ADC1CF &= ~BIT1;}					// Gain select.
#define	ADC1_GAIN_2()		{ADC1CF &= ~BIT0;\
									 ADC1CF |= BIT1;}
#define	ADC1_GAIN_4()		{ADC1CF |= (BIT0 | BIT1);}
#define	ADC1_SPEED_COM()	{ADC1CF |= (BIT6 | BIT4);\
									 ADC1CF &= ~(BIT7 | BIT5 | BIT3);}
																			//AD1SC=((SYSCLK/SAR1Clock) - 1).
																			// SAR1Clock = 2[MHz] => 10.
#define	ADC1_TRIGGER()		{ADC1CN |= (BIT1 | BIT2);\
									 ADC1CN &= ~BIT3;}		// 000 > Write to AD1BUSY.
																	// 001 > Overflow Timer3.
																	// 010 > Ext.Input.CNVSTR(Cross.XBR2.0).
																	// 011 > Overflow Timer2.
																	// 1xx > Write to AD0BUSY
																	// (Synchronized with ADC0).
//INTERRUPT PARAMETERS
#define	ADC1_ENABLE_CONV()	{ADC1CN |= BIT7;}		// Activate converter.
#define	ADC1_ENABLE_INT()		{EIE2 |= BIT3;}		// Enable Interrupt.
#define	ADC1_PRIORITY()		{EIP2 |= BIT3;}		// Priority level / 1 = HIGH, 0 = LOW.
//FLAG
#define	ADC1_FLAG_RESET()		{ADC1CN &= ~BIT5;}	// Flag reset to 0.
//---------------------------------------------------------------------------------------->

// Global Variables :
extern uint8 byGV_ADC1, byGV_ADC1_old;

// Function Prototypes :
extern void ADC0_12bits_init(void);	// Interrupt ADC0.
extern void ADC1_8bits_init(void);	// Interrupt ADC1.

#endif
// End of definition.