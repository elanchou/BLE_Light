#include <ioCC2540.h>
#include "bcomdef.h"
#include "OSAL.h"
#include "pwm.h"
#include "TempBrightnessControl.h"

static uint16 temp = 0; // 0~100 值越大，越接近白光
static uint16 brightness = 0; //0~100

void TBControl_Init(void)
{
	PWM_Init();
}

void TBControl_ChangeTemp(uint16 thetemp)
{
	temp = thetemp;
	SendtoPWM(temp,brightness);
}

void TBControl_ChangeBrightness(uint16 thebrightness)
{
	brightness = thebrightness;
	SendtoPWM(temp,brightness);
}

void SendtoPWM(uint16 thetemp,uint16 thebrightness)
{
	float totalbright = 375 * (thebrightness / 100.0);
	float whitebrignt = totalbright * (temp / 100.0);
	float warmbright = totalbright * ((100-temp) / 100.0);
	PWM_Change((uint16)whitebrignt,(uint16)warmbright);
}