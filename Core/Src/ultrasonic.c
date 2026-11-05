/*
 * ultrasonic.c
 *
 *  Created on: 2 lis 2026
 *      Author: wojus
 */


#include "ultrasonic.h"

void Ultrasonic_Init(Ultrasonic_t *dev, TIM_HandleTypeDef *htim,
                     GPIO_TypeDef *trig_port, uint16_t trig_pin,
                     GPIO_TypeDef *echo_port, uint16_t echo_pin)
{
    dev->htim = htim;
    dev->trig_port = trig_port;
    dev->trig_pin = trig_pin;
    dev->echo_port = echo_port;
    dev->echo_pin = echo_pin;
    dev->last_measurement_time = 0;
    dev->distance_cm = 0;

    // Upewniamy się, że TRIG jest w stanie niskim przy starcie
    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_RESET);
}

void Ultrasonic_Update(Ultrasonic_t *dev)
{
    uint32_t now = HAL_GetTick();

    // Wykonujemy pomiar nie częściej niż co 60 ms, aby uniknąć nakładania się ech
    if (now - dev->last_measurement_time < 60) {
        return;
    }
    dev->last_measurement_time = now;

    // 1. Wysłanie impulsu TRIG (~10-12 us)
    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_SET);
    for (volatile int i = 0; i < 300; i++);
    HAL_GPIO_WritePin(dev->trig_port, dev->trig_pin, GPIO_PIN_RESET);

    // 2. Oczekiwanie na pojawienie się sygnału
    uint32_t timeout = 10000;
    while (HAL_GPIO_ReadPin(dev->echo_port, dev->echo_pin) == GPIO_PIN_RESET && --timeout);
    if (timeout == 0) return;

    // Start pomiaru czasu
    __HAL_TIM_SET_COUNTER(dev->htim, 0);
    HAL_TIM_Base_Start(dev->htim);

    // 3. Oczekiwanie na koniec sygnału
    timeout = 65000;
    while (HAL_GPIO_ReadPin(dev->echo_port, dev->echo_pin) == GPIO_PIN_SET && --timeout);

    uint32_t echo_time = __HAL_TIM_GET_COUNTER(dev->htim);
    HAL_TIM_Base_Stop(dev->htim);

    if (timeout == 0) return;

    // 4. Przeliczenie na centymetry i filtracja błędów
    uint32_t raw_dist = echo_time / 58;
    if (raw_dist >= 2 && raw_dist <= 200) {
        dev->distance_cm = raw_dist;
    }
}

uint32_t Ultrasonic_GetDistance(Ultrasonic_t *dev)
{
    return dev->distance_cm;
}
