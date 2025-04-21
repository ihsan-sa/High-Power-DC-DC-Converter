/*
 * config.h
 *
 *  Created on: Apr 10, 2025
 *      Author: ihsan
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

/* INCLUDES */
#include <stdint.h>

/* MACROS */
#define INPUT_BUFFER_SIZE 255

#define PWM_MAX_DC 0.9f
#define PWM_MIN_DC 0.1f

uint8_t debug_level(void);


#endif /* SRC_CONFIG_H_ */
