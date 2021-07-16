/*,..---------------------------------------------------------------------------------..,
 /´/																				   \`\
|x| __--""'\																			|x|
|x|  ,__  -'""`;												            __-'´´---__ |x|
|x| /   \  /"'  \												         ./´,__         |x|
|x|   __// \-"-_/												        /  /   \        |x|
|x| ´"   \  |				> Title		:	F04_StateMachine	        (/     |        |x|
|x| \     |  \  _.-"',											              /         |x|
|x| "^,-´\/\  '" ,--. \		> Creation	:	dd.mm.yyyy			 /"--_  _.   |          |x|
|x|  \|\| | | , /    | |		'-> Last Update :	dd.mm.yyyy	|     \ \ \,/           |x|
|x|     '`'\|._ |   / /											 \  __ \/__/       _-,  |x|
|x|         '\),/  / |		> Description : /n					  "---\          //   \ |x|
|x|           |/.-"_/			° Project's Description			      |\        ||      |x|
|x| .__---+-_/'|--"				  Blablablablablabla 			    :´  "-,     | \,    |x|
|x|         _| |_--,			  Blablablablablablablablabla.		 \-,   \     \  "-. |x|
|x|        ',/ |   /											        \  /".-^  |     |x|
|x|        /|| |  /												        | | |\ /. ,'.,  |x|
|x|     |\| |/ |- |												        /  \/ : |\ /| \ |x|
|x| .-,-/ | /  '/-"												        \  | /  | | _\/ |x|
|x| -\|/-/\/ ^,'|												         ".:/  _\ \/ |\ |x|
|x| _-     |  |/												           \,   ,'-´' " |x|
|x|  .  --"  /													             '--_\      |x|
|x| /--__,.-/													                   "'-- |x|
 \`\___________________________________________________________________________________/´/
  \_____________________________________________________________________________________/*/
/*========================================================================================>
=========================================================================================*/
// Linker to : ...
#include <c8051f020.h>		// ... definition folder SFR.
#include "base_sfr.h"	// ... Base (SFR, sbit, define, var. type, etc...)
#include "time.h"		// ... Timers 0-1-2-3-4 AND Waiting Function.
#include "AD_DA.h"
// ... Functions to WS281x purposes.
#include "../headers/led_manip/ws2812_led_manip.h"

// HELPS : 
// https://stackoverflow.com/questions/13706809/structs-in-c-with-initial-values
// Board can only be of 128bytes max. :
/* https://www.google.com/search?client=firefox-b-d&q=keil+error+c249+%27data%27+segment
+too+large */

#define	PRESS_MODE_DN (BYI_PRESS & BIT0)
#define	PRESS_MODE_UP (BYI_PRESS & BIT1)

#define	PRESS_RED_DN (BYI_PRESS & BIT2)
#define	PRESS_RED_UP (BYI_PRESS & BIT3)

#define	PRESS_GRN_DN (BYI_PRESS & BIT4)
#define	PRESS_GRN_UP (BYI_PRESS & BIT5)

#define	PRESS_BLU_DN (BYI_PRESS & BIT6)
#define	PRESS_BLU_UP (BYI_PRESS & BIT7)

#define STEP_COLOR 0x18

//===================================================
//=================================\Global_Definitions/==================================>

//===================================================
//===========================\Function_Prototypes_Declaration/============================>
void PERIPH_Init(void);	// Initialized the Peripherals (Timers, Ext. Int., ADC).

uint8 PRESS_Modifier(uint8 _PressInput, uint8* _ui8_Dest, uint8 _ui8_DeBounce, \
uint8 _ui8_Step, uint8 _ui8_Sens, uint16 _ui16_MaxLimit);

uint8 PRESS_ModifierLoop(uint8 _PressInput, uint8* _ui8_Dest, uint8 _ui8_DeBounce, \
uint8 _ui8_Step, uint8 _ui8_Sens, uint16 _ui16_MinLimit, uint16 _ui16_MaxLimit);

//========================================================================
//==================================\Global_Variables/====================================>

//===================================================
//====================================\Global_Boards/=====================================>
#define MAP_FN(type) \
type map_##type(type x, type in_min, type in_max, type out_min, type out_max) {	\
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);	\
}

MAP_FN(uint8)

