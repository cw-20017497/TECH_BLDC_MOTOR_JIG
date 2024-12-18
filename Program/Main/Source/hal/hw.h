#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"


/* HW
 *
 * TIMER0 
 *  -. BASE TIMER 1ms
 *
 * PIN INTERRUPT
 *
 */


/* Watchdog timer reset */
#define	RESET_WDT()		    R_WDT_Restart()


/* LED */
#define  P_LED_11                       P0.4
#define  P_LED_12                       P2.0
#define  P_LED_13                       P2.1
#define  P_LED_14                       P2.2
#define  P_LED_15                       P2.3
#define  P_LED_16                       P2.4
#define  P_LED_17                       P2.5

#define  P_LED_21                       P2.6
#define  P_LED_22                       P2.7
#define  P_LED_23                       P14.7
#define  P_LED_24                       P14.6
#define  P_LED_25                       P1.0
#define  P_LED_26                       P1.3
#define  P_LED_27                       P1.4

#define  P_LED_31                       P1.5
#define  P_LED_32                       P1.6
#define  P_LED_33                       P1.7
#define  P_LED_34                       P5.5
#define  P_LED_35                       P5.4
#define  P_LED_36                       P5.3
#define  P_LED_37                       P5.2

#define  P_LED_41                       P5.1
                                        

#define  P_KEY_1                        P7.0
#define  P_KEY_2                        P7.1
#define  P_KEY_3                        P7.2
#define  P_KEY_4                        P7.3

// BLDC FREQUENCY GENERATOR
#define BLDC_MOTOR_FG               P5.0    
#define GET_BLDC_MOTOR_FG()         ( BLDC_MOTOR_FG )

#define BLDC_MOTOR_BRK              P0.6    
#define TURN_ON_BLDC_MOTOR_BRK()    do{ BLDC_MOTOR_BRK = 0; }while(0) 
#define TURN_OFF_BLDC_MOTOR_BRK()   do{ BLDC_MOTOR_BRK = 1; }while(0) 

#define BLDC_MOTOR_FR               P3.0    
#define TURN_ON_BLDC_MOTOR_FR()     do{ BLDC_MOTOR_FR = 0; }while(0)        // CW
#define TURN_OFF_BLDC_MOTOR_FR()    do{ BLDC_MOTOR_FR = 1; }while(0)        // CCW

#define BLDC_MOTOR_PWM              P0.5    

/* BUZZER */
#define P_BUZZER_ONOFF              P7.7

#define SMPS_12V                    P7.6 


#endif /* __HW__H__ */

