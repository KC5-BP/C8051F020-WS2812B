//---------------------------------------------------------------------------------------->
// Title		: KBP_Inter_Ext
// Source		: From nothing...
/* Author		: Just some kind of procrastinator .-° */
// Creation		: 14.03.2017
// Last_Update	: 09.12.2019
/* Description	: Insert the base for all the projects.*/

/*========================================================================================>
=========================================================================================*/

//**********************************\Header_Definition/***********************************>
//*****************************\Beginning_Of_The_Definition/******************************>
#ifndef	__base_sfr__
#define	__base_sfr__

// *** NEW TYPES ***
typedef unsigned char	uint8;	/* 8 bits unsigned  */
typedef signed char		int8;	/* 8 bits signed    */
typedef unsigned int	uint16;	/* 16 bits unsigned */
typedef signed int		int16;	/* 16 bits signed   */
typedef unsigned long	uint32;	/* 32 bits unsigned */
typedef signed long		int32;	/* 32 bits signed   */
typedef enum
{   /* Application's state machine's initial state. */
    APP_STATE_INIT = 0,
		APP_STATE_SERVICE_TASKS,
    APP_STATE_WAIT
} APP_STATES;
enum enum_Sens { DOWN = 0, UP };

//-- Registers Definition 16-bit :------------------>
sfr16 DP       = 0x82;		// data pointer
sfr16 TMR3RL   = 0x92;		// Timer3 reload value
sfr16 TMR3     = 0x94;		// Timer3 counter
sfr16 ADC0     = 0xBE;		// ADC0 data
sfr16 ADC0GT   = 0xC4;		// ADC0 greater than window
sfr16 ADC0LT   = 0xC6;		// ADC0 less than window
sfr16 RCAP2    = 0xCA;		// Timer2 capture/reload
sfr16 T2       = 0xCC;		// Timer2
sfr16 DAC0     = 0xD2;		// DAC0 data
sfr16 DAC1     = 0xD5;		// DAC1 data
sfr16 RCAP4    = 0xE4;		// Timer4 capture/reload
sfr16 T4       = 0xF4;		// Timer4

//-- Equivalences :--------------------------------->
#define BIT0	0x01	// Activate mask BIT0
#define BIT1	0x02	// Activate mask BIT1
#define BIT2	0x04	// ..............BIT2
#define BIT3	0x08	// ..............BIT3
#define BIT4	0x10	// ..............BIT4
#define BIT5	0x20	// ..............BIT5
#define BIT6	0x40	// ..............BIT6
#define BIT7	0x80	// ..............BIT7

//-- IN Ports :------------------------------------->
#define	BYI_PRESS P6	// Pressure Button Definition
// sbit only work from P0 to P3, because they're not use as group, like P4 to P7.
// + sbit goes from 0 -> 7.
//	sbit NOTE_DO2 = BYI_PRESS ^ 7;	// Last pressure button have
												// some special conditions so I exaled it.
#define	BYI_SWITCH P5	// Switches definition

//-- Interrupts :----------------------------------->

//-- Sensors :-------------------------------------->

//-- OUT Ports :------------------------------------>
#define BYO_LEDS P4					// LEDs Definition on P4.
	
#define	BYO_OUT_OFF	0x00		// Use to shut Outputs down.


//===================================================
//================================\Header's_Definitions/==================================>
#define SPACE		0x20		// Space value in ASCII for text writing.
#define PERCENT		0x25		// % value in ASCII for text writing.

#define HALF_0_3 0x0F
#define HALF_4_7 0xF0
#define FST_0_1 0x03
#define SND_2_3 0x0C
#define TRD_4_5 0x30
#define FTH_6_7 0xC0

//-- SWITCH ; Case :-------------------------------->
#define	PIC_NBR			1

//-- TIME ; Blink / Period / T_On :----------------->
// #define

//===================================================
//===================================\Header's_Macros/====================================>

//-------------------------\Funct._Proto._Dec.+_Initializations/------------------------->
//Disable watchdog timer (less than 4 cycles)
#define WATCHDOG_DISABLE(void)\
{	WDTCN = 0xDE;\
	WDTCN = 0xAD;\
	XBR2 = 0x40; }

//===================================================
//=================================\External_Global_Var/==================================>
extern APP_STATES StateOfTheMachine;

//===================================================
//===========================\Function_Prototypes_Declaration/============================>
void SYSCLK_Init(void); 	// Enable external clock (22.1184MHz)
void Ports_Init(void);		//	Ports Initialization.
extern void SYS_Init(void); // Initialized the essential (CPU's Clock, Ports).
// Change the State of the Main StateMachine.
extern void UpdateState(APP_STATES NewState);

#endif
//**********************************\End_Of_Definition/***********************************>
