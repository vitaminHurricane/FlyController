#include "main.h"
#include "Menu.h"
#include "OLED.h"
#include "Key.h"
#include "Switch.h"
#include "OLED_Data.h"
#include "Delay.h"
#include "NRF.h"
#include "NRF_Command.h"
#include "stm32f1xx_it.h"
#include <stdbool.h>

bool BIND_Flag = false;

void Menu_Start(void)
{
    OLED_ShowString(33, 25, "Welcome", OLED_8X16);
    OLED_Update();
    Delay_s(1);
    OLED_Clear();

    OLED_ShowImage(1, 1, 12, 8, Battery_FULL);
    OLED_ShowImage(110, 1, 12, 8, No_Signal);
    OLED_ShowString(1, 12, "Battery state:", OLED_6X8);
    OLED_ShowString(1, 22, "Signal state:", OLED_6X8);

    OLED_ShowImage(20, 33, 20, 20, SETTING);
    OLED_ShowImage(80, 33, 20, 20, BIND);

    OLED_ShowString(10, 55, "SETTING", OLED_6X8);
    OLED_ShowString(79, 55, "BIND", OLED_6X8);

    OLED_Update();
}

void __Menu_Clear(void)
{
    OLED_ClearArea(1, 9, 127, 54);
}

void __Menu_Home_SETTING(void)
{
    OLED_ShowImage(18, 31, 24, 24, Square_SETTING); 
    OLED_UpdateArea(18, 31, 24, 24);
    OLED_ClearArea(78, 31, 24, 24);
    OLED_ShowImage(80, 33, 20, 20, BIND);
    OLED_UpdateArea(78, 31, 24, 24);
}

void __Menu_Home_BIND(void)
{
    OLED_ShowImage(78, 31, 24, 24, Square_BIND);
    OLED_UpdateArea(78, 31, 24, 24);
    OLED_ClearArea(18, 31, 24, 24);
    OLED_ShowImage(20, 33, 20, 20, SETTING);
    OLED_UpdateArea(18, 31, 24, 24);
}

uint8_t __Menu_BIND_Searching(void)
{
    TX_Buf[0] = BIND_START;
    OLED_ShowImage(75, 40, 8, 8, Dot);
    OLED_UpdateArea(75, 40, 8, 8);
    Delay_ms(300);
    if (H24_SendData()) { 
        OLED_ClearArea(75, 40, 28, 8);
        OLED_UpdateArea(75, 40, 28, 8);
        return 1; 
    }
    OLED_ShowImage(85, 40, 8, 8, Dot);
    OLED_UpdateArea(85, 40, 8, 8);
    Delay_ms(300);
    if (H24_SendData()) { 
        OLED_ClearArea(75, 40, 28, 8);
        OLED_UpdateArea(75, 40, 28, 8);
        return 1; 
    }
    OLED_ShowImage(95, 40, 8, 8, Dot);
    OLED_UpdateArea(95, 40, 8, 8);
    Delay_ms(300);
    if (H24_SendData()) { 
        OLED_ClearArea(75, 40, 28, 8);
        OLED_UpdateArea(75, 40, 28, 8);
        return 1; 
    }
    OLED_ClearArea(75, 40, 28, 8);
    OLED_UpdateArea(75, 40, 28, 8);
    Delay_ms(300);
    if (H24_SendData()) { 
        return 1; 
    }
    return 0;
}

void __Menu_One_ShowIndex(uint8_t index, uint8_t state)
{
    static uint8_t pre_index = 1;
    if (state == 1) {
        if (pre_index != 0) {
            OLED_ReverseArea(1, 12 + (pre_index - 1) * 10, 127, 8);
        }
        OLED_ReverseArea(1, 12 + (index - 1) * 10, 127, 8);
        pre_index = index;
        OLED_Update();
    } else {
        pre_index = index;
    }
}

void __Menu_Channel_SetClear(void)
{
    OLED_ClearArea(28, 46, 90, 8);
    OLED_UpdateArea(28, 46, 90, 8);
}

