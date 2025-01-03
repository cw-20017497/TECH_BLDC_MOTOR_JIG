#include <stdio.h>
#include <string.h>

#include "prj_type.h"
#include "timer.h"
#include "process_sys_event.h"
#include "bldc_motor.h"
#include "process_display.h"
#include "hal_led.h"
#include "hal_key.h"
#include "buzzer.h"
#include "key.h"
#include "key_normal_handler.h"
#include "process_key.h"

#include "r_cg_timer.h"

typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_50ms_Handler( void );
static void Evt_100ms_Handler( void );
extern void Evt_Durability_Handler( void );

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                    NORMAL,                     FCT,        EOL */
    { TIMER_ID_1MS,                 Evt_1ms_Handler,            NULL,       NULL },
    { TIMER_ID_10MS,                Evt_10ms_Handler,           NULL,       NULL },
    { TIMER_ID_50MS,                Evt_50ms_Handler,           NULL,       NULL },
    { TIMER_ID_100MS,               Evt_100ms_Handler,          NULL,       NULL },
    { TIMER_ID_DURABILITY,          Evt_Durability_Handler,     NULL,       NULL },
};
#define	SZ_LIST		( sizeof( SysEventList ) / sizeof( SysEvent_T ) )

void ProcessEventHandler( void )
{
    U8 i;
    Action_T fun;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( IsExpiredTimer( SysEventList[ i ].timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( SysEventList[ i ].timerId );

            // NORMAL or EOL 
            //if( GetEolStatus() == TRUE )
            //{
            //    // Get Eol Test Mode Event Handler
            //    fun = SysEventList[ i ].pFun_EOL; 
            //}
            //else if( GetFctStatus() == TRUE )
            //{
            //    // Get Eol Test Mode Event Handler
            //    fun = SysEventList[ i ].pFun_FCT; 
            //}
            //else
            {
                // Get Normal Mode Event Handler
                fun = SysEventList[ i ].pFun;
            }

            // Call a event handl
            if( fun != NULL )
            {
                fun();
            }
        }
    }
}

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    HAL_ScanKey();
}

static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    ProcessBldcMotor();
}



static void Evt_50ms_Handler( void )
{
    StartTimer( TIMER_ID_50MS, 50);

    ContyUpDownVolt();
}

static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);

    ProcessDisplay();
    
}


void TimerIsrCallback(void)
{
    BuzControl();
}

void InitSystem(void)
{
    /* IO */
    InitBldcMotor();
    InitKey();
    InitBuzzer();
    InitKeyHandler();
    InitDisplay();
    HAL_InitLed();

    RegisterTimerISR( TimerIsrCallback  );

}

