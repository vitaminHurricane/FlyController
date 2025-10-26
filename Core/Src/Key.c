#include "main.h"
#include "Key.h"
#include "Delay.h"
#include <stdbool.h>

uint8_t push_time = 0;

uint8_t __Key_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

uint8_t Key_GetState(void)      //判断按键按下状态
{
    static bool pre_level_1_7 = true, pre_level_3_10 = true;
    static bool pre_level_2_8 = true, pre_level_4_9 = true;
    static bool pre_level_5_6 = true;
    
    bool cur_level_1_7 = __Key_ReadPin(K1_PORT, K1_PIN) & __Key_ReadPin(K7_PORT, K7_PIN);
    bool cur_level_3_10 = __Key_ReadPin(K3_PORT, K3_PIN) & __Key_ReadPin(K10_PORT, K10_PIN);
    bool cur_level_2_8 = __Key_ReadPin(K2_PORT, K2_PIN) & __Key_ReadPin(K8_PORT, K8_PIN);
    bool cur_level_4_9 = __Key_ReadPin(K4_PORT, K4_PIN) & __Key_ReadPin(K9_PORT, K9_PIN);
    bool cur_level_5_6 = __Key_ReadPin(K5_PORT, K5_PIN) & __Key_ReadPin(K6_PORT, K6_PIN);

    Key_Side side = None;

    if (cur_level_1_7 == false && pre_level_1_7 == true) {
        side = Left;
    } else if (cur_level_3_10 == false && pre_level_3_10 == true) {
        side = Right;
    } else if (cur_level_2_8 == false && pre_level_2_8 == true) {
        side = Up;
    } else if (cur_level_4_9 == false && pre_level_4_9 == true) {
        side = Down;
    } else if (cur_level_5_6 == false && pre_level_5_6 == true) {
        side = Mid;
    } 
    
    pre_level_1_7 = cur_level_1_7, pre_level_2_8 = cur_level_2_8;
    pre_level_3_10 = cur_level_3_10, pre_level_4_9 = cur_level_4_9;
    pre_level_5_6 = cur_level_5_6;

    return side;
}



