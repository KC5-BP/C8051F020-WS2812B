/*..-----------------------------------------------------------------------------------.
../ .---------------------------------------------------------------------------------. \
./´/
|x| __--""'\
|x|  ,__  -'""`;
|x| /   \  /"'  \
|x|   __// \-"-_/
|x| ´"   \  |           > Title : 9LedsTest
|x| \     |  \  _.-"',
|x| "^,-´\/\  '" ,--. \         > Src : None
|x|  \|\| | | , /    | |
|x|     '`'\|._ |   / /
|x|         '\),/  / |          > Creation : YYYY.MM.DD
|x|           |/.-"_/           > By :  KC5-BP
|x| .__---+-_/'|--"
|x|         _| |_--,            > Description :
|x|        ',/ |   /                >   Testing WS2812b communication
|x|        /|| |  /                 >   with Silabs C8051F020.
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
// .. definition folder SFR (Like Port Definition "P5", "P6", "TR0", etc...).
#include <c8051f020.h>

#include "base_sfr.h"
#include "peripherals/time.h"
#include "peripherals/ad_da.h"
#include "leds/ws2812_led_manip.h"
#include "leds/ws2812_mx_manip.h"

// HELPS : 
// https://stackoverflow.com/questions/13706809/structs-in-c-with-initial-values
// Board can only be of 128bytes max. :
/* https://www.google.com/search?client=firefox-b-d&q=keil+error+c249+%27data%27+segment
+too+large */

//===================================================
//=================================\Global_Definitions/=================================>
#define PRESS_MODE_DN (BYI_PRESS & BIT0)
#define PRESS_MODE_UP (BYI_PRESS & BIT1)

#define PRESS_RED_DN (BYI_PRESS & BIT2)
#define PRESS_RED_UP (BYI_PRESS & BIT3)

#define PRESS_GRN_DN (BYI_PRESS & BIT4)
#define PRESS_GRN_UP (BYI_PRESS & BIT5)

#define PRESS_BLU_DN (BYI_PRESS & BIT6)
#define PRESS_BLU_UP (BYI_PRESS & BIT7)

#define STEP_COLOR 0x18

//===================================================
//==========================\Function_Prototypes_Declaration/===========================>
//======================================================================================>
/* Description  :   Initialized the Peripherals (Timers, Ext. Int., ADC).              */
void PERIPH_Init(void);
//======================================================================================>
/* Description  :   In-/De-crease values depending of the button pressed and avoid
                    repetition if button not released.                                 */
uint8 PRESS_Modifier(uint8 _pressInput, uint8* _dest, uint8 _oldDebounceState, \
                        uint8 _step, uint8 _sens, uint16 _maxLimit);
//======================================================================================>
/* Description  :   Same as before, but instead of blocking at _maxLimit val.,
                    it loops to the _minLimit.                                         */
uint8 PRESS_ModifierLoop(uint8 _pressInput, uint8* _dest, uint8 _oldDeBounceState,  \
                            uint8 _step, uint8 _sens, uint16 _minLimit,             \
                                uint16 _maxLimit);
//======================================================================================>
/* Description  :   Same as before, but instead of blocking at _maxLimit val.,
                    it loops to the _minLimit.                                         */
#define TOTAL_SECONDS_IN_A_DAY 86400
typedef struct {
	unsigned char h_decade;
	unsigned char h_unit;
	unsigned char m_decade;
	unsigned char m_unit;
	unsigned char s_decade;
	unsigned char s_unit;
} hourFormat;

hourFormat secToTimeFormat(unsigned long sec) {
	hourFormat tmp = {0,0,0};
	unsigned char h = sec / 3600;
	unsigned char m = (sec % 3600) / 60;
	unsigned char s = sec % 60;

	tmp.h_decade = h / 10;
	tmp.h_unit = h % 10;

	tmp.m_decade = m / 10;
	tmp.m_unit = m % 10;

	tmp.s_decade = s / 10;
	tmp.s_unit = s % 10;

	return tmp;
}

//===================================================
//============================\Something_like_overloaded_fn/============================>
#define MAP_FN(type)                                                            \
type map_##type(type x, type in_min, type in_max, type out_min, type out_max) { \
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);  \
}

