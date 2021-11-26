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
    enum MODE { COLOR_ALL = 1, BRANCH_IN, WHOLE_BRANCH, SEQUENCE, ONE_AFTER_ONE, STATIC_FADER, AUDIO_REACT };
    const uint8 TOT_MODES = 8 - 1;
    xdata uint8 stateInTasks = 0, stateInTasks_OLD = 0;
// By modes :
// COMMON :
    xdata color col_strip = {0, 0, 0};
    //xdata pixel* display = displayInit(MAX_LEDS);
// COLOR_ALL :
    xdata uint8 Red_OLD = 0, Green_OLD = 0, Blue_OLD = 0;
// ONE_AFTER_ONE :
    xdata posType i = 0;
    xdata unsigned char oneOnTwo = 0;
    xdata unsigned char sens = 0, sens_OLD = 0;
// BRANCH_IN && WHOLE_BRANCH
    //enum BRANCH { BR_0, BR_1=3, BR_2=6, BR_3=9, BR_4=12, BR_5=15, BR_6=18, BR_7=21, BR_8=24, BR_9=27, BR_10=30, BR_11=33 };
		/*xdata posType branchIn_Limits[37] = {  0, 32, 65, 91, \
																						 111,136,163, \
																						 188,214,237, \
																						 261,286,311, \
																						 338,363,383, \
																						 409,442,474, \
																						 499,525,548, \
																						 572,597,622, \
																						 647,670,688, \
																						 716,749,781, \
																						 813,846,874, \
																						 892,915,MAX_LEDS};*/
    enum BRANCH { BR_0, BR_1, BR_2, BR_3, BR_4, BR_5, BR_6, BR_7, BR_8, BR_9, BR_10, BR_11};
		xdata posType branchStarts[12] = {  0, 91,163,237,311,383, \
																			474,548,622,688,781,874};
		xdata posType branchEnds[12]   = { 91,163,237,311,383,474, \
																			548,622,688,781,874,MAX_LEDS};
		xdata uint8 branch_i = 0;
    const uint8 BRANCH_DEFINED = 11;
// SEQUENCE
		xdata uint8 sequence[12] = {BR_0,BR_1,BR_8,BR_9,BR_2,BR_3, \
																BR_10,BR_11,BR_4,BR_5,BR_6,BR_7};
		xdata uint8 seqIndex = 0;
    const uint8 END_OF_SEQUENCE = 11;
// STATIC_FADER : 
    xdata uint8 shiftStatus = 0;
    xdata uint8 shiftingDelay = 0;
    xdata uint8 greenSens = 0;
    xdata uint8 faderSpeed = 5;
// AUDIO_REACT :
    enum AUDIO_THEME { THEME_R = 0, THEME_G, THEME_B,
                       THEME_RG, THEME_RB,
                       THEME_GB, THEME_RGB };
    xdata unsigned char audioTheme = THEME_R;
    const uint8 TOT_THEMES = 7 - 1;
    //xdata unsigned char tempo = 0;

    // For loop :
    uint16 j = 0;

