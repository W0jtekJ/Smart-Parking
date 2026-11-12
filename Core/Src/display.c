/*
 * display.c
 *
 *  Created on: 9 lis 2026
 *      Author: wojus
 */


#include "display.h"
#include "ssd1306.h"
#include "fonts.h"
#include "main.h"
#include "stm32f0xx_hal.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;
static uint32_t last_update_time = 0;

void Display_Init(void)
{

    __HAL_RCC_I2C1_FORCE_RESET();
    HAL_Delay(10);
    __HAL_RCC_I2C1_RELEASE_RESET();
    HAL_Delay(10);

    HAL_I2C_Init(&hi2c1);

    SSD1306_Init();
    SSD1306_Clear();


    SSD1306_GotoXY(0, 0);
    SSD1306_Puts("--SMART  PARKING--", &Font_7x10, 1);
    SSD1306_UpdateScreen();
}

void Display_Update(uint32_t distance_cm, bool is_closed)
{
    if (HAL_GetTick() - last_update_time < 150) {
        return;
    }
    last_update_time = HAL_GetTick();

    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
        return;
    }

    char buf[32];


    SSD1306_GotoXY(0, 22);
    if (is_closed) {
        SSD1306_Puts("SZLABAN: ZAMKNIETY ", &Font_7x10, 1);
    } else {
        SSD1306_Puts("SZLABAN: OTWARTY   ", &Font_7x10, 1);
    }


    SSD1306_GotoXY(0, 44);
    if (distance_cm == 0) {
        SSD1306_Puts("DYSTANS: --- cm   ", &Font_7x10, 1);
    } else {
        sprintf(buf, "DYSTANS: %-3lu cm   ", (unsigned long)distance_cm);
        SSD1306_Puts(buf, &Font_7x10, 1);
    }

    SSD1306_UpdateScreen();
}
