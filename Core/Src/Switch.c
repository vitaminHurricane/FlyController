#include "main.h"
#include "adc.h"
#include "Delay.h"
#include "OLED.h"
#include "OLED_Data.h"
#include "Switch.h"

int16_t Channel_Value[4] = {0};
int16_t Channel_Final_Value[4] = {0};
bool Channel_Res[4] = {true, false, true, false};

int16_t __Switch_V_to_Image(uint8_t channel, int16_t value)
{
    int16_t result;
    result = (float)value / 4096.0 * 80;
    return result;
}

void __Switch_FillBlack(int16_t X, int16_t Y, int16_t value)
{
    if (Y == C1_y) {
        OLED_DrawRectangle(X + 1, Y + 1, value, 4, 1);
        OLED_ClearArea(X + 1 + value, Y + 1, 78 - value, 4);
        OLED_UpdateArea(X, Y + 1, 80, 4);
    } else if (Y == C1_y + 10) {
        if (value > 0) {
            OLED_DrawRectangle(X + 41, Y + 1, value, 4, 1);
            OLED_ClearArea(X + 41 + value, Y + 1, 38 - value, 4);
            OLED_ClearArea(X + 1, Y + 1, 39, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        } else {
            OLED_DrawRectangle(X + 40 + value, Y + 1, -value, 4, 1);
            OLED_ClearArea(X + 1, Y + 1, 39 + value, 4);
            OLED_ClearArea(X + 40, Y + 1, 39, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        }
    } else if (Y == C1_y + 20) {
        if (value > 0) {
            OLED_DrawRectangle(X + 39, Y + 1, value, 4, 1);
            OLED_ClearArea(X + 39 + value, Y + 1, 39 - value, 4);
            OLED_ClearArea(X + 1, Y + 1, 38, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        } else {
            OLED_DrawRectangle(X + 39 + value, Y + 1, -value, 4, 1);
            OLED_ClearArea(X + 1, Y + 1, 39 + value, 4);
            OLED_ClearArea(X + 39, Y + 1, 38, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        }
    } else if (Y == C1_y + 30) {
        if (value > 0) {
            OLED_DrawRectangle(X + 39, Y + 1, value, 4, 1);
            OLED_ClearArea(X + 39 + value, Y + 1, 39 - value, 4);
            OLED_ClearArea(X + 1, Y + 1, 38, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        } else {
            OLED_DrawRectangle(X + 39 + value, Y + 1, -value, 4, 1);
            OLED_ClearArea(X + 1, Y + 1, 39 + value, 4);
            OLED_ClearArea(X + 39, Y + 1, 38, 4);
            OLED_UpdateArea(X, Y + 1, 80, 4);
        }
    }
}

void __Switch_FillKuang(int16_t X, int16_t Y, bool value)
{
    if (value == true) {
        OLED_DrawRectangle(X, Y, 8, 8, 1);
        OLED_ClearArea(X, Y + 8, 8, 8);
        OLED_UpdateArea(X, Y, 8, 16);
    } else {
        OLED_DrawRectangle(X, Y + 8, 8, 8, 1);
        OLED_ClearArea(X, Y, 8, 8);
        OLED_UpdateArea(X, Y, 8, 16);
    }
}

void Switch_Channel_MO(void)
{
    int16_t MO_C1 = __Switch_V_to_Image(1, Channel_Final_Value[0]);
    int16_t MO_C2 = __Switch_V_to_Image(2, Channel_Final_Value[1]);
    int16_t MO_C3 = __Switch_V_to_Image(3, Channel_Final_Value[2]);
    int16_t MO_C4 = __Switch_V_to_Image(4, Channel_Final_Value[3]);

    __Switch_FillBlack(C1_x, C1_y, MO_C1);
    __Switch_FillBlack(C1_x, C1_y + 10, MO_C2);
    __Switch_FillBlack(C1_x, C1_y + 20, MO_C3);
    __Switch_FillBlack(C1_x, C1_y + 30, MO_C4);
}

void Swith_Channel_Res(void)
{
    if (Channel_Res[0] == true) {       //油门通道
        Channel_Final_Value[0] = 4095 - Channel_Value[0];
    } else {
        Channel_Final_Value[0] = Channel_Value[0];
    }
    if (Channel_Res[1] == true) {       //Yaw通道
        Channel_Final_Value[1] = 2047 - Channel_Value[1];
    } else {
        Channel_Final_Value[1] = Channel_Value[1] - 2047;
    }
    if (Channel_Res[2] == true) {
        Channel_Final_Value[2] = 2047 - Channel_Value[2];
    } else {
        Channel_Final_Value[2] = Channel_Value[2] - 2047;
    }
    if (Channel_Res[3] == true) {
        Channel_Final_Value[3] = 2047 - Channel_Value[3];
    } else {
        Channel_Final_Value[3] = Channel_Value[3] - 2047;
    }
}

void Switch_Channel_Res_Show(void)
{
    __Switch_FillKuang(29, 23, Channel_Res[0]);
    __Switch_FillKuang(54, 23, Channel_Res[1]);
    __Switch_FillKuang(79, 23, Channel_Res[2]);
    __Switch_FillKuang(104, 23, Channel_Res[3]);
}

void Switch_Infor_Devide(uint8_t *TX)
{
    //数据包格式{连接状态，油门低6位，油门高6位，Yaw低6位，Yaw高6位，Pitch低6位，Pitch高6位，Roll低6位，Roll高6位}
    //先拆分油门数据，带负数注意补码
    uint8_t temp_h, temp_l;
    temp_h = Channel_Final_Value[0] >> 6;
    temp_l = Channel_Final_Value[0] & 0x3F;     //油门数据不带负数，直接拆分
    TX[1] = temp_l, TX[2] = temp_h;
    //其余姿态数据带正负，注意拆分

    temp_h = Channel_Final_Value[1] >> 8;
    temp_l = Channel_Final_Value[1] & 0xFF;
    TX[3] = temp_l, TX[4] = temp_h;
    temp_h = Channel_Final_Value[2] >> 8;
    temp_l = Channel_Final_Value[2] & 0xFF;
    TX[5] = temp_l, TX[6] = temp_h;
    temp_h = Channel_Final_Value[3] >> 8;
    temp_l = Channel_Final_Value[3] & 0xFF;
    TX[7] = temp_l, TX[8] = temp_h;
}
