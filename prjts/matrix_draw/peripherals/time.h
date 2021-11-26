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
|x|        ',/ |   /                >   Prototypes of timers functions.
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
#ifndef __time_h__
#define __time_h__    // __time__ BEGIN

//----------------------------------\GLOBAL_Definition/---------------------------------.
//-- GLOBAL MACROS ..   : -------------------------------->
//-- TIMER_0 : ---------------------->
// CLOCK DIVISOR
#define	T0_CLK_DIV_12(void) {CKCON &= ~BIT3;}
#define	T0_CLK_DIV_1(void)  {CKCON |= BIT3;}
// INTERNAL / EXTERNAL SYSCLOCK
#define	T0_SYSCLK_INT(void) {TMOD &= ~BIT2;}
#define	T0_SYSCLK_EXT(void) {TMOD |= BIT2;}
// INTERNAL / EXTERNAL COMMAND
#define	T0_GATE0_INT(void)	{TMOD &= ~BIT3;}    // INTERNAL ON/OFF Command.
#define	T0_GATE0_EXT(void)	{TMOD |= BIT3;}		// EXTERNAL ON/OFF Command.
// MODES
#define	T0_MOD_13BITS(void)	{TMOD &= ~BIT0; \
                             TMOD &= ~BIT1;}	    // Mode(0) Counter 13bits.
#define	T0_MOD_16BITS(void)	{TMOD |= BIT0;  \
                             TMOD &= ~BIT1;}	    // Mode(1) Counter 16bits.
#define	T0_MOD_8BITS_AUTO_R(void)   {TMOD &= ~BIT0; \
                                     TMOD |= BIT1;} // Mode(2) Counter 8bits with AutoRld.
#define	T0_MOD_8BITS(void)  {TMOD |= BIT0;  \
                             TMOD |= BIT1;}		    // Mode(3) 2 Counters 8bits.

//-- TIMER_1 : ---------------------->
// CLOCK DIVISOR
#define	T1_CLK_DIV_12(void) {CKCON &= ~BIT4;}
#define	T1_CLK_DIV_1(void)	{CKCON |= BIT4;}
// INTERNAL / EXTERNAL SYSCLOCK
#define	T1_SYSCLK_INT(void)	{TMOD &= ~BIT6;}
#define	T1_SYSCLK_EXT(void)	{TMOD |= BIT6;}
// INTERNAL / EXTERNAL COMMAND
#define	T1_GATE1_INT(void)	{TMOD &= ~BIT7;}	// INTERNAL ON/OFF Command.
#define	T1_GATE1_EXT(void)	{TMOD |= BIT7;}		// EXTERNAL ON/OFF Command.
// MODES
#define	T1_MOD_13BITS(void)	{TMOD &= ~BIT4; \
                             TMOD &= ~BIT5;}    	// Mode(0) Counter 13bits.
#define	T1_MOD_16BITS(void)	{TMOD |= BIT4; \
                             TMOD &= ~BIT5;}	    // Mode(1) Counter 16bits.
#define	T1_MOD_8BITS_AUTO_R(void)	{TMOD &= ~BIT4; \
                                     TMOD |= BIT5;} // Mode(2) Counter 8bits with AutoRld.
#define	T1_MOD_8BITS(void)	{TMOD |= BIT4; \
                             TMOD |= BIT5;}		    // Mode(3) 2 Counters 8bits.

//-- TIMER_2 : ---------------------->
// CLOCK DIVISOR
#define	T2_CLK_DIV_12(void)	{CKCON &= ~BIT5;}
#define	T2_CLK_DIV_1(void)	{CKCON |= BIT5;}
// MODES
// Mode(0) Counter 16bits with AutoRld.
#define	T2_MOD_16BITS_AUTORLD(void) {RCLK0 = 0; \
                                     TCLK0 = 0; \
                                     CPRL2 = 0;}
// Mode(1) Counter 16bits with Capt.
#define	T2_MOD_16BITS_CAPT(void)    {RCLK0 = 0; \
                                     TCLK0 = 0; \
                                     CPRL2 = 1;}
// Mode(2) BaudRate Gen UART0 on SEND.
#define	T2_MOD_UART0_SEND(void)		{RCLK0 = 0; \
                                     TCLK0 = 1; \
                                     CPRL2 = 0;}
// Mode(3) BaudRate Gen UART0 on RECEIP.
#define	T2_MOD_UART0_RECEIP(void)	{RCLK0 = 1; \
                                     TCLK0 = 0; \
                                     CPRL2 = 0;}
// Mode(4) BdRt Gen UART0 on SEND + REC.
#define	T2_MOD_UART0_SD_RC(void)	{RCLK0 = 1; \
                                     TCLK0 = 1; \
                                     CPRL2 = 0;}
