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
    // Hardware reset peryferium I2C1 (zabezpieczenie przed Bus Hang)
    __HAL_RCC_I2C1_FORCE_RESET();
    HAL_Delay(10);
    __HAL_RCC_I2C1_RELEASE_RESET();
    HAL_Delay(10);

    HAL_I2C_Init(&hi2c1);

    SSD1306_Init();
    SSD1306_Clear();
    SSD1306_UpdateScreen();
}

void Display_Update(uint32_t distance_cm,
                    bool is_closed,
                    VirtualClock_t clock_time,
                    uint8_t free_slots,
                    uint8_t total_slots,
                    const ParkingReceipt_t *last_receipt)
{
    if (HAL_GetTick() - last_update_time < 150) {
        return;
    }
    last_update_time = HAL_GetTick();

    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
        return;
    }

    char buf[32];

    // Linia 1 (y=0): Wirtualny zegar + aktywna taryfa
    SSD1306_GotoXY(0, 0);
    sprintf(buf, "%02u:%02u  TARYFA:%s ",
            clock_time.hours,
            clock_time.minutes,
            clock_time.is_day_tariff ? "DZIEN" : "NOC  ");
    SSD1306_Puts(buf, &Font_7x10, 1);

    // Linia 2 (y=17): Stan szlabanu
    SSD1306_GotoXY(0, 17);
    if (is_closed) {
        SSD1306_Puts("SZLABAN: ZAMKNIETY", &Font_7x10, 1);
    } else {
        SSD1306_Puts("SZLABAN: OTWARTY  ", &Font_7x10, 1);
    }


    SSD1306_GotoXY(0, 34);
    if (distance_cm == 0) {
        sprintf(buf, "WOLNE:%u/%u  D:---cm", free_slots, total_slots);
    } else {
        sprintf(buf, "WOLNE:%u/%u  D:%3lucm", free_slots, total_slots, (unsigned long)distance_cm);
    }
    SSD1306_Puts(buf, &Font_7x10, 1);


    SSD1306_GotoXY(0, 51);
    if (last_receipt != NULL && last_receipt->total_minutes > 0) {
        uint32_t zl = last_receipt->total_fee_gr / 100U;
        uint32_t gr = last_receipt->total_fee_gr % 100U;
        sprintf(buf, "#%lu %lum %lu.%02luPLN ",
                (unsigned long)last_receipt->vehicle_id,
                (unsigned long)last_receipt->total_minutes,
                (unsigned long)zl,
                (unsigned long)gr);
        SSD1306_Puts(buf, &Font_7x10, 1);
    } else {
        SSD1306_Puts("OPLATA: BRAK DANYCH", &Font_7x10, 1);
    }

    SSD1306_UpdateScreen();
}