void __Menu_Channel_SetShowIndex(uint8_t index)
{
    __Menu_Channel_SetClear();
    switch (index)
    {
    case 1:
        OLED_ShowImage(28, 46, 8, 8, UP_Arrow);
        OLED_UpdateArea(28, 46, 8, 8);
        break;
    case 2:
        OLED_ShowImage(53, 46, 8, 8, UP_Arrow);
        OLED_UpdateArea(53, 46, 8, 8);
        break;
    case 3:
        OLED_ShowImage(78, 46, 8, 8, UP_Arrow);
        OLED_UpdateArea(78, 46, 8, 8);
        break;
    case 4:
        OLED_ShowImage(103, 46, 8, 8, UP_Arrow);
        OLED_UpdateArea(103, 46, 8, 8);
        break;
    default:
        break;
    }
}

uint8_t Menu_Home(void)                     //主菜单，初始化界面
{
    static uint8_t state = 0;
    uint8_t temp = state;
    OLED_ShowString(1, 12, "Battery state:", OLED_6X8);
    OLED_ShowString(1, 22, "Signal state:", OLED_6X8);
    OLED_ShowString(10, 55, "SETTING", OLED_6X8);
    OLED_ShowString(79, 55, "BIND", OLED_6X8);
    OLED_ShowImage(20, 33, 20, 20, SETTING);
    OLED_ShowImage(80, 33, 20, 20, BIND);
    OLED_Update();
    while (1) {
        static bool first_in = true;
        if (first_in) {                     //刚开机，无论按哪个按键都会选定到SETTING
            if (keynum != 0) {
                OLED_ShowImage(18, 31, 24, 24, Square_SETTING); 
                OLED_UpdateArea(18, 31, 24, 24);
                state++;
                first_in = false;
                Delay_ms(50);
            }
        } else {
            if (keynum != 0) {
                switch (keynum){
                case Left:
                    if (state > 1) {
                        __Menu_Home_SETTING();
                        state--;
                    }
                    break;
                case Right:
                    if (state < 2) {
                        __Menu_Home_BIND();
                        state++;
                    }
                    break;
                case Mid:
                    temp = state;
                    state = 0;
                    first_in = true;
                    goto out;
                default:
                    break;
                }
            }
        }
    }
    out:
    return temp;
}

void Menu_BIND(void)
{
    OLED_ClearArea(78, 31, 24, 24);
    OLED_UpdateArea(78, 31, 24, 24);

    while (1) {
        static uint8_t cnt = 0;
        if (!BIND_Flag) {
            if (__Menu_BIND_Searching()) {
                OLED_DrawLine(81, 48, 85, 52);
                OLED_DrawLine(85, 52, 99, 38);
                OLED_UpdateArea(80, 33, 20, 20);
                OLED_ShowImage(110, 1, 12, 8, MAX_Signal);
                OLED_UpdateArea(110, 1, 12, 8);
                Delay_ms(500);
                TX_Buf[0] = BIND_OK;
                get_time = current_time;
                BIND_Flag = true;
                break;
            }
            cnt++;
            if (cnt > 4) {                  //尝试对码4次，失败则显示×
                cnt = 0;
                TX_Buf[0] = 0x00;
                OLED_DrawLine(81, 34, 99, 52);
                OLED_DrawLine(81, 52, 99, 34);
                OLED_UpdateArea(80, 33, 20, 20);
                Delay_ms(500);
                break;
            }
        } else {
            OLED_ShowString(68, 31, "Already", OLED_6X8);
            OLED_ShowString(78, 41, "BIND", OLED_6X8);
            OLED_UpdateArea(68, 31, 45, 24);
            Delay_ms(500);
            OLED_ClearArea(68, 31, 45, 24);
            break;
        }
    }

}

uint8_t Menu_One(void)                      //一级菜单，显示基础内容
{
    uint8_t temp = 0;
    __Menu_Clear();
    OLED_ShowString(1, 12, "Channel test", OLED_6X8);
    OLED_ShowString(1, 22, "Channel set", OLED_6X8);
    OLED_ShowString(1, 32, "Balance set", OLED_6X8);
    OLED_ShowString(1, 42, "Back", OLED_6X8);
    OLED_ReverseArea(1, 12, 127, 8);
    OLED_Update();
    while (1) {                           
        static uint8_t index = 1;               //首次进入或者退出二级菜单到一级菜单时重新刷新整个OLED屏幕
        if (keynum != 0) {
            switch (keynum) {
            case Up:
                if (index > 1) {
                    index--;
                    __Menu_One_ShowIndex(index, 1);
                }
                break;
            case Down:
                if (index < 4) {
                    index++;
                    __Menu_One_ShowIndex(index, 1);
                }
                break;
            case Mid:
                temp = index;
                index = 1;
                __Menu_One_ShowIndex(index, 0);
                __Menu_Clear();
                goto out;
            default:
                break;
            }
        }
    }
    out:
    return temp;
}

