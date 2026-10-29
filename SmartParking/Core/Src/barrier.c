/*
 * barrier.c
 *
 *  Created on: 29 paź 2026
 *      Author: wojus
 */

#include "barrier.h"

static TIM_HandleTypeDef *barrier_htim;
static uint32_t barrier_channel;

static uint16_t current_pulse = BARRIER_CLOSED_PULSE;
static uint16_t target_pulse = BARRIER_CLOSED_PULSE;
static uint32_t last_step_time = 0;

void Barrier_Init(TIM_HandleTypeDef *htim, uint32_t channel){
	barrier_htim = htim;
	barrier_channel = channel;

	HAL_TIM_PWM_Start(barrier_htim, barrier_channel);
	__HAL_TIM_SET_COMPARE(barrier_htim, barrier_channel, current_pulse);
}

void Barrier_StartOpening(void){
	target_pulse = BARRIER_OPEN_PULSE;
}

void Barrier_StartClosing(void){
	target_pulse = BARRIER_CLOSED_PULSE;
}

void Barrier_Update(void) {
	if (current_pulse == target_pulse) {
	        return;
	    }

	    if ((HAL_GetTick() - last_step_time) >= BARRIER_STEP_DELAY_MS) {
	        last_step_time = HAL_GetTick();

	        if (current_pulse < target_pulse) {
	            current_pulse += 10;
	        }
	        else if (current_pulse > target_pulse) {
	            current_pulse -= 10;
	        }

	        __HAL_TIM_SET_COMPARE(barrier_htim, barrier_channel, current_pulse);
	    }
	}

uint8_t Barrier_IsClosed(void){
	return (target_pulse == BARRIER_CLOSED_PULSE);
}

