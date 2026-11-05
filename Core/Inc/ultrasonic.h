/*
 * ultrasonic.h
 *
 *  Created on: 2 lis 2026
 *      Author: wojus
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "stm32f0xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;      // Uchwyt do timera (np. &htim2)
    GPIO_TypeDef *trig_port;       // Port GPIO pinu TRIG
    uint16_t trig_pin;             // Pin TRIG
    GPIO_TypeDef *echo_port;       // Port GPIO pinu ECHO
    uint16_t echo_pin;             // Pin ECHO

    uint32_t last_measurement_time; // Czas ostatniego pomiaru
    uint32_t distance_cm;           // Zmierzona odległość w cm
} Ultrasonic_t;

// Inicjalizacja struktury czujnika
void Ultrasonic_Init(Ultrasonic_t *dev, TIM_HandleTypeDef *htim,
                     GPIO_TypeDef *trig_port, uint16_t trig_pin,
                     GPIO_TypeDef *echo_port, uint16_t echo_pin);

// Bezblokujące wyzwolenie pomiaru i przeliczenie dystansu
void Ultrasonic_Update(Ultrasonic_t *dev);

// Odczyt aktualnego zmierzonego dystansu (w cm) z filtracją
uint32_t Ultrasonic_GetDistance(Ultrasonic_t *dev);

#endif /* INC_ULTRASONIC_H_ */
