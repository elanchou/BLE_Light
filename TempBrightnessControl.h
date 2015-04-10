#ifndef _TBControl_H_
#define _TBControl_H_


void TBControl_Init(void);
void TBControl_ChangeTemp(uint16 thetemp);
void TBControl_ChangeBrightness(uint16 thebrightness);
void TBControl_ChangeTempandBrightness(uint16 thetemp,uint16 thebrightness);
void SendtoPWM(uint16 thetemp,uint16 thebrightness);

#endif