void Menu_Channel_Test(void)
{
    __Menu_Clear();
    OLED_ShowString(1, 12, "CH1", OLED_6X8);
    OLED_ShowString(1, 22, "CH2", OLED_6X8);
    OLED_ShowString(1, 32, "CH3", OLED_6X8);
    OLED_ShowString(1, 42, "CH4", OLED_6X8);
    OLED_ShowImage(30, 12, 80, 6, Kuang);
    OLED_ShowImage(30, 22, 80, 6, Kuang);
    OLED_ShowImage(30, 32, 80, 6, Kuang);
    OLED_ShowImage(30, 42, 80, 6, Kuang);
    OLED_Update();
    while (1) {
        Switch_Channel_MO();
        //Delay_ms(10);
        if (keynum != 0) {
            switch (keynum)
            {
            case Mid:
                __Menu_Clear();
                goto out;
            default:
                break;
            }
        }
    }
    out:
    return;
}

void Menu_Channel_Set(void)
{
    __Menu_Clear();
    OLED_ShowString(25, 12, "CH1", OLED_6X8);
    OLED_ShowString(50, 12, "CH2", OLED_6X8);
    OLED_ShowString(75, 12, "CH3", OLED_6X8);
    OLED_ShowString(100, 12, "CH4", OLED_6X8);
    OLED_DrawRectangle(28, 22, 10, 20, 0);
    OLED_DrawRectangle(53, 22, 10, 20, 0);
    OLED_DrawRectangle(78, 22, 10, 20, 0);
    OLED_DrawRectangle(103, 22, 10, 20, 0);
    OLED_ShowString(1, 44, "Res", OLED_6X8);
    Switch_Channel_Res_Show();
    OLED_Update();
    uint8_t index = 0;
    while (1) {
        static bool first_in = true;
        if (first_in == true) {
            if (keynum != 0) {
                if (keynum == Mid) {
                    first_in = true;
                    __Menu_Clear();
                    goto out;
                } else {
                    first_in = false;
                    index = 1;
                    __Menu_Channel_SetShowIndex(index);
                    Delay_ms(20);
                }
            }
        } else {
            if (keynum != 0) {
                switch (keynum)
                {
                case Left:      //左移光标
                    if (index > 1) {
                        index--;
                        __Menu_Channel_SetShowIndex(index);
                        Delay_ms(20);
                    }
                    break;
                case Right:     //右移光标
                    if (index < 4) {
                        index++;
                        __Menu_Channel_SetShowIndex(index);
                        Delay_ms(20);
                    }
                    break;
                case Up:        //反转通道
                    if (!Channel_Res[index - 1]) {
                        Channel_Res[index - 1] = true;
                        Switch_Channel_Res_Show();
                    }
                    break;
                case Down:      //反转通道
                    if (Channel_Res[index - 1]) {
                        Channel_Res[index - 1] = false;
                        Switch_Channel_Res_Show();
                    }
                    break;
                default:
                    __Menu_Clear();
                    goto out;
                }
            }
        }
    }
    out:
    return;   
}

void Menu_Balance_Set(void)
{
    __Menu_Clear();
    OLED_ShowString(1, 12, "Pitch", OLED_6X8);
    OLED_ShowString(1, 22, "Roll", OLED_6X8);
    OLED_ShowString(1, 32, "Yaw", OLED_6X8);
    OLED_ShowString(1, 42, "Back", OLED_6X8);
    OLED_ReverseArea(1, 12, 127, 8);
    OLED_Update();
    while (1) {
        static uint8_t index = 1;
        Delay_ms(5);
        if (keynum != None) {
            switch (keynum)
            {
            case Up:
                if (index > 1) {
                    index--;
                    __Menu_One_ShowIndex(index, 1);
                }
                break;
            case Down:
                if (index < 4) {
                    index++;
                    __Menu_One_ShowIndex(index, 1);
                }
                break;
            case Left:

                break;
            case Right:

                break;
            case Mid:
                if (index == 1) {
                    
                } else if (index == 4) {
                    index = 1;
                    __Menu_One_ShowIndex(index, 0);
                    __Menu_Clear();
                    goto out;
                }
                break;
            default:
                break;
            }
        }
    }
    out:
    return;
}


