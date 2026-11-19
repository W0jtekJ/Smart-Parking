/*
 * parking_manager.h
 *
 *  Created on: 19 lis 2026
 *      Author: wojus
 */

#ifndef INC_PARKING_MANAGER_H_
#define INC_PARKING_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>


#define PARKING_TOTAL_SLOTS         4U

// Stawki taryfowe w groszach za 1 wirtualną godzinę (60 minut)
// Dzień (06:00 - 22:00): 6.00 PLN/h = 600 gr/h (czyli 10 gr/min)
// Noc   (22:00 - 06:00): 3.00 PLN/h = 300 gr/h (czyli 5 gr/min)
#define TARIFF_DAY_RATE_PER_HOUR_GR     600U
#define TARIFF_NIGHT_RATE_PER_HOUR_GR   300U


typedef struct {
    uint8_t  slot_id;
    bool     is_occupied;
    uint32_t vehicle_id;
    uint32_t entry_time_min;
} ParkingSlot_t;


typedef struct {
    uint32_t vehicle_id;
    uint8_t  slot_id;
    uint32_t total_minutes;
    uint32_t day_minutes;
    uint32_t night_minutes;
    uint32_t total_fee_gr;
} ParkingReceipt_t;


void Parking_Init(void);



uint8_t Parking_VehicleEnter(uint32_t current_time_min, uint32_t *assigned_vehicle_id);



bool Parking_VehicleExit(uint8_t slot_id, uint32_t current_time_min, ParkingReceipt_t *receipt);


ParkingReceipt_t Parking_CalculateFee(uint8_t slot_id, uint32_t current_time_min);


uint8_t Parking_GetFreeSlotsCount(void);
bool    Parking_IsFull(void);
const ParkingSlot_t* Parking_GetSlotInfo(uint8_t slot_id);



#endif /* INC_PARKING_MANAGER_H_ */
