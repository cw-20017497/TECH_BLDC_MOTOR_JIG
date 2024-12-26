#ifndef __HAL_LED_H__
#define __HAL_LED_H__


#define DEFAULT_ONOFF_DUTY          90U
#define DEFAULT_DIMMING_DUTY        30U
#define DEFAULT_ONOFF_DUTY_DARK     50U
#define DEFAULT_DIMMING_DUTY_DARK   20U


// PIN2PIN leds count = 35
typedef enum
{
   // MATRIX LED
    SEG_1_A,
    SEG_1_F,    // digit 1
    SEG_1_G,
    SEG_1_E,
    SEG_1_D,
    SEG_1_C,
    SEG_1_B,

    SEG_2_A,
    SEG_2_F,    // digit 2
    SEG_2_G,
    SEG_2_E,
    SEG_2_D,
    SEG_2_C,
    SEG_2_B,

    SEG_3_A,
    SEG_3_F,    // digit 3
    SEG_3_G,
    SEG_3_E,
    SEG_3_D,
    SEG_3_C,
    SEG_3_B,

    SEG_22,

    LED_ALL,

    MAX_LED_NUM
} LedId_T;
#define MAX_LED    6       /* 5bytes * 8bits = 40 */

void HAL_InitLed(void);
void HAL_TurnOnOffLEDAll(U8 mu8OnOff);
void HAL_DimmingLedAll(U8 mu8OnOff);

void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);

void HAL_SetOnOffLED(U8 *pLeds, U8 mu8Size );
void HAL_SetOnOffDuty(U8 mu8Duty);

void HAL_SetDimmingLED(U8 *pLeds, U8 mu8Size);
void HAL_SetDimmingDuty(U8 mu8Duty);

#endif /* __HAL_LED_H__ */