MAP_FN(uint8)

//========================================================================
//==================================\Global_Variables/==================================>


//========================================================================
//=====================================\main_Code/======================================>
int main(void) {
//-- LOCAL VARIABLES  : ---------------------------->
    enum MODE { CFG_COL_TXT = 1, SHIFTING_TEXT, CFG_COL_CLK, CLOCK_DIGITAL};
    const uint8 TOT_MODES = 5 - 1;
    xdata uint8 stateInTasks = 0, stateInTasks_OLD = 0;
// By modes :
// COMMON :
    xdata color colText = {0, 0, 0};
    xdata color colClock = {0, 0, 0};
    //xdata pixel* display = displayInit(MAX_LEDS);
// CFG_COL_TXT :
    xdata uint8 redTxt_OLD = 0, greenTxt_OLD = 0, blueTxt_OLD = 0;
// SHIFTING_TEXT :
    xdata char xPos = 25, xPos_OLD = 0;
    xdata char yPos = 0, yPos_OLD = 0;
    xdata uint8 speed = 5;
// CFG_COL_CLK :
    xdata uint8 redClk_OLD = 0, greenClk_OLD = 0, blueClk_OLD = 0;
// CLOK_DIGITAL :
		xdata unsigned long timeSinceDayStart = 80400;
		xdata unsigned long prevSec = timeSinceDayStart;
		xdata uint8 secCntr = 0;
		hourFormat timeSplit = {0,0,0};
		//char* str = "00\0";
		char str[3] = "00\0";
		xdata uint8 font_OLD = formatDisplay.font;
    // For loop :
    uint16 j = 0;

//-- COUNTERS         : ---------------------------->
    uint16 cnt = 0;

//-- FLAGS            : ---------------------------->
    xdata uint8 flagDisplayed = FALSE;

//-- DEBOUNCING FLAGS : ---------------------------->
    xdata uint8 ui8_PressBtn_0 = 0, ui8_PressBtn_1 = 0, \
                ui8_PressBtn_2 = 0, ui8_PressBtn_3 = 0, \
                ui8_PressBtn_4 = 0, ui8_PressBtn_5 = 0, \
                ui8_PressBtn_6 = 0, ui8_PressBtn_7 = 0;

/*--------------------."'-._@\|':.(o'\____CODE_____/'o).:'|/@_.-'".--------------------*/
    SYS_Init(); // SYSCLK | WATCHDOG | PORTS

    if ( display ) {
        while (TRUE) {
            switch (StateOfTheMachine) {
                //======================================================================>
                case APP_STATE_INIT :
                    PERIPH_Init();

                    BYO_LEDS = BYO_OUT_OFF;
                    leds_Off(display);

                    // Starting mode :
                    stateInTasks = CFG_COL_TXT;
                    //stateInTasks = CFG_COL_CLK;

                    enableTimers();
                    updateAppState(APP_STATE_WAIT);
                    break;

                //======================================================================>
                case APP_STATE_SERVICE_TASKS : // Look into the ISR of T0 in << time.c >>
                    ui8_PressBtn_0 = PRESS_Modifier(PRESS_MODE_DN, &stateInTasks,
                                                    ui8_PressBtn_0, 1, DOWN, TOT_MODES);
                    ui8_PressBtn_1 = PRESS_Modifier(PRESS_MODE_UP, &stateInTasks,
                                                    ui8_PressBtn_1, 1, UP, TOT_MODES);

                    if (stateInTasks != stateInTasks_OLD) {
                        flagDisplayed = FALSE;
                        stateInTasks_OLD = stateInTasks;
                        leds_Off(display);
                    }
										
										if(++secCntr >= 99) {
												secCntr = 0;
												if(++timeSinceDayStart >= TOTAL_SECONDS_IN_A_DAY)
														timeSinceDayStart = 0;
													
												if(stateInTasks == CLOCK_DIGITAL)
														flagDisplayed = FALSE;
										}

                    switch (stateInTasks) {
                        //==============================================================>
                        case CFG_COL_TXT :
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,
                                                                &colText.Red, ui8_PressBtn_2, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,
                                                                &colText.Red, ui8_PressBtn_3, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,
                                                    &colText.Green, ui8_PressBtn_4, \
                                                        STEP_COLOR, DOWN, BRIGHT_MID);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,
                                                    &colText.Green, ui8_PressBtn_5, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN,
                                                    &colText.Blue, ui8_PressBtn_6, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP,
                                                    &colText.Blue, ui8_PressBtn_7, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);
																
                                if ( (colText.Red != redTxt_OLD) ||
                                        (colText.Green != greenTxt_OLD) ||
                                            (colText.Blue != blueTxt_OLD) ) {
                                    for (j = 0; j < MAX_LEDS; j++)
                                        pixel_Set(display, colText, j);

                                    redTxt_OLD = colText.Red;
                                    greenTxt_OLD = colText.Green;
                                    blueTxt_OLD = colText.Blue;
																							
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case SHIFTING_TEXT :
                            if (flagDisplayed == FALSE) {
                                leds_ResetStatus(display); // And not led_Off() like before ..
															
																matrix_SetTextPosition(xPos, yPos);
																matrix_Print(display, colText, "HELLO WOLRD!");
																/*matrix_SetTextPosition(xPos, 8);
																matrix_Print(display, colText, "J\'TE POUF");
																matrix_SetTextPosition(xPos+40, 0);
																matrix_Print(display, colText, "BIBOU^");*/
                                leds_Show(display);
															
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,
                                                                &speed, ui8_PressBtn_2, \
                                                        5, DOWN, 200);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,
                                                                &speed, ui8_PressBtn_3, \
                                                        5, UP, 200);

                                ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,
                                                    &yPos, ui8_PressBtn_4, \
                                                        1, DOWN, MAX_LINE);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,
                                                    &yPos, ui8_PressBtn_5, \
                                                        1, UP, MAX_LINE);

                               /* ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN,
                                                    &colText.Blue, ui8_PressBtn_6, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP,
                                                    &colText.Blue, ui8_PressBtn_7, \
                                                        STEP_COLOR, UP, BRIGHT_MAX); */
															
																if(++cnt >= speed) {
																	cnt = 0;
																	xPos = (--xPos < -100) ? (xPos = 25) : (xPos);
																}
																
                                if (xPos != xPos_OLD) {																		
																		xPos_OLD = xPos; 
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case CFG_COL_CLK :
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,
                                                                &colClock.Red, ui8_PressBtn_2, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,
                                                                &colClock.Red, ui8_PressBtn_3, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,
                                                    &colClock.Green, ui8_PressBtn_4, \
                                                        STEP_COLOR, DOWN, BRIGHT_MID);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,
                                                    &colClock.Green, ui8_PressBtn_5, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN,
                                                    &colClock.Blue, ui8_PressBtn_6, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP,
                                                    &colClock.Blue, ui8_PressBtn_7, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);
																
                                if ( (colClock.Red != redClk_OLD) ||
                                        (colClock.Green != greenClk_OLD) ||
                                            (colClock.Blue != blueClk_OLD) ) {
                                    for (j = 0; j < MAX_LEDS; j++)
                                        pixel_Set(display, colClock, j);

                                    redClk_OLD = colClock.Red;
                                    greenClk_OLD = colClock.Green;
                                    blueClk_OLD = colClock.Blue;
																							
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case CLOCK_DIGITAL :
                            if (flagDisplayed == FALSE) {
                                leds_ResetStatus(display); // And not led_Off() like before ..
															
																timeSplit = secToTimeFormat(timeSinceDayStart);
															
																matrix_SetTextPosition(2, 8);
																//matrix_Print(display, colClock, (char*)('0'+timeSplit.h_decade));
																//str[0] = (char*)('0'+timeSplit.h_decade);
																str[0] = '0'+timeSplit.h_decade;
																str[1] = '0'+timeSplit.h_unit;
																//str[0] = '0'+timeSplit.m_decade;
																//str[1] = '0'+timeSplit.m_unit;
																matrix_Print(display, colClock, str);
															
																matrix_SetTextPosition(5, 0);
																str[0] = '0'+timeSplit.m_decade;
																str[1] = '0'+timeSplit.m_unit;
																//str[0] = '0'+timeSplit.s_decade;
																//str[1] = '0'+timeSplit.s_unit;
																matrix_Print(display, colClock, str);
															
                                leds_Show(display);
															
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,
                                                                &formatDisplay.font, ui8_PressBtn_2, \
                                                        1, DOWN, 2);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,
                                                                &formatDisplay.font, ui8_PressBtn_3, \
                                                        1, UP, 2);

                                /*ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,
                                                    &yPos, ui8_PressBtn_4, \
                                                        1, DOWN, MAX_LINE);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,
                                                    &yPos, ui8_PressBtn_5, \
                                                        1, UP, MAX_LINE);*/

                               /* ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN,
                                                    &colText.Blue, ui8_PressBtn_6, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP,
                                                    &colText.Blue, ui8_PressBtn_7, \
                                                        STEP_COLOR, UP, BRIGHT_MAX); */
																/*if(++secCntr >= 99) {
																		secCntr = 0;
																		if(++timeSinceDayStart >= TOTAL_SECONDS_IN_A_DAY) {
																				timeSinceDayStart = 0;
																		}
                                    flagDisplayed = FALSE;
																}*/
																
                                if (formatDisplay.font != font_OLD) {
																		font_OLD = formatDisplay.font; 
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        default :    /* Shut the matrix if mode goes out of bound. */
                            if (flagDisplayed == FALSE) {
                                leds_Off(display);
                                flagDisplayed = TRUE;
                            }
                            break;
                    }
                    updateAppState(APP_STATE_WAIT);
                    break;

                //======================================================================>
                case APP_STATE_WAIT :    /* Nothing. */    break;

                //======================================================================>
                default :   break;
            }
        }
    }
    return 0;
}