//========================================================================
//======================================\Main_Code/=======================================>
//===================================================
void Main(void)
//Description	: Contain the Main code.
//Input			: Nothing.
//Output		: Nothing.
{
//-Local Variables---------------------------------->
	enum MODE {COLOR_ALL = 1, ONE_AFTER_ONE, STATIC_FADER, AUDIO_REACT};
	const uint8 TOT_MODES = 5-1;
	xdata uint8 stateInTasks = 0, stateInTasks_OLD;
// By modes : 
  //xdata pixel* display = displayInit(MAX_LEDS);
	//xdata pixel* display = displayInit(9);
// COLOR_ALL :
	xdata uint8 Red_OLD = 0, Green_OLD = 0, Blue_OLD = 0;
// STATIC_FADER : 
	xdata uint8 shiftStatus = 0;
	xdata uint8 shiftingDelay = 0;
	xdata uint8 greenSens = 0;
	xdata uint8 faderSpeed = 5;
// ONE_AFTER_ONE :
	xdata posType i = 0;
	xdata unsigned char oneOnTwo = 0;
	xdata unsigned char sens = 0, sens_OLD = 0;
// AUDIO_REACT :
	xdata unsigned char tempo = 0;

	// For loop :
	uint16 j;
	
	// For Color Mngmt :
	xdata color col_strip = {0, 0, 0};

//-Counter------------------------------------------>

//-Flags-------------------------------------------->
	xdata uint8 ui8_FlagDispl = 0, ui8_FlagClr = 0;
	
//-Debouncing Flags--------------------------------->
	xdata uint8 ui8_PressBtn_0 = 0, ui8_PressBtn_1 = 0;
	xdata uint8 ui8_PressBtn_2 = 0, ui8_PressBtn_3 = 0;
	xdata uint8 ui8_PressBtn_4 = 0, ui8_PressBtn_5 = 0;
	xdata uint8 ui8_PressBtn_6 = 0, ui8_PressBtn_7 = 0;

	// Use of external CLK (22.1184MHz) + Ports 8 -> 0 initialization
	SYS_Init();

/*---------------------."'-._@\|':.(o'\__MAIN_Loop__/'o).:'|/@_.-'".---------------------*/
	if( display ) {
		while( 1 ) {
	//****************************************\CODE/******************************************>
			switch(StateOfTheMachine)
			{
				case APP_STATE_INIT :
	//- Initialization_of_different_Interruption ----->
					PERIPH_Init();

					// Shut the Output down ! IF NEEDED !.
					BYO_LEDS = BYO_OUT_OFF;
					// Shut the Matrix Down : In case of.
					leds_Off(display);
					
					//stateInTasks = COLOR_ALL;
					stateInTasks = AUDIO_REACT;
				
					// Start Timer(s) :
				  enableTimers();
					j = 0;
					// Switch to .._WAIT :
					UpdateState(APP_STATE_WAIT);
					break;
				
				case APP_STATE_SERVICE_TASKS : // Come every 10ms
					// Reading the inputs to detect if a change occured.
					ui8_PressBtn_0 = PRESS_Modifier(PRESS_MODE_DN, &stateInTasks, ui8_PressBtn_0, 1, \
																	DOWN, TOT_MODES);
					ui8_PressBtn_1 = PRESS_Modifier(PRESS_MODE_UP, &stateInTasks, ui8_PressBtn_1, 1, \
																	UP, TOT_MODES);
				
					// =============================================================================>
					if(stateInTasks != stateInTasks_OLD)
					{
						ui8_FlagDispl = 0;
						stateInTasks_OLD = stateInTasks;
						leds_Off(display);
						
						if(stateInTasks == ONE_AFTER_ONE) {
							for(j = 0; j < MAX_LEDS; j++) {
								pixel_Set(display, col_strip, j);
								pixel_ToggleStatus(display, j);
							}
							i = 0;
						}
						
						if(stateInTasks == STATIC_FADER) {
							for(j = 0; j < MAX_LEDS; j++) {
								col_strip.Red = BRIGHT_MAX;
								col_strip.Green = BRIGHT_MAX;
								col_strip.Blue = BRIGHT_MIN;
								
								pixel_Set(display, col_strip, j);
								pixel_ToggleStatus(display, j);
							}
						}
						
						
					}

					// =============================================================================>
					switch(stateInTasks) {	
						case COLOR_ALL :
							if(ui8_FlagDispl == 0) {
								leds_Show(display);
								ui8_FlagDispl = 1;
							}
							else {							
								ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN, &col_strip.Red, \
								ui8_PressBtn_2, STEP_COLOR, DOWN, BRIGHT_MAX);
								ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP, &col_strip.Red, \
								ui8_PressBtn_3, STEP_COLOR, UP, BRIGHT_MAX);
								
								ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN, &col_strip.Green, \
								ui8_PressBtn_4, STEP_COLOR, DOWN, BRIGHT_MID);
								ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP, &col_strip.Green, \
								ui8_PressBtn_5, STEP_COLOR, UP, BRIGHT_MAX);
								
								ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN, &col_strip.Blue, \
								ui8_PressBtn_6, STEP_COLOR, DOWN, BRIGHT_MAX);
								ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP, &col_strip.Blue, \
								ui8_PressBtn_7, STEP_COLOR, UP, BRIGHT_MAX);
								
								if( (col_strip.Red != Red_OLD) || (col_strip.Green != Green_OLD) \
										|| (col_strip.Blue != Blue_OLD) ) {
									for(j = 0; j < MAX_LEDS; j++) {
										pixel_Set(display, col_strip, j);
									}
									
									// Update the saving val. :
									Red_OLD = col_strip.Red;
									Green_OLD = col_strip.Green;
									Blue_OLD = col_strip.Blue;
									
									// Reset the Display : 
									ui8_FlagDispl = 0;
								}
							}
							break;
							
						// =============================================================================>
						case ONE_AFTER_ONE:
							if(ui8_FlagDispl == 0) {
								leds_Show(display);
								//leds_ResetStatus(display);
								ui8_FlagDispl = 1;
							}
							else {
								ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN, &faderSpeed, \
								ui8_PressBtn_2, 1, DOWN, 1);
								ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP, &faderSpeed, \
								ui8_PressBtn_3, 1, UP, 10);
								
								ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN, &sens, \
								ui8_PressBtn_4, 1, DOWN, 1);
								ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP, &sens, \
								ui8_PressBtn_5, 1, UP, 1);
								
								if(sens != sens_OLD) {
									sens_OLD = sens;
									leds_ResetStatus(display);
									
									if(!sens)
										i = 0;
									else
										i = MAX_LEDS - 1;
								}
								
								if(++shiftingDelay >= faderSpeed) {
									shiftingDelay = 0;
									
									pixel_ToggleStatus(display, i);
									//pixel_ToggleStatus(display, i - 1);
									
									if(!sens) {
										if(i++ >= MAX_LEDS)
											i = 0;
									}
									else {
										if(i-- >= MAX_LEDS)
											i = MAX_LEDS - 1;
									}
									ui8_FlagDispl = 0;
								}
							}
							break;
							
						// =============================================================================>
						case STATIC_FADER:
							if(ui8_FlagDispl == 0) {
								leds_Show(display);
								ui8_FlagDispl = 1;
							}
							else {
								ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN, &faderSpeed, \
								ui8_PressBtn_2, 1, DOWN, 1);
								ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP, &faderSpeed, \
								ui8_PressBtn_3, 1, UP, 10);
								
								if(++shiftingDelay >= faderSpeed) {
									shiftingDelay = 0;
									
									if((greenSens != 0) && (col_strip.Green < BRIGHT_MID)) {
										col_strip.Red++;
										col_strip.Green++;
										//col_strip.Blue++;
									}
									else
										greenSens = 0;
									if((greenSens == 0) && (col_strip.Green > BRIGHT_MAX)) {
										col_strip.Red--;
										col_strip.Green--;
										//col_strip.Blue--;
									}
									else
										greenSens = 1;
									
									for(j = 0; j < MAX_LEDS; j++) {
										pixel_Set(display, col_strip, j);
									}
									ui8_FlagDispl = 0;
								}
							}
							break;
						case AUDIO_REACT :
							if(ui8_FlagDispl == 0) {
								leds_Show(display);
								ui8_FlagDispl = 1;
							}
							else {
								//if(1 < ++tempo) {
								//	tempo = 0;
									//col_strip.Red = col_strip.Green = col_strip.Blue = map_uint8(byGV_ADC1, 50, 255, BRIGHT_MIN, BRIGHT_WHITE_MAX);
								
									//col_strip.Red = 0;
									col_strip.Green = 0;
									//col_strip.Blue = 0;
								
									col_strip.Red = col_strip.Blue = map_uint8(byGV_ADC1, 50, 255, BRIGHT_MIN, BRIGHT_MID);
									//col_strip.Blue = map_uint8(byGV_ADC1, 50, 255, BRIGHT_MIN, BRIGHT_MID / 2);
								
									//col_strip.Green = col_strip.Blue = map_uint8(byGV_ADC1, 50, 255, BRIGHT_MIN, BRIGHT_MID);
									
									//byGV_ADC1 = map_uint8(byGV_ADC1, 0, 255, 0, MAX_LEDS);
									
									if( (col_strip.Red != Red_OLD) || (col_strip.Green != Green_OLD) \
											|| (col_strip.Blue != Blue_OLD) ) {
										for(j = 0; j < MAX_LEDS; j++) {
											pixel_Set(display, col_strip, j);
										}
										
										// Update the saving val. :
										Red_OLD = col_strip.Red;
										Green_OLD = col_strip.Green;
										Blue_OLD = col_strip.Blue;
										
										// Reset the Display : 
										ui8_FlagDispl = 0;
									}
								//}
							}
							break;
							
						// =============================================================================>
						default :	/* Shut the matrix if mode goes out of bound. */
							if(ui8_FlagDispl == 0) {
								leds_Off(display);
								ui8_FlagDispl = 1;
							}
							break;
					}
					// Switch to .._WAIT :
					UpdateState(APP_STATE_WAIT);
					break;
				
				case APP_STATE_WAIT :	/* Nothing. */	break;
				
				default :	/* State of every undefined case, so not supposed to come here. */	break;
			}
		}
	}
}
//========================================================================
//===================================\Functions_Init./====================================>
//===================================================
//========================================================================
//====================================\PERIPH_init/=======================================>
//=================================================
void PERIPH_Init(void)
// Description	:	Initialize the peripheral functions, like Timers, Ext. Int., ADC, etc ...
// Input		: Nothing.
// Output		: Nothing.
{
	Timer0_init();
	
	ADC1_8bits_init();

//- Activate ALL Interruptions ------------------->
	EA = 1;
}

