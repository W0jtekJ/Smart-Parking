/*
 * display.h
 *
 *  Created on: 9 lis 2026
 *      Author: wojus
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "virtual_clock.h"
#include "parking_manager.h"

void Display_Init(void);

void Display_Update(uint32_t distance_cm,
                    bool is_closed,
                    VirtualClock_t clock_time,
                    uint8_t free_slots,
                    uint8_t total_slots,
                    const ParkingReceipt_t *last_receipt);

#endif /* INC_DISPLAY_H_ */
