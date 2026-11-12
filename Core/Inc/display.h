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

void Display_Init(void);
void Display_Update(uint32_t distance_cm, bool is_closed);




#endif /* INC_DISPLAY_H_ */
