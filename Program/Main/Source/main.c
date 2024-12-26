#include "hw.h"
#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "sound.h"
#include "process_sys_event.h"

// TIMER
// 00 : base timer ( 1ms )
// 01 : bldc motor hall sensor ( 800us )
// 02 : led matrix ( 1ms )
// 03 : buzzer ( no isr )
// 045 : bldc motor output pwm ( no isr )

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_50MS,  50);
    StartTimer( TIMER_ID_100MS, 100);
}


void main( void )
{
    Delay_MS( 1200 );

    R_WDT_Restart();

    InitSystem();
    InitStartTimers();
    EI();

    /* EEPROM */
    R_WDT_Restart();
    
    // OUTPUT 24V
    SMPS_12V = 0;

    Sound( SOUND_POWER_ON );
    while(1)
    {
        R_WDT_Restart();

        ProcessEventHandler();        
    }
}
