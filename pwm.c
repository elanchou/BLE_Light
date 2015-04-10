#include <ioCC2540.h>
#include "bcomdef.h"
#include "OSAL.h"
#include "pwm.h"

//pwm pins:
//P0_3:白  (TX)--ch1
//P0_4:暖  (CTS)--ch2
static uint16 WhiteLight =1;
static uint16 WarmLight =1;



void PWM_Init()
{
  //设置pwm端口为输出
  P0DIR|= BV(3)|BV(4);
  //设置pwm端口为外设端口，非gpio
  P0SEL|= BV(3)|BV(4);
  //由于uart等会占用我们当前使用的pwm端口，因此需要将uart等重映射到别的端口去。
  PERCFG |= 0x03;             // Move USART1&2 to alternate2 location so that T1 is visible

  // Initialize Timer 1
  T1CTL = 0x0C;               // Div = 128, CLR, MODE = Suspended          
  T1CCTL1 = 0x0C;             // IM = 0; CMP = Clear output on compare; Mode = Compare
  T1CCTL2 = 0x0C;             // IM = 0; CMP = Clear output on compare; Mode = Compare
  T1CNTL = 0;                 // Reset timer to 0;

    //必须设置，否则定时器不工作
  T1CCTL0 = 0x4C;            // IM = 1, CMP = Clear output on compare; Mode = Compare

  //设置周期的tick为375, 也就是1.5ms
#if 1
  T1CC0H = 0x01;              // Ticks = 375 (1.5ms initial duty cycle)
  T1CC0L = 0x77;              //             
  T1CC1H = 0x01;              // Ticks = 375 (1.5ms initial duty cycle)
  T1CC1L = 0x77;
  T1CC2H = 0x01;              // Ticks = 375 (1.5ms initial duty cycle)
  T1CC2L = 0x77;
#else//以下设置会是1khz
#define VALUE_H     0x00
#define VALUE_L     0x10
  T1CC0H = VALUE_H;    
  T1CC0L = VALUE_L;    
  T1CC1H = VALUE_H;    
  T1CC1L = VALUE_L;
  T1CC2H = VALUE_H;    
  T1CC2L = VALUE_L;
  T1CC3H = VALUE_H;    
  T1CC3L = VALUE_L;  
#endif 

  EA=1;
  IEN1 |= 0x02;               // Enable T1 cpu interrupt
}

//red， green， blue 的值必须是 1~375, 其他值无效
void PWM_Pulse(uint16 white, uint16 warm)
{
  uint16 wh,wa;

  // stop,注意，不能加这句，加了周期偏差十几倍，具体原因未查明
  //T1CTL &= BV(0)|BV(1); 

  wh = white;
  wa = warm;

  // Set up the timer registers

  T1CC1L = (uint8)wh;
  T1CC1H = (uint8)(wh >> 8);
  //避免比较值为0时，输出一直为高
  if(wh!=0){
    T1CCTL1 = 0x0C;
  }else{
    T1CCTL1 = 0x00;
  }
  T1CC2L = (uint8)wa;
  T1CC2H = (uint8)(wa >> 8);
  if(wa!=0){
    T1CCTL2 = 0x0C;
  }else{
    T1CCTL2 = 0x00;
  }

  // Reset timer
  T1CNTL = 0;
  

  // Start timer in modulo mode.
  T1CTL |= 0x02;   
}

//red， green， blue 的值必须是 1~375, 其他值无效
void PWM_Change(uint16 white, uint16 warm)
{    
  WhiteLight = white;
  WarmLight = warm;
}

//#pragma register_bank=2
#pragma vector = T1_VECTOR
__interrupt void pwmISR (void) 
{
    uint8 flags = T1STAT;
    // T1 ch 0
    if (flags & 0x01){          
      
      // Stop Timer 1
      //T1CTL |= 0x02;
    //red， green， blue 的值必须是 1~375, 其他值无效
      PWM_Pulse(WhiteLight,WarmLight);
     
    }
    T1STAT = ~ flags;
}
 