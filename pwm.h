#ifndef _PWM_H_
#define _PWM_H_

void PWM_Init();
void PWM_Change(uint16 white, uint16 warm);
void PWM_Pulse(uint16 white, uint16 warm);
#endif