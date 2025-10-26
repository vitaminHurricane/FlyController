#ifndef __KEY_H
#define __KEY_H
#include "main.h"
#include <stdbool.h>

#define K1_PORT     GPIOA
#define K2_PORT     GPIOA
#define K3_PORT     GPIOB
#define K4_PORT     GPIOB
#define K5_PORT     GPIOB
#define K6_PORT     GPIOB
#define K7_PORT     GPIOA
#define K8_PORT     GPIOA
#define K9_PORT     GPIOB
#define K10_PORT    GPIOB

#define K1_PIN      GPIO_PIN_6
#define K2_PIN      GPIO_PIN_7
#define K3_PIN      GPIO_PIN_0
#define K4_PIN      GPIO_PIN_1
#define K5_PIN      GPIO_PIN_2
#define K6_PIN      GPIO_PIN_10
#define K7_PIN      GPIO_PIN_11
#define K8_PIN      GPIO_PIN_12
#define K9_PIN      GPIO_PIN_5
#define K10_PIN     GPIO_PIN_6

//我们设置Key1，7为一组，Key3，10为一组，Key2，8为一组，Key4，9为一组
typedef enum{
    None,
    Left, Right, Up, Down,
    Mid,
}Key_Side;

extern uint8_t push_time;

/***********************private************************/
uint8_t __Key_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/******************************************************/

/***********************public*************************/
uint8_t Key_GetState(void);
/******************************************************/

#endif
