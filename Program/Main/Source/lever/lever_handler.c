#include "lever_handler.h"
#include "lever_common_handler.h"

#include "water_out.h"
#include "flush_water_out.h"
#include "front.h"
#include "process_display.h"
#include "sound.h"
//#include "cold_water.h"
//#include "error.h"
//#include "flow_meter.h"
//#include "hot_water.h"



// 읾반 모드
/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);
static U8 LeverOpenTapWaterLong(void);

const static LeverEventList_T LeverEventList[] =
{
    /*   Short,                Long(2초),            Long2 (8초)           Pop  */
    {  LeverOpenTapWater,    LeverOpenTapWaterLong,  LeverCloseTapWater,   NULL  },
};


/* WATER OUT - START */
extern U8 mu8SelButton;
static U8 LeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;


    if( GetWaterOut() == TRUE )
    {
        StopWaterOut();
        return SOUND_EFFLUENT_END;
    }

    // 물 추출 종료 후, 일정 시간동안 재추출 금지
    if( IsExpiredOutProtectTime() == FALSE )
    {
        return SOUND_NONE;
    }

    // 추출 재시도하면 단수 에러 해제
    //SetOutageStatus( TYPE_WATER, FLOW_SUPPLY );

    // 연속 추출음 설정
    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        mu8Sound = SOUND_EFFLUENT_CONTINUE;
    }

    mu8SelButton = 1;
    SetWaterOutSelect( SEL_WATER_ROOM );
    mu8Sound = StartWaterOutFlush( mu8Sound );

    return mu8Sound;
}


// 초기 채우기 동작은 강제 취소
//static U8 CancelFlushPowreOn(void)
//{
//    if( IsDoneFlushStatus() == TRUE 
//            && GetFlushPowerOn() == FLUSH_STATUS_INIT )
//    {
//        //SetColdWaterInitFull( TRUE );
//        SetFlushPowerOn( FLUSH_STATUS_DONE );
//        SetWaterOutSelect( SEL_WATER_ROOM );
//
//        return TRUE;
//    }
//
//    return FALSE;
//}

static U8 LeverCloseTapWater(void)
{
    U8 mu8Sound = SOUND_NONE;

    // 추출 중이면, 추출 중지
    if( GetWaterOut() == TRUE )
    {
        StopWaterOut();
        
        //CancelFlushPowreOn();
        //SetFotaReboot( FALSE );
        return SOUND_EFFLUENT_END;
    }

    //if( CancelFlushPowreOn() == TRUE )
    //{
    //    return SOUND_CANCEL;
    //}

    return mu8Sound;
}

static U8 WaterOutInfinity(void)
{
    if( GetWaterOut() == TRUE )
    {
        // Skip long button when FLUSHING...
        //if( IsDoneFlushInitSetup() == FALSE )
        //{
        //    return FALSE;
        //}

        if( GetWaterOutAmoutType() != FIXED_AMOUNT_INFINITY )
        {
            SetWaterOutInfinityLongKey();
            return TRUE;
        }
    }

    return FALSE;
}

static U8 LeverOpenTapWaterLong(void)
{
    U8 mu8Sound = SOUND_NONE;


    if( WaterOutInfinity() == TRUE )
    {
        mu8Sound =  SOUND_EFFLUENT_CONTINUE;
    }

    return mu8Sound;
}



/* WATER OUT */
static U8 CodyLeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;


    if( GetWaterOut() == TRUE )
    {
        StopWaterOut();
        return SOUND_EFFLUENT_END;
    }

    // 물 추출 종료 후, 일정 시간동안 재추출 금지
    if( IsExpiredOutProtectTime() == FALSE )
    {
        return SOUND_NONE;
    }

    // 소다 저수위 추출 금지
    //if( GetWaterOutSelect() == SEL_WATER_SODA 
    //        && GetColdWaterLevel() == LEVEL_LOW )
    //{
    //    return SOUND_ERROR;
    //}

    StartWaterOut();
    
    return mu8Sound;
}


void* GetLeverEventList(void)
{
#if 0
    if( GetCodyService() == TRUE )
    {
        return (void *)CodyLeverEventList;
    }
#endif
    //else if( IsMenuService() == TRUE )
    //{
    //    return (void *)ServiceLeverEventList;
    //}
    // Return address of lever event table 
    return (void *)LeverEventList;
}

