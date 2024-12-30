#include "hw.h"
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "key_event_lib.h"
#include "front.h"
#include "sound.h"
#include "process_display.h"
#include "timer.h"
#include "timer_id.h"
#include "bldc_motor.h"


static U8 SelSound(void);

static U8 SelBldcUpEx(void);
static U8 SelBldcDownEx(void);
static U8 SelBldcUp(void);
static U8 SelBldcDown(void);
static U8 SelBldcDir(void);
static U8 SelBldcBreak(void);
static U8 SelBldcStop(void);
static U8 SelBldcOn(void);


U8 TestDurability = FALSE;
U8 TestDurability_Step = 0;
static U8 SelDurability(void);

// 일반 모드 리스트
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,            2sec,           3sec,  5sec,  Pop,           TS */
    { K_1,              SelBldcUpEx,       NULL,  NULL,  NULL,  KeyStopCont,   KeyContUp },
    { K_2,              SelBldcDownEx,     NULL,  NULL,  NULL,  KeyStopCont,   KeyContDown },
    { K_3,              SelBldcDir,      NULL,  NULL,  NULL,  NULL,          NULL },
    { K_4,              SelBldcBreak,    NULL,  NULL,  NULL,  NULL,          NULL },
    { K_ON,             NULL,            NULL,  NULL,  NULL,  NULL,          SelBldcOn },
    { K_OFF,            NULL,            NULL,  NULL,  NULL,  NULL,          SelBldcStop },
#if CONFIG_TEST_DURABILITY
    { K_DURABILITY,     NULL,            NULL,  NULL,  NULL,  NULL,          SelDurability },
#endif
};

U8 IsValidNormalKeyCondition(U32 mu32Key)
{
    if( TestDurability == TRUE )
    {
        if( mu32Key != K_DURABILITY )
        {
            return FALSE;
        }
    }
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
U8 set_blink = FALSE;
static U8 SelBldcUp(void)
{
    U8 sound = SOUND_SELECT;

    if( bldc_volt == 0 )
    {
        return SOUND_ERROR;
    }


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
        sound = SOUND_ERROR;
    }

    StartBldcMotor( bldc_dir, bldc_volt );
    return sound;
}

static U8 SelBldcUpEx(void)
{
    U8 sound;

    sound = SelBldcUp();
    if( bldc_volt >= MAX_BLDC_OUTPUT_VOLT )
    {
        set_blink = TRUE;
    }
    return sound;
}

static U8 SelBldcDown(void)
{
    U8 sound = SOUND_SELECT;


    if( bldc_volt == 0 )
    {
        return SOUND_ERROR;
    }

    bldc_break = FALSE;
    BreakBldcMotor( bldc_break );


    bldc_volt = bldc_volt - 0.1f;
    if( bldc_volt <= 3.0f )
    {
        bldc_volt = 3.0f;
        sound = SOUND_ERROR;
    }

    StartBldcMotor( bldc_dir, bldc_volt );
    return sound;
}

static U8 SelBldcDownEx(void)
{
    U8 sound;

    sound = SelBldcDown();
    if( bldc_volt <= 3.0f )
    {
        set_blink = TRUE;
    }

    return sound;
}

static U8 SelBldcDir(void)
{
    if( bldc_volt == 0 )
    {
        return SOUND_ERROR;
    }

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

    bldc_break = FALSE;
    BreakBldcMotor( bldc_break );

    return SOUND_CONFIG_SETUP;
}

static U8 SelBldcStop(void)
{
    bldc_volt = 0.0f;
    StartBldcMotor( bldc_dir, bldc_volt );

    bldc_break = TRUE;
    BreakBldcMotor( bldc_break );
    return SOUND_CONFIG_CANCEL;
}


static U8 SelDurability(void)
{
    TestDurability = !TestDurability;

    if( TestDurability == TRUE )
    {
        StartTimer( TIMER_ID_DURABILITY, SEC(1) ); 
        TestDurability_Step = 0;
    }
    else
    {
        StopTimer( TIMER_ID_DURABILITY );
        return SelBldcStop();
    }

   return SOUND_SELECT;
}


void Evt_Durability_Handler( void )
{
    switch( TestDurability_Step )
    {
        case 0:
            // ON CW - 24V 
            StartBldcMotor( MOTOR_DIR_CW, 24.0f );
            BreakBldcMotor( FALSE );
            TestDurability_Step++;

            StartTimer( TIMER_ID_DURABILITY, SEC(2) );
            break;

        case 1:
            // OFF 
            StartBldcMotor( MOTOR_DIR_CW, 0.0f );
            BreakBldcMotor( TRUE );
            TestDurability_Step++;

            StartTimer( TIMER_ID_DURABILITY, SEC(2) );
            break;

        case 2:
            // ON CCW, 24V 
            StartBldcMotor( MOTOR_DIR_CCW, 24.0f );
            BreakBldcMotor( FALSE );
            TestDurability_Step++;

            StartTimer( TIMER_ID_DURABILITY, SEC(2) );
            break;

        case 3:
            // OFF 
            StartBldcMotor( MOTOR_DIR_CCW, 0.0f );
            BreakBldcMotor( TRUE );
            TestDurability_Step = 0;

            StartTimer( TIMER_ID_DURABILITY, SEC(2) );
            break;

        default:
            StartBldcMotor( MOTOR_DIR_CCW, 24.0f );
            BreakBldcMotor( TRUE );
            TestDurability_Step = 0;

            StartTimer( TIMER_ID_DURABILITY, SEC(1) );
            break;
    }
}

