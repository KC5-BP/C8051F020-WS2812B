
// Title			: 	Convert_AD_DA
// Source		: 	Convert_AD_DA.c
/* Author		: 	Kévin - Claude - Simon, 
						BOUGNON - PEIGNE*/
// Creation		: 	24.03.2017
// Last_Update	:	02.03.2018
/* Description	: 	Insert the library for the Converter configuration.*/
							
/*========================================================================================>
=========================================================================================*/

#include <c8051f020.h>		// Include definition folder SFR
#include "Base_SFR.h"		// Include SFR, sbit, define, etc...
#include "AD_DA.h"			// Include the library for Convert 8bits AND 12bits.

// Global values :
uint8 byGV_ADC1 = 0, byGV_ADC1_old = 0;

//========================================================================
//==================================\ADC1_8bits_init/=====================================>
//=================================================
void ADC1_8bits_ISR(void)			interrupt 17 
// Fonction
// Description :	Execution of the interruption.
// Date 			:	24.03.2017
// Input			:	Nothing.
// Output	 	:	Nothing.
{	
	// Locals variables :
	
	// Flags reset.
	ADC1_FLAG_RESET();
	TF2 = 0;	// Stop timer while reading.
			
	// Recovery of ADC1.
	byGV_ADC1 = ADC1;
	
	TF2 = 1;	// Stop timer while reading.
}
//---------------------------------------------------------------------------------------->
void ADC1_8bits_init(void)
// Fonction
// Description :	Open the way of the configuration with Crossbar.
// Date 			:	24.03.2017
// Input			:	Nothing.
// Output	 	:	Nothing.
{
	ADC1_REF_ENABLE();		// Activate the reference.
	ADC1_REF_3_3V();			/* Reference to 0 = 2.4V
													  / 1 = 3.3V.*/
	// AMX1SL :
	ADC1_INPUT_USED();			// Inputs select on 8bits.
										// In that case : Input value 0 by default.
	// ADC1CF :
	ADC1_GAIN_1();				// Gain 1.
	ADC1_SPEED_COM();			// SAR1Clock = 2[MHz] => 10.
	// ADC1CN :
	ADC1_TRIGGER();				// Overflow Timer2.
	// Activate Interruption
	ADC1_ENABLE_CONV();			// Activate the Converter.
	ADC1_ENABLE_INT();			// Enable interrupt.
	ADC1_PRIORITY();				// Priority / 1 = HIGH, 0 = LOW.
	
	// Timer2 Part if used Overflow Timer2.
	RCLK0 = 0;			// Use Timer2 with Mode
	TCLK0 = 0;			// 16bits AutoRld.
	CPRL2 = 0;
	EXEN2 = 0;
	CT2 = 0;				// Use SYSCLK /12.
	TR2 = 1;				// Start Timer2.
	ET2 = 0;				// Enable or not its Interrupt.
	RCAP2 = 0xFFA2;		// RCAP2 = 65'536 - (22118400/(32*VitesseDeCommunication))
								// SpeedComm. = 9600 => 65'464 Dec = 0xFFB8 Hex.
	T2 = 0xFFA2;			// Starting value of the Counter with BaudRate Gen.
}