/*
 * virtual_clock.h
 *
 *  Created on: 19 lis 2026
 *      Author: wojus
 */

#ifndef INC_VIRTUAL_CLOCK_H_
#define INC_VIRTUAL_CLOCK_H_

#include <stdint.h>
#include <stdbool.h>

#define VCLOCK_MS_PER_VIRTUAL_MIN   200U
#define VCLOCK_MINS_PER_DAY         1440U

#define DAY_TARIFF_START_HOUR       6U
#define DAY_TARIFF_END_HOUR         22U

typedef struct {
    uint8_t  hours;
    uint8_t  minutes;
    uint32_t total_minutes;
    bool     is_day_tariff;
} VirtualClock_t;

void VClock_Init(uint8_t start_hour, uint8_t start_minute);
void VClock_Update(void);
VirtualClock_t VClock_GetTime(void);
bool VClock_IsDayMinute(uint32_t minute_of_day);




#endif /* INC_VIRTUAL_CLOCK_H_ */
