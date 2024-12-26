#include "display.h"
#include "hal_led.h"
#include "display_lib.h"


/* 7segment:  0 ~ 9 */
static U8 gu8Digit[10] = 
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x07F, 0x06F 
};

/* 7segment Char */
#define CHAR_O              0x3F
#define CHAR_n              0x54
#define CHAR_F              0x71
#define CHAR_S              0x6D
#define CHAR_T              0x78
#define CHAR_E              0x79
#define CHAR_o              0x5C
#define CHAR_C              0x39
#define CHAR_H              0x76
#define CHAR_A              0x77
#define CHAR_P              0x73
#define CHAR_SPACE          0x00
#define BAR_LEFT            0x30
#define BAR_RIGHT           0x06
#define BAR_TOP             0x01
#define BAR_CENTER          0x40
#define BAR_BOTTOM          0x08
#define BAR_BRAKET_LEFT     0x39    // '['
#define BAR_BRAKET_RIGHT    0x0F    // ']'
#define BAR_TOP_BOTTOM      0x09    // '='
#define RING_TOP            0x63    // 
#define RING_BOTTOM         0x5C
#define STACK_BAR_CENTER   (BAR_CENTER|BAR_BOTTOM)
#define STACK_BAR_TOP      (BAR_CENTER|BAR_BOTTOM|BAR_TOP)

#define SEG_OFF             0x00    //
#define SEG_A               0x01    // SEG_0
#define SEG_B               0x02    // SEG_1
#define SEG_C               0x04    // SEG_2
#define SEG_D               0x08    // SEG_4
#define SEG_E               0x10    // SEG_5
#define SEG_F               0x20    // SEG_6
#define SEG_G               0x40    // SEG_7

#define SEG_EF              (SEG_E|SEG_F)
#define SEG_EFA             (SEG_E|SEG_F|SEG_A)
#define SEG_EFAD            (SEG_E|SEG_F|SEG_A|SEG_D)
#define SEG_FAD             (SEG_F|SEG_A|SEG_D)
#define SEG_AD              (SEG_A|SEG_D)
#define SEG_AB              (SEG_A|SEG_B)
#define SEG_ABC             (SEG_A|SEG_B|SEG_C)
#define SEG_ABCD            (SEG_A|SEG_B|SEG_C|SEG_D)
#define SEG_BCD             (SEG_B|SEG_C|SEG_D)
#define SEG_CD              (SEG_C|SEG_D)

/* 7segemnt id */
#define SEGMENT_1       SEG_3_A
#define SEGMENT_10      SEG_2_A
#define SEGMENT_100     SEG_1_A
#define SEGMENT_1000    100
static void DispSegment(U8 mu8Seg, U8 mu8Val)
{
    LedId_T mLedId;

    if( mu8Seg == SEGMENT_1000 )
    {
    //    mLedId = mu8Seg;
    //    HAL_TurnOnOffLED( mLedId,      ((mu8Val & 0x04) >> 2) );
    //    HAL_TurnOnOffLED( mLedId + 1,  ((mu8Val & 0x02) >> 1) );
    }
    else
    {
        mLedId = mu8Seg;
        
        HAL_TurnOnOffLED( mLedId,     ((mu8Val & 0x01) ));          // a    0x01
        HAL_TurnOnOffLED( mLedId + 1, ((mu8Val & 0x20) >> 5) );     // f    0x20
        HAL_TurnOnOffLED( mLedId + 2, ((mu8Val & 0x40) >> 6) );     // g    0x40
        HAL_TurnOnOffLED( mLedId + 3, ((mu8Val & 0x10) >> 4) );     // e    0x10
        HAL_TurnOnOffLED( mLedId + 4, ((mu8Val & 0x08) >> 3) );     // d    0x08
        HAL_TurnOnOffLED( mLedId + 5, ((mu8Val & 0x04) >> 2) );     // c    0x04
        HAL_TurnOnOffLED( mLedId + 6, ((mu8Val & 0x02) >> 1) );     // b    0x02
    }
}

