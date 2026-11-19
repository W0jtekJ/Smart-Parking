/*
 * virtual_clock.c
 *
 *  Created on: 19 lis 2026
 *      Author: wojus
 */


#include "virtual_clock.h"
#include "stm32f0xx_hal.h"

static VirtualClock_t vclock;
static uint32_t last_tick_ms = 0;


static void VClock_RecalculateFields(void)
{
    uint32_t minute_of_day = vclock.total_minutes % VCLOCK_MINS_PER_DAY;
    vclock.hours   = (uint8_t)(minute_of_day / 60U);
    vclock.minutes = (uint8_t)(minute_of_day % 60U);
    vclock.is_day_tariff = VClock_IsDayMinute(minute_of_day);
}

void VClock_Init(uint8_t start_hour, uint8_t start_minute)
{

    start_hour   %= 24U;
    start_minute %= 60U;

    vclock.total_minutes = ((uint32_t)start_hour * 60U) + (uint32_t)start_minute;
    last_tick_ms = HAL_GetTick();

    VClock_RecalculateFields();
}

void VClock_Update(void)
{
    uint32_t now = HAL_GetTick();
    uint32_t elapsed_ms = now - last_tick_ms;


    if (elapsed_ms >= VCLOCK_MS_PER_VIRTUAL_MIN) {
        uint32_t mins_to_add = elapsed_ms / VCLOCK_MS_PER_VIRTUAL_MIN;

        vclock.total_minutes += mins_to_add;

        last_tick_ms += mins_to_add * VCLOCK_MS_PER_VIRTUAL_MIN;

        VClock_RecalculateFields();
    }
}

VirtualClock_t VClock_GetTime(void)
{
    return vclock;
}

bool VClock_IsDayMinute(uint32_t minute_of_day)
{
    minute_of_day %= VCLOCK_MINS_PER_DAY;
    uint32_t start_min = DAY_TARIFF_START_HOUR * 60U;
    uint32_t end_min   = DAY_TARIFF_END_HOUR * 60U;

    return (minute_of_day >= start_min && minute_of_day < end_min);
}
