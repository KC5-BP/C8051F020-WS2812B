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

//===================================================
//================================\HEADER'S_Definitions/================================>
#ifndef	__base_sfr__
#define	__base_sfr__    // __base_sfr__ BEGIN

//----------------------------------\GLOBAL_Definition/---------------------------------.
//-- SYNONYM TYPE       : -------------------------------->
typedef unsigned char   uint8;	/* 8 bits unsigned  */
typedef signed char		int8;	/* 8 bits signed    */
typedef unsigned int	uint16;	/* 16 bits unsigned */
typedef signed int		int16;	/* 16 bits signed   */
typedef unsigned long	uint32;	/* 32 bits unsigned */
typedef signed long		int32;	/* 32 bits signed   */

typedef enum { APP_STATE_INIT = 0,
               APP_STATE_SERVICE_TASKS,
               APP_STATE_WAIT } APP_STATES;
typedef enum { FALSE = 0, TRUE } MY_BOOL;
typedef enum { DOWN = 0, UP } SENS;

//-- REGISTERS DEFINITION 16-BIT : ----------------------->
sfr16 DP        = 0x82;     // data pointer
sfr16 TMR3RL    = 0x92;     // Timer3 reload value
sfr16 TMR3      = 0x94;     // Timer3 counter
sfr16 ADC0      = 0xBE;     // ADC0 data
sfr16 ADC0GT    = 0xC4;     // ADC0 greater than window
sfr16 ADC0LT    = 0xC6;     // ADC0 less than window
sfr16 RCAP2     = 0xCA;     // Timer2 capture/reload
sfr16 T2        = 0xCC;     // Timer2
sfr16 DAC0      = 0xD2;     // DAC0 data
sfr16 DAC1      = 0xD5;     // DAC1 data
sfr16 RCAP4     = 0xE4;     // Timer4 capture/reload
sfr16 T4        = 0xF4;     // Timer4

//-- EQUIVALENCES : -------------------------------------->
#define BIT0    0x01    // Activate mask BIT0
#define BIT1    0x02    // Activate mask BIT1
#define BIT2    0x04    // ..............BIT2
#define BIT3    0x08    // ..............BIT3
#define BIT4    0x10    // ..............BIT4
#define BIT5    0x20    // ..............BIT5
#define BIT6    0x40    // ..............BIT6
#define BIT7    0x80    // ..............BIT7

//-- IN PORTS : ------------------------------------------>
#define BYI_PRESS P6
// sbit only work from P0 to P3, because they're not use as group, like P4 to P7.
// + sbit goes from 0 -> 7.
//	sbit NOTE_DO2 = BYI_PRESS ^ 7;  // Last pressure button have
// some special conditions so I exaled it.
#define BYI_SWITCH P5

//-- INTERRUPTS : ---------------------------------------->

//-- SENSORS : ------------------------------------------->

//-- OUT PORTS : ----------------------------------------->
#define BYO_LEDS P4
#define BYO_OUT_OFF 0x00

#define HALF_0_3 0x0F
#define FST_0_1 0x03
#define SND_2_3 0x0C

#define HALF_4_7 0xF0
#define TRD_4_5 0x30
#define FTH_6_7 0xC0

//-- GLOBAL TYPE		: -------------------------------->
#define SPACE 0x20      // Space value in ASCII for text writing.
#define PERCENT 0x25    // % value in ASCII for text writing.

//-- SWITCH ; Case      : -------------------------------->
#define PIC_NBR 1

//-- TIMING             : -------------------------------->
// #define

//-- GLOBAL VARIABLES	: -------------------------------->
extern APP_STATES StateOfTheMachine;

//-- GLOBAL MACROS ..   : -------------------------------->
//Disable watchdog timer (less than 4 cycles)
#define WATCHDOG_DISABLE(void) {    \
    WDTCN = 0xDE;                   \
	WDTCN = 0xAD;                   \
	XBR2 = 0x40;                    \
}

//===================================================
//===============================\FUNCTIONS'_Prototypes/================================>
//======================================================================================>
/* Description  :   Enable external clock (quartz)                                     */
void SYSCLK_Init(void);
/* Description  :   Ports 0 to 7 Initialization
 *                  Default state P 7 - 0 : Weak Pull up enable /
 *                                          Open Drain / Data 0xFF.                    */
void PORTS_Init(void);
/* Description  :   Initialize the main peripherals (CPU's Clock, Ports).              */
extern void SYS_Init(void);
/* Description  :   Update the State of the State Machine inside the main.             */
extern void updateAppState(APP_STATES NewState);

#endif  // __base_sfr__ END
