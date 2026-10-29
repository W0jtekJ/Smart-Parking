/*
 * barrier.h
 *
 *  Created on: 29 paź 2026
 *      Author: wojus
 */

#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

#include "main.h"

#define BARRIER_CLOSED_PULSE 1000
#define BARRIER_OPEN_PULSE 2000
#define BARRIER_STEP_DELAY_MS 15

void Barrier_Init(TIM_HandleTypeDef *htim, uint32_t Channel);
void Barrier_StartOpening(void);
void Barrier_StartClosing(void);
void Barrier_Update(void);

uint8_t Barrier_IsClosed(void);

#endif
