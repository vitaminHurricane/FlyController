#ifndef __SWITCH_H
#define __SWITCH_H
#include "main.h"
#include "Delay.h"

#define C1_x        30
#define C1_y        12
#define C_w         80
#define C_h         6
#define split       10

/**
 *  油门通道是PA1 -> value[0]
 *  Yaw通道是PA0 -> value[1]
 *  Pitch通道是PA3 -> value[2]
 *  Roll通道是PA2 -> value[3]
 */

extern int16_t Channel_Value[];
extern int16_t Channel_Final_Value[];
extern bool Channel_Res[];

int16_t __Switch_V_to_Image(uint8_t channel, int16_t value);
void __Switch_FillKuang(int16_t X, int16_t Y, bool value);
void __Switch_FillBlack(int16_t X, int16_t Y, int16_t value);

void Switch_Channel_MO(void);
void Swith_Channel_Res(void);
void Switch_Channel_Res_Show(void);
void Switch_Infor_Devide(uint8_t *TX);

#endif