// Mode(5) OFF, Start(TR2) = 0.

//-- TIMER_3 : ---------------------->
// CLOCK DIVISOR
#define	T3_CLK_DIV_12(void)	{TMR3CN &= ~BIT1;}
#define	T3_CLK_DIV_1(void)	{TMR3CN |= BIT1;}
// INTERNAL / EXTERNAL SYSCLOCK
#define	T3_SYSCLK_INT(void)	{TMR3CN &= ~BIT0;}
#define	T3_SYSCLK_EXT(void)	{TMR3CN |= BIT0;}   // EXTERNAL System Clock + ! /8 !.
//ENABLE / PRIORITY
#define	T3_INT_DISABLE(void)	{EIE2 &= ~BIT0;}
#define	T3_INT_ENABLE(void)		{EIE2 |= BIT0;}
#define	T3_PRIORITY(void)		{EIP2 |= BIT0;} // Priority 0 = LOW, 1 = HIGH.
//START / STOP / FLAG
#define	T3_START(void)	    {TMR3CN |= BIT2;}   // Start Timer3.
#define	T3_STOP(void)	    {TMR3CN &= ~BIT2;}	// Stop Timer3.
#define	T3_FLAG_RESET(void)	{TMR3CN &= ~BIT7;}  // FLAG reset of Timer3.

//-- TIMER_4 : ---------------------->
// CLOCK DIVISOR
#define	T4_CLK_DIV_12(void)	{CKCON &= ~BIT6;}
#define	T4_CLK_DIV_1(void)	{CKCON |= BIT6;}
// INTERNAL / EXTERNAL SYSCLOCK
#define	T4_SYSCLK_INT(void)	{T4CON &= ~BIT1;}
#define	T4_SYSCLK_EXT(void)	{T4CON |= BIT1;}
// INTERNAL / EXTERNAL COMMAND
#define	T4_GATE4_INT(void)	{T4CON &= ~BIT3;}	// INTERNAL ON/OFF Command.
#define	T4_GATE4_EXT(void)	{T4CON |= BIT3;}	// EXTERNAL ON/OFF Command.
// MODES
// Mode(0) Counter 16bits with AutoRld.
#define	T4_MOD_16BITS_AUTORLD(void)	{T4CON &= ~BIT5; \
                                     T4CON &= ~BIT4; \
                                     T4CON &= ~BIT0;}\
// Mode(1) Counter 16bits with Capt.
#define	T4_MOD_16BITS_CAPT(void)	{T4CON &= ~BIT5; \
                                     T4CON &= ~BIT4; \
                                     T4CON |= BIT0;}
// Mode(2) BaudRate Gen UART1 on SEND.
#define	T4_MOD_UART0_SEND(void)		{T4CON &= ~BIT5; \
                                     T4CON |= BIT4; \
                                     T4CON &= ~BIT0;}
// Mode(3) BaudRate Gen UART1 on RECEIP.
#define	T4_MOD_UART0_RECEIP(void)	{T4CON |= BIT5; \
                                     T4CON &= ~BIT4; \
                                     T4CON &= ~BIT0;}
// Mode(4) BdRt Gen UART1 on SEND + REC.
#define	T4_MOD_UART0_SD_RC(void)	{T4CON |= BIT5; \
                                     T4CON |= BIT4; \
                                     T4CON &= ~BIT0;}
// Mode(5) OFF, Start(TR4) = 0.
//ENABLE / PRIORITY
#define	T4_INT_DISABLE(void)    {EIE2 &= ~BIT2;}
#define	T4_INT_ENABLE(void)		{EIE2 |= BIT2;}
#define	T4_PRIORITY()	        {EIP2 |= BIT2;} // Priority 0 = LOW, 1 = HIGH.
//START / STOP / FLAG
#define	T4_START(void)	        {T4CON |= BIT2;}
#define	T4_STOP(void)           {T4CON &= ~BIT2;}
#define	T4_FLAG_RST_INT(void)	{T4CON &= ~BIT7;}
#define	T4_FLAG_RST_EXT(void)	{T4CON &= ~BIT6;}

//===================================================
//===============================\FUNCTIONS'_Definition/================================>
//======================================================================================>
/* Description  :   STOP every used timers.                                            */
extern void disableTimers(void);
//======================================================================================>
/* Description  :   START every used timers.                                           */
extern void enableTimers(void);

//======================================================================================>
/* Description  :   Timers init.                                                       */
extern void Timer0_init(void);
//======================================================================================>
// extern void Timer1_init(void);
//======================================================================================>
// extern void Timer2_init(void);
//======================================================================================>
// extern void Timer3_init(void);
//======================================================================================>
// extern void Timer4_init(void);

#endif  // __time__ END