void DispSegmentDigit(U16 mu16Val)
{
    U8 mu8Val_1000;
    U8 mu8Val_100;
    U8 mu8Val_10;
    U8 mu8Val_1;

    mu8Val_1000 = (U8)(mu16Val / 1000);
    mu8Val_100  = (U8)((mu16Val % 1000) / 100);
    mu8Val_10   = (U8)((mu16Val % 100) / 10);
    mu8Val_1    = (U8)((mu16Val % 10));

    if( mu8Val_1000 > 0 )
    {
        DispSegment( SEGMENT_1000, gu8Digit[ mu8Val_1000 ] );
    }
    else
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE );
    }

    if( mu8Val_100 > 0 || mu8Val_1000 > 0)
    {
        DispSegment( SEGMENT_100, gu8Digit[ mu8Val_100 ] );
    }
    else
    {
        DispSegment( SEGMENT_100, CHAR_SPACE );
    }

    if( mu8Val_10 > 0 || mu8Val_100 > 0 || mu8Val_1000 > 0 )
    {
        DispSegment( SEGMENT_10, gu8Digit[ mu8Val_10 ] );
    }
    else
    {
        //DispSegment( SEGMENT_10, CHAR_SPACE );
        DispSegment( SEGMENT_10, gu8Digit[0] );
    }

    DispSegment( SEGMENT_1, gu8Digit[ mu8Val_1 ] );
}


/* mu8Val : 0 ~ 9
 * display -1-.. -2- ... -3-...
 */

static void DispCenterVal(U8 mu8Val)
{
    if( mu8Val > 9 )
    {
        mu8Val = 9;
    }
    DispSegment( SEGMENT_1000,  CHAR_SPACE  );
    DispSegment( SEGMENT_100,   BAR_CENTER );
    DispSegment( SEGMENT_10,    gu8Digit[mu8Val] );
    DispSegment( SEGMENT_1,     BAR_CENTER );
}