//========================================================================
//===================================\PRESS_Modifier/=====================================>
//=================================================
uint8 PRESS_Modifier(uint8 _PressInput, uint8* _ui8_Dest, uint8 _ui8_DeBounce, \
uint8 _ui8_Step, uint8 _ui8_Sens, uint16 _ui16_MaxLimit)
// Description	:	Just test Outputs like LEDs & Inputs like Switches and Press Buttons.
// Input		: Nothing.
// Output		: Nothing.
{	// Var. Dec. :
	uint8 ui8_Return;
	int16 i16_ValRec = *_ui8_Dest;
	
	if(_ui8_Sens != DOWN)
	{
		if(_PressInput == 0)
		{
			if(_ui8_DeBounce == 0)
			{
				i16_ValRec += _ui8_Step;
				if(i16_ValRec >= _ui16_MaxLimit)
				{
					*_ui8_Dest = _ui16_MaxLimit;
				}
				else
				{
					*_ui8_Dest = i16_ValRec;
				}
			}
			else
			{
			}
			ui8_Return = 1;
		}
		else
		{
			ui8_Return = 0;
		}
	}
	else
	{
		if(_PressInput == 0)
		{
			if(_ui8_DeBounce == 0)
			{
				i16_ValRec -= _ui8_Step;
				if(i16_ValRec <= 0)
				{
					*_ui8_Dest = 0;
				}
				else
				{
					*_ui8_Dest = i16_ValRec;
				}
			}
			else
			{
			}
			ui8_Return = 1;
		}
		else
		{
			ui8_Return = 0;
		}
	}
	return ui8_Return;
}

