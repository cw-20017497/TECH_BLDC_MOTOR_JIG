#include "hw.h"
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "key_event_lib.h"
#include "front.h"
//#include "hot_water.h"
//#include "cold_water.h"
//#include "water_out.h"
//#include "power_saving.h"
#include "sound.h"
//#include "eeprom.h"
//#include "error.h"
#include "process_display.h"
//#include "eol.h"
//#include "fct.h"
//#include "time_short.h"
#include "timer.h"
#include "timer_id.h"
//#include "flush_water_out.h"
//#include "hal_step_motor.h"
//#include "moving_faucet.h"
#include "bldc_motor.h"


static U8 SelSound(void);

static U8 SelBldcUp(void);
static U8 SelBldcDown(void);
static U8 SelBldcDir(void);
static U8 SelBldcBreak(void);
static U8 SelBldcStop(void);
static U8 SelBldcOn(void);

// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,            2sec,           3sec,  5sec,  Pop,           TS */
    /* SINGLE KEY */
    { K_HOT,            SelBldcUp,       NULL,  NULL,  NULL,  KeyStopCont,    KeyContUp },
    { K_ROOM,           SelBldcDown,     NULL,  NULL,  NULL,  KeyStopCont,    KeyContDown },
    { K_COLD,           SelBldcDir,      NULL,  NULL,  NULL,  NULL,             NULL },
    { K_AMOUNT,         SelBldcBreak,    NULL,  NULL,  NULL,  NULL,             NULL },
    { K_ON,             NULL,  NULL,  NULL,  NULL,  NULL,          SelBldcOn },
    { K_OFF,            NULL,  NULL,  NULL,  NULL,  NULL,          SelBldcStop },

    /* KEY,            Short,            2sec,           3sec,  5sec,  Pop,           TS */
    ///* SINGLE KEY */
};

U8 IsValidNormalKeyCondition(U32 mu32Key)
{
    /* 전체 잠금 상태 */
    //if( GetLockAll() == LOCK )
    //{
    //    if( mu32Key != K_LOCK_ALL )
    //    {
    //        return FALSE;
    //    }
    //}

    //if( GetFaucetStatus() == MOVING_FAUCET_INITING 
    //        || GetFaucetStatus() == MOVING_FAUCET_GOING_READY
    //        || GetFaucetStatus() == MOVING_FAUCET_GOING_MOVE )
    //{
    //    return FALSE;   // 무빙 파우셋 버튼 입력 금지 조건
    //}

    //if( GetWaterOut() == TRUE )
    //{
    //    return FALSE;   // 일반 추출이면 FALSE
    //}

    return TRUE;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
void* GetNormalKeyEventList(void)
{
    //if( GetMementoDisp() == TRUE )
    //{
    //    return KeyEventMementoList;
    //}
    //else if( GetTimeShortStatus() == TRUE )
    //{
    //    return KeyEventTimeShortList;
    //}

    return KeyEventList;
}

// 모드에 따라 메멘토 타임쇼트, 일반 3가지를 반환
U8 GetNormalKeyEventListSize(void)
{
    //if( GetMementoDisp() == TRUE )
    //{
    //    return ( sizeof(KeyEventMementoList) / sizeof(KeyEventList_T) );
    //}
    //else if( GetTimeShortStatus() == TRUE )
    //{
    //    return ( sizeof(KeyEventTimeShortList) / sizeof(KeyEventList_T) );
    //}

    return ( sizeof(KeyEventList) / sizeof(KeyEventList_T) );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static U8 SelSound(void)
{
    return SOUND_SELECT;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void InitKeyHandler(void)
{
    InitContKey( DEFAULT_CONT_TICK );
}


void ContyUpDownVolt(void)
{
    ControlContKey( SelBldcDown, SelBldcUp );
}

U8 bldc_dir = MOTOR_DIR_CW;
U8 bldc_break = FALSE;
F32 bldc_volt = 0.0f;
static U8 SelBldcUp(void)
{
    bldc_break = FALSE;
    BreakBldcMotor( bldc_break );

    if( bldc_volt < 3.0f )
    {
        bldc_volt = 3.0f;
    }
    else
    {
        bldc_volt = bldc_volt + 0.1f;
    }

    if( bldc_volt >= MAX_BLDC_OUTPUT_VOLT )
    {
        bldc_volt = MAX_BLDC_OUTPUT_VOLT;
    }

    StartBldcMotor( bldc_dir, bldc_volt );
    return SOUND_SELECT;
}

static U8 SelBldcDown(void)
{
    bldc_break = FALSE;
    BreakBldcMotor( bldc_break );


    bldc_volt = bldc_volt - 0.1f;
    if( bldc_volt <= 0.0f )
    {
        bldc_volt = 0.0f;
    }

    StartBldcMotor( bldc_dir, bldc_volt );
    return SOUND_SELECT;
}

static U8 SelBldcDir(void)
{
    if( bldc_dir == MOTOR_DIR_CW )
    {
        bldc_dir = MOTOR_DIR_CCW;
    }
    else
    {
        bldc_dir = MOTOR_DIR_CW;
    }

    StartBldcMotor( bldc_dir, bldc_volt );
    return SOUND_SELECT;
}

static U8 SelBldcBreak(void)
{
    if( bldc_break == TRUE )
    {
        bldc_break = FALSE;
    }
    else
    {
        bldc_break = TRUE;
    }

    BreakBldcMotor( bldc_break );
    return SOUND_SELECT;
}


static U8 SelBldcOn(void)
{
    bldc_volt = 12.0f;
    StartBldcMotor( bldc_dir, bldc_volt );
    return SOUND_POWER_ON;
}

static U8 SelBldcStop(void)
{
    bldc_volt = 0.0f;
    StartBldcMotor( bldc_dir, bldc_volt );
    return SOUND_POWER_OFF;
}