// 7세그먼트에 캐릭터 표시
void DispSegmentChar(U8 mu8CharId)
{
    if( mu8CharId == SEGMENT_CHAR_ID_ON )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_O );
        DispSegment( SEGMENT_10,   CHAR_n );
        DispSegment( SEGMENT_1,    CHAR_SPACE);
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_OFF )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_O );
        DispSegment( SEGMENT_10,   CHAR_F );
        DispSegment( SEGMENT_1,    CHAR_F );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CON )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_C );
        DispSegment( SEGMENT_10,   CHAR_o );
        DispSegment( SEGMENT_1,    CHAR_n );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STACK_BAR_CENTER )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   STACK_BAR_CENTER  );
        DispSegment( SEGMENT_10,    STACK_BAR_CENTER  );
        DispSegment( SEGMENT_1,     STACK_BAR_CENTER  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STACK_BAR_TOP )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   STACK_BAR_TOP  );
        DispSegment( SEGMENT_10,    STACK_BAR_TOP  );
        DispSegment( SEGMENT_1,     STACK_BAR_TOP  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_BOTTOM )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_BOTTOM  );
        DispSegment( SEGMENT_10,    BAR_BOTTOM  );
        DispSegment( SEGMENT_1,     BAR_BOTTOM  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_CENTER )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_CENTER  );
        DispSegment( SEGMENT_10,    BAR_CENTER  );
        DispSegment( SEGMENT_1,     BAR_CENTER  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_TOP )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_TOP  );
        DispSegment( SEGMENT_10,    BAR_TOP  );
        DispSegment( SEGMENT_1,     BAR_TOP  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CHP )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_C  );
        DispSegment( SEGMENT_10,    CHAR_H  );
        DispSegment( SEGMENT_1,     CHAR_P  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CP )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_C  );
        DispSegment( SEGMENT_10,    CHAR_P  );
        DispSegment( SEGMENT_1,     CHAR_SPACE  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CIRCLE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_BRAKET_LEFT );
        DispSegment( SEGMENT_10,    BAR_TOP_BOTTOM );
        DispSegment( SEGMENT_1,     BAR_BRAKET_RIGHT );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_1 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_TOP );
        DispSegment( SEGMENT_10,    CHAR_SPACE );
        DispSegment( SEGMENT_1,     CHAR_SPACE );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_2 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_BOTTOM );
        DispSegment( SEGMENT_10,    RING_TOP );
        DispSegment( SEGMENT_1,     CHAR_SPACE );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_3 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_TOP );
        DispSegment( SEGMENT_10,    RING_BOTTOM );
        DispSegment( SEGMENT_1,     RING_TOP );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_4 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_BOTTOM );
        DispSegment( SEGMENT_10,    RING_TOP );
        DispSegment( SEGMENT_1,     RING_BOTTOM );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_1 )
    {
        DispCenterVal( 1 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_2 )
    {
        DispCenterVal( 2 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_3 )
    {
        DispCenterVal( 3 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_SE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_SPACE );
        DispSegment( SEGMENT_10,    CHAR_S );
        DispSegment( SEGMENT_1,     CHAR_E );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CO )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_SPACE );
        DispSegment( SEGMENT_10,    CHAR_C );
        DispSegment( SEGMENT_1,     CHAR_O );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE );
        DispSegment( SEGMENT_100,   CHAR_S );
        DispSegment( SEGMENT_10,    CHAR_T );
        DispSegment( SEGMENT_1,     CHAR_E );
    }
    else
    {
        // mu8CharId == SPACE
        DispSegment( SEGMENT_1000, CHAR_SPACE );
        DispSegment( SEGMENT_100,  CHAR_SPACE );
        DispSegment( SEGMENT_10,   CHAR_SPACE );
        DispSegment( SEGMENT_1,    CHAR_SPACE );
    }
}

void DispSegmentOff(void)
{
    // DISPALY OFF
    DispSegment( SEGMENT_1000, CHAR_SPACE );
    DispSegment( SEGMENT_100, CHAR_SPACE );
    DispSegment( SEGMENT_10, CHAR_SPACE );
    DispSegment( SEGMENT_1, CHAR_SPACE );
}





void TurnOffAllLED(void)
{
    HAL_TurnOnOffLEDAll( OFF );
    HAL_DimmingLedAll( OFF );
}


void TurnOnAllLED(void)
{
    HAL_TurnOnOffLEDAll( ON );
    HAL_DimmingLedAll( OFF );
}

#if CONFIG_TEST_LED 
U8 dbg_duty_on           = 10;
U8 dbg_duty_dimming      = 0;

U8 dbg_duty_dark         = 50;
U8 dbg_duty_dimming_dark = 20;
#endif

void DispLedDuty(void)
{
#if CONFIG_TEST_LED
    HAL_SetOnOffDuty( dbg_duty_on );
    HAL_SetDimmingDuty( dbg_duty_dimming );
#else
    HAL_SetOnOffDuty( DEFAULT_ONOFF_DUTY );
    HAL_SetDimmingDuty( DEFAULT_DIMMING_DUTY );
#endif
}


extern U8 bldc_dir;
extern U8 bldc_break;
U16 dbg_seg_val = 0;
void DispWaterOutAmount(U16 mu16Val)
{
    if( bldc_break == TRUE )
    {
        // 연속 선택시 mL 미표시
        DispSegmentChar(SEGMENT_CHAR_ID_BAR_CENTER);
        HAL_TurnOnOffLED( SEG_22, OFF );
    }
    else
    {
        // 정량 선택시 mL 표시
        DispSegmentDigit( mu16Val );

        HAL_TurnOnOffLED( SEG_22, ON );
        
    }
}