uint8 PRESS_ModifierLoop(uint8 _PressInput, uint8* _ui8_Dest, uint8 _ui8_DeBounce, \
uint8 _ui8_Step, uint8 _ui8_Sens, uint16 _ui16_MinLimit, uint16 _ui16_MaxLimit)
// Description	:	Just test Outputs like LEDs & Inputs like Switches and Press Buttons.
// Input		: Nothing.
// Output		: Nothing.
{	// Var. Dec. :
	uint8 ui8_Return;
	int16 i16_ValRec = *_ui8_Dest;
	
	if(_ui8_Sens != DOWN)
	{
		if(_PressInput == 0)
		{
			if(_ui8_DeBounce == 0)
			{
				i16_ValRec += _ui8_Step;
				if(i16_ValRec > _ui16_MaxLimit)
				{
					*_ui8_Dest = _ui16_MinLimit;
				}
				else
				{
					*_ui8_Dest = i16_ValRec;
				}
			}
			else
			{
			}
			ui8_Return = 1;
		}
		else
		{
			ui8_Return = 0;
		}
	}
	else
	{
		if(_PressInput == 0)
		{
			if(_ui8_DeBounce == 0)
			{
				i16_ValRec -= _ui8_Step;
				if(i16_ValRec < 0)
				{
					*_ui8_Dest = _ui16_MaxLimit;
				}
				else
				{
					*_ui8_Dest = i16_ValRec;
				}
			}
			else
			{
			}
			ui8_Return = 1;
		}
		else
		{
			ui8_Return = 0;
		}
	}
	return ui8_Return;
}