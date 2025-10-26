#ifndef __MENU_H
#define __MENU_H
#include "main.h"
#include <stdbool.h>

extern bool BIND_Flag;

/**********************private***********************/
void __Menu_Home_SETTING(void);
void __Menu_Home_BIND(void);
uint8_t __Menu_BIND_Searching(void);
void __Menu_Channel_TestShowIndex(uint8_t index);
/****************************************************/

/*********************public*************************/
void Menu_Start(void);
uint8_t Menu_Home(void);
uint8_t Menu_One(void);
void Menu_BIND(void);
void Menu_Channel_Test(void);
void Menu_Channel_Set(void);
void Menu_Balance_Set(void);
/****************************************************/


#endif

