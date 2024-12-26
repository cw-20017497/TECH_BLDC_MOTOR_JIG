#include "hw.h"
#include "hal_key.h"


#define  HAL_KEY_PUSHED     LOW
#define  HAL_LEVER_PUSHED   LOW

#define  DEFAULT_COUNT      50


typedef struct _hal_key_
{
    U32 Key;
    U8  WaterOut;
} TKeyVal;

TKeyVal HalKey;


void HAL_InitKey(void)
{
    HalKey.Key      = 0UL;
    HalKey.WaterOut = 0U;
}

void HAL_SetKeyVal(U32 mu32Val)
{
    HalKey.Key = mu32Val;
}

U32 HAL_GetKeyVal(void)
{
    return HalKey.Key;
}


void HAL_SetLeverWater(U8 mu8Val)
{
    HalKey.WaterOut = mu8Val;
}


U8 HAL_GetLeverWater(void)
{
    return HalKey.WaterOut;
}

static void ScanKey(void)
{
    static U16 mu16Count = 0;
    static U32 mu32PrevVal = 0;
    U32 mu32Val = 0;


    // SCAN KEY
    if( P_KEY_1 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_1;
    }
    if( P_KEY_2 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_2;
    }
    if( P_KEY_3 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_3;
    }
    if( P_KEY_4 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_4;
    }

    // RESET KEY COUNT
    if( mu32PrevVal != mu32Val )
    {
        mu32PrevVal = mu32Val;
        mu16Count = DEFAULT_COUNT;

        return ;
    }

    if( mu16Count != 0 )
    {
        mu16Count--;
        return ;
    }

    // UPDATE NEW KEY
    HalKey.Key = mu32Val;
}

void HAL_ScanKey(void)
{
    ScanKey();
}

