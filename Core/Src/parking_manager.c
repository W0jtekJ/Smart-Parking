/*
 * parking_manager.c
 *
 *  Created on: 19 lis 2026
 *      Author: wojus
 */

#include "parking_manager.h"
#include "virtual_clock.h"
#include <stddef.h>

static ParkingSlot_t slots[PARKING_TOTAL_SLOTS];
static uint32_t next_vehicle_id = 1001U;

void Parking_Init(void)
{
    for (uint8_t i = 0; i < PARKING_TOTAL_SLOTS; i++) {
        slots[i].slot_id        = i + 1U;
        slots[i].is_occupied    = false;
        slots[i].vehicle_id     = 0U;
        slots[i].entry_time_min = 0U;
    }
    next_vehicle_id = 1001U;
}

uint8_t Parking_VehicleEnter(uint32_t current_time_min, uint32_t *assigned_vehicle_id)
{
    for (uint8_t i = 0; i < PARKING_TOTAL_SLOTS; i++) {
        if (!slots[i].is_occupied) {
            slots[i].is_occupied    = true;
            slots[i].vehicle_id     = next_vehicle_id++;
            slots[i].entry_time_min = current_time_min;

            if (assigned_vehicle_id != NULL) {
                *assigned_vehicle_id = slots[i].vehicle_id;
            }
            return slots[i].slot_id;
        }
    }
    return 0U;
}

ParkingReceipt_t Parking_CalculateFee(uint8_t slot_id, uint32_t current_time_min)
{
    ParkingReceipt_t receipt = {0};

    if (slot_id == 0U || slot_id > PARKING_TOTAL_SLOTS) {
        return receipt;
    }

    const ParkingSlot_t *slot = &slots[slot_id - 1U];
    if (!slot->is_occupied) {
        return receipt;
    }

    receipt.vehicle_id = slot->vehicle_id;
    receipt.slot_id    = slot->slot_id;

    if (current_time_min <= slot->entry_time_min) {
        return receipt;
    }

    uint32_t duration = current_time_min - slot->entry_time_min;
    receipt.total_minutes = duration;


    for (uint32_t m = slot->entry_time_min; m < current_time_min; m++) {
        if (VClock_IsDayMinute(m)) {
            receipt.day_minutes++;
        } else {
            receipt.night_minutes++;
        }
    }


    uint32_t day_fee   = (receipt.day_minutes   * TARIFF_DAY_RATE_PER_HOUR_GR)   / 60U;
    uint32_t night_fee = (receipt.night_minutes * TARIFF_NIGHT_RATE_PER_HOUR_GR) / 60U;

    receipt.total_fee_gr = day_fee + night_fee;
    return receipt;
}

bool Parking_VehicleExit(uint8_t slot_id, uint32_t current_time_min, ParkingReceipt_t *receipt)
{
    if (slot_id == 0U || slot_id > PARKING_TOTAL_SLOTS) {
        return false;
    }

    ParkingSlot_t *slot = &slots[slot_id - 1U];
    if (!slot->is_occupied) {
        return false;
    }

    if (receipt != NULL) {
        *receipt = Parking_CalculateFee(slot_id, current_time_min);
    }


    slot->is_occupied    = false;
    slot->vehicle_id     = 0U;
    slot->entry_time_min = 0U;

    return true;
}

uint8_t Parking_GetFreeSlotsCount(void)
{
    uint8_t free_count = 0U;
    for (uint8_t i = 0; i < PARKING_TOTAL_SLOTS; i++) {
        if (!slots[i].is_occupied) {
            free_count++;
        }
    }
    return free_count;
}

bool Parking_IsFull(void)
{
    return (Parking_GetFreeSlotsCount() == 0U);
}

const ParkingSlot_t* Parking_GetSlotInfo(uint8_t slot_id)
{
    if (slot_id == 0U || slot_id > PARKING_TOTAL_SLOTS) {
        return NULL;
    }
    return &slots[slot_id - 1U];
}