//========================================================================
//==================================\Functions_Init./===================================>
//======================================================================================>
void PERIPH_Init(void) {
    Timer0_init();

    ADC1_8bits_init();

//- Activate ALL Interruptions ------------------->
    EA = 1;
}
//======================================================================================>
uint8 PRESS_Modifier(uint8 _pressInput, uint8* _dest, uint8 _oldDebounceState, \
                        uint8 _step, uint8 _sens, uint16 _maxLimit) {
    uint8 ui8_Return;
    int16 i16_ValRec = *_dest;

    if (_sens != DOWN) {
        if (_pressInput == 0) {
            if (_oldDebounceState == 0) {
                i16_ValRec += _step;
                if (i16_ValRec >= _maxLimit) {
                    *_dest = _maxLimit;
                } else {
                    *_dest = i16_ValRec;
                }
            } else {
            }
            ui8_Return = 1;
        } else {
            ui8_Return = 0;
        }
    } else {
        if (_pressInput == 0) {
            if (_oldDebounceState == 0) {
                i16_ValRec -= _step;
                if (i16_ValRec <= 0) {
                    *_dest = 0;
                } else {
                    *_dest = i16_ValRec;
                }
            } else {
            }
            ui8_Return = 1;
        } else {
            ui8_Return = 0;
        }
    }
    return ui8_Return;
}
//======================================================================================>
uint8 PRESS_ModifierLoop(uint8 _pressInput, uint8* _dest, uint8 _oldDebounceState,  \
                            uint8 _step, uint8 _sens, uint16 _minLimit,             \
                                uint16 _maxLimit) {
    uint8 ui8_Return;
    int16 i16_ValRec = *_dest;

    if (_sens != DOWN) {
        if (_pressInput == 0) {
            if (_oldDebounceState == 0) {
                i16_ValRec += _step;
                if (i16_ValRec > _maxLimit) {
                    *_dest = _minLimit;
                } else {
                    *_dest = i16_ValRec;
                }
            } else {
            }
            ui8_Return = 1;
        } else {
            ui8_Return = 0;
        }
    } else {
        if (_pressInput == 0) {
            if (_oldDebounceState == 0) {
                i16_ValRec -= _step;
                if (i16_ValRec < 0) {
                    *_dest = _maxLimit;
                } else {
                    *_dest = i16_ValRec;
                }
            } else {
            }
            ui8_Return = 1;
        } else {
            ui8_Return = 0;
        }
    }
    return ui8_Return;
}