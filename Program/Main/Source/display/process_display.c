#include "hw.h"
#include "display.h"
#include "display_lib.h"
#include "process_display.h"

#include "front.h"
#include "hal_key.h"
#include "hal_led.h"
#include "sound.h"
#include "key.h"
#include "key_normal_handler.h"
#include "bldc_motor.h"




static void UpdateTimer(void);
static void ProcessDisplayBoot(void);
static void ProcessDisplayNormalMode(void);


typedef struct _display_
{
    U8 Init;            /* FALSE : 초기화 전, TRUE : 초기화 완료 */
    U8 Version;         /* 버전 표시 FLAG */
    U8 VersionStep;

} Display_T;

Display_T   Disp;


void InitDisplay(void)
{
    // TIMER 
    InitDispTimer();

    /* Booting Display - Showing Blink Front Display */
    Disp.Init = FALSE;

    /* TURN OFF ALL LED */
    TurnOffAllLED();
}


U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;

    InitDispTimer();
}

typedef struct _disp_id_time_
{
    U8 id;
    U16 mTime;
} DispTimerVal_T;

DispTimerVal_T DispTimerValTable[] = 
{
    { DISP_TIMER_CLOCK,               TIMER_20SEC },  
    { DISP_TIMER_ERROR,               TIMER_3SEC },

    { DISP_TIMER_CONFIG_AMOUNT,       TIMER_2SEC },     
    { DISP_TIMER_ALARM_SETTING,       TIMER_2SEC },
    { DISP_TIMER_ALARM_LOCK_ALL,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_LOCK_HOT,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_LOCK_ICE,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_MAKE_OFF_ICE,  TIMER_2SEC },      
    { DISP_TIMER_ALARM_MAKE_OFF_COLD, TIMER_2SEC },      

    { DISP_TIMER_WIFI_AP_ERROR,       TIMER_5SEC },

    { DISP_TIMER_UP,                  TIMER_1SEC },
    { DISP_TIMER_DOWN,                TIMER_1SEC },
    { DISP_TIMER_NEXT,                TIMER_1SEC },

    { DISP_TIMER_CUSTOMER_MENU,       TIMER_6SEC },
    { DISP_TIMER_SOUND_MENU,          TIMER_6SEC },

    { DISP_TIMER_KEY_ICE,             TIMER_7SEC },  
    { DISP_TIMER_KEY_AMOUNT,          TIMER_7SEC },
    { DISP_TIMER_KEY_HOT,             TIMER_7SEC },
    { DISP_TIMER_KEY_HOT_TEMP,        TIMER_7SEC },

    { DISP_TIMER_SETTING_MENU,        TIMER_20SEC },
    { DISP_TIMER_CLOCK_MENU,          TIMER_20SEC },
    { DISP_TIMER_SLEEP,               TIMER_60MIN },

    { DISP_TIMER_ON,                  TIMER_2SEC },
    { DISP_TIMER_OFF,                 TIMER_2SEC },
    { DISP_TIMER_SETTING_CLOCK,       TIMER_2SEC }
};
#define SZ_DISP_TIMER_TABLE (sizeof(DispTimerValTable)/sizeof(DispTimerVal_T))

static U16 FindTimeVal(U8 id)
{
    U8 i;

    for( i = 0; i < SZ_DISP_TIMER_TABLE; i++ )
    {
        if( id == DispTimerValTable[ i ].id )
        {
            return DispTimerValTable[ i ].mTime;
        }
    }

    return 0;   // timer 0...
}

// Display Timer start/stop
void StartDispTimerId(U8 id)
{
    U16 mTime;

    mTime = FindTimeVal( id );
    SetDispTimer( id, mTime );
}

void StopDispTimerId(U8 id)
{
    SetDispTimer( id, 0);
}

static void ProcessDisplayBoot(void)
{
    static U8 mu8Count = 5;
    static U8 mu8Toggle = 0;

    DispLedDuty();

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        mu8Toggle = !mu8Toggle;

        mu8Count--;
        if( mu8Count == 0 )
        {
            TurnOffAllLED();

            Disp.Init = TRUE;
            mu8Count = 5;
            return;
        }
    }

    if( mu8Toggle == 0 )
    {
        TurnOffAllLED();
    }
    else
    {
        TurnOnAllLED();
    }
}






extern F32 bldc_volt;
static void DispVolt(void)
{
    U16 volt;
    volt = (U16)( bldc_volt * 10.0f );
    DispWaterOutAmount( volt );
}

extern U8 set_blink;
static void ProcessDisplayNormalMode(void)
{
    static blink_cnt = 5;

    DispLedDuty();

    if( set_blink == FALSE )
    {
        blink_cnt = 5;
        DispVolt();
    }
    else
    {
        blink_cnt--;
        if( blink_cnt != 0 )
        {
            TurnOffAllLED();
        }
        else
        {
            blink_cnt = 5;
            set_blink = FALSE;
        }
    }
}

void ProcessDisplay(void)
{
    UpdateDisplayTimer();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        return ;
    }

    DispLedDuty();

    /* NORMAL MODE */
    ProcessDisplayNormalMode();
}