//-- COUNTERS         : ---------------------------->

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
                    //stateInTasks = COLOR_ALL;
                    //stateInTasks = STATIC_FADER;
                    stateInTasks = AUDIO_REACT;

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

                        if (stateInTasks == ONE_AFTER_ONE) {
                            for (j = 0; j < MAX_LEDS; j++) {
                                pixel_Set(display, col_strip, j);
                                pixel_ToggleStatus(display, j);
                            }
                            i = 0;
                        }

                        if (stateInTasks == STATIC_FADER) {
                            for (j = 0; j < MAX_LEDS; j++) {
                                col_strip.Red = BRIGHT_MID;
                                col_strip.Green = BRIGHT_MID;
                                col_strip.Blue = BRIGHT_MIN;

                                pixel_Set(display, col_strip, j);
                                pixel_ToggleStatus(display, j);
                            }
                            greenSens = UP;
                        }
                    }

                    switch (stateInTasks) {
                        //==============================================================>
                        case COLOR_ALL :
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,
                                                                &col_strip.Red, ui8_PressBtn_2, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,
                                                                &col_strip.Red, ui8_PressBtn_3, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,
                                                    &col_strip.Green, ui8_PressBtn_4, \
                                                        STEP_COLOR, DOWN, BRIGHT_MID);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,
                                                    &col_strip.Green, ui8_PressBtn_5, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                ui8_PressBtn_6 = PRESS_Modifier(PRESS_BLU_DN,
                                                    &col_strip.Blue, ui8_PressBtn_6, \
                                                        STEP_COLOR, DOWN, BRIGHT_MAX);
                                ui8_PressBtn_7 = PRESS_Modifier(PRESS_BLU_UP,
                                                    &col_strip.Blue, ui8_PressBtn_7, \
                                                        STEP_COLOR, UP, BRIGHT_MAX);

                                if ( (col_strip.Red != Red_OLD) ||
                                        (col_strip.Green != Green_OLD) ||
                                            (col_strip.Blue != Blue_OLD) ) {
                                    for (j = 0; j < MAX_LEDS; j++)
                                        pixel_Set(display, col_strip, j);

                                    Red_OLD = col_strip.Red;
                                    Green_OLD = col_strip.Green;
                                    Blue_OLD = col_strip.Blue;

                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case ONE_AFTER_ONE:
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                //leds_ResetStatus(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,       \
                                                    &faderSpeed, ui8_PressBtn_2, 1, \
                                                        DOWN, 1);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,       \
                                                    &faderSpeed, ui8_PressBtn_3, 1, \
                                                        UP, 10);

                                ui8_PressBtn_4 = PRESS_Modifier(PRESS_GRN_DN,       \
                                                    &sens, ui8_PressBtn_4, 1, DOWN, 1);
                                ui8_PressBtn_5 = PRESS_Modifier(PRESS_GRN_UP,       \
                                                    &sens, ui8_PressBtn_5, 1, UP, 1);

                                if (sens != sens_OLD) {
                                    sens_OLD = sens;
                                    leds_ResetStatus(display);

                                    i = ( sens ) ? (MAX_LEDS - 1) : (0);
                                }

                                if (++shiftingDelay >= faderSpeed) {
                                    shiftingDelay = 0;

                                    pixel_ToggleStatus(display, i);
                                    //pixel_ToggleStatus(display, i - 1);

                                    if (!sens) {
                                        if (i++ >= MAX_LEDS)
                                            i = 0;
                                    } else {
                                        if (i-- >= MAX_LEDS)
                                            i = MAX_LEDS - 1;
                                    }
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
												case WHOLE_BRANCH:
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,       \
                                                    &faderSpeed, ui8_PressBtn_2, 5, \
                                                        DOWN, 200);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,       \
                                                    &faderSpeed, ui8_PressBtn_3, 5, \
                                                        UP, 200);

                                if (++shiftingDelay >= faderSpeed) {
																		shiftingDelay = 0;
																	
																		leds_ResetStatus(display);
																		
																		while(branch_i % 3)
																			++branch_i;
																		
																		leds_ChainedLeds(display, col_strip, branchStarts[branch_i], branchEnds[branch_i]);
																		if(branch_i >= BRANCH_DEFINED) 
																			branch_i = 0;
																		
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
												case SEQUENCE:
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,       \
                                                    &faderSpeed, ui8_PressBtn_2, 5, \
                                                        DOWN, 200);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,       \
                                                    &faderSpeed, ui8_PressBtn_3, 5, \
                                                        UP, 200);

                                if (++shiftingDelay >= faderSpeed) {
																		shiftingDelay = 0;
																	
																		leds_ResetStatus(display);
																		
																		leds_ChainedLeds(display, col_strip, 								\
																									branchStarts[sequence[seqIndex]],	\
																									branchEnds[sequence[seqIndex]+3]);
																	
																		if(++seqIndex > END_OF_SEQUENCE) 
																			seqIndex = 0;
																		
                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case STATIC_FADER:
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,       \
                                                    &faderSpeed, ui8_PressBtn_2, 1, \
                                                        DOWN, 1);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,       \
                                                    &faderSpeed, ui8_PressBtn_3, 1, \
                                                        UP, 10);

                                if (++shiftingDelay >= faderSpeed) {
                                    shiftingDelay = 0;

                                    if ( (greenSens == UP) &&
                                            (col_strip.Green < BRIGHT_MID) ) {
                                        col_strip.Red++;
                                        col_strip.Green++;
                                    } else
                                        greenSens = DOWN;

                                    if ( (greenSens == DOWN) &&
                                            (col_strip.Green > BRIGHT_MIN) ) {
                                        col_strip.Red--;
                                        col_strip.Green--;
                                    } else
                                        greenSens = UP;

                                    for (j = 0; j < MAX_LEDS; j++)
                                        pixel_Set(display, col_strip, j);

                                    flagDisplayed = FALSE;
                                }
                            }
                            break;

                        //==============================================================>
                        case AUDIO_REACT :
                            if (flagDisplayed == FALSE) {
                                leds_Show(display);
                                flagDisplayed = TRUE;
                            } else {
                                ui8_PressBtn_2 = PRESS_Modifier(PRESS_RED_DN,           \
                                                        &audioTheme, ui8_PressBtn_2,    \
                                                        1, DOWN, TOT_THEMES);
                                ui8_PressBtn_3 = PRESS_Modifier(PRESS_RED_UP,           \
                                                        &audioTheme, ui8_PressBtn_3,    \
                                                        1, UP, TOT_THEMES);

                                //if(1 < ++tempo) {
                                //	tempo = 0;
                                //byGV_ADC1 = map_uint8(byGV_ADC1, 0, 255, 0, MAX_LEDS);

                                switch (audioTheme) {
                                    case THEME_R:
                                        col_strip.Red = map_uint8(byGV_ADC1, 50, 255,   \
                                                                BRIGHT_MIN, BRIGHT_MID);
                                        col_strip.Green = 0;
                                        col_strip.Blue = 0;
                                        break;
                                    case THEME_G:
                                        col_strip.Green = map_uint8(byGV_ADC1, 50, 255, \
                                                                BRIGHT_MIN, BRIGHT_MID);
                                        col_strip.Red = 0;
                                        col_strip.Blue = 0;
                                        break;
                                    case THEME_B:
                                        col_strip.Blue = map_uint8(byGV_ADC1, 50, 255,  \
                                                                BRIGHT_MIN, BRIGHT_MID);
                                        col_strip.Red = 0;
                                        col_strip.Green = 0;
                                        break;

                                    case THEME_RG:
                                        col_strip.Red = col_strip.Green =               \
                                                                map_uint8(byGV_ADC1, 50,\
                                                                    255, BRIGHT_MIN,    \
                                                                        BRIGHT_MID);
                                        col_strip.Blue = 0;
                                        break;
                                    case THEME_RB:
                                        col_strip.Red = col_strip.Blue =                \
                                                                map_uint8(byGV_ADC1, 50,\
                                                                    255, BRIGHT_MIN,    \
                                                                        BRIGHT_MID);
                                        col_strip.Green = 0;
                                        break;

                                    case THEME_GB:
                                        col_strip.Green = col_strip.Blue =              \
                                                                map_uint8(byGV_ADC1, 50,\
                                                                    255, BRIGHT_MIN,    \
                                                                        BRIGHT_MID);
                                        col_strip.Red = 0;
                                        break;

                                    case THEME_RGB:
                                        col_strip.Red = col_strip.Green =               \
                                            col_strip.Blue = map_uint8(byGV_ADC1, 50,   \
                                                                        255, BRIGHT_MIN,\
                                                                            BRIGHT_MID);
                                        break;
                                }

                                if ( (col_strip.Red != Red_OLD) ||          \
                                        (col_strip.Green != Green_OLD) ||   \
                                            (col_strip.Blue != Blue_OLD) ) {
                                    for (j = 0; j < MAX_LEDS; j++)
                                        pixel_Set(display, col_strip, j);

                                    Red_OLD = col_strip.Red;
                                    Green_OLD = col_strip.Green;
                                    Blue_OLD = col_strip.Blue;

                                    flagDisplayed = FALSE;
                                }
                                //}
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