/*
 * input_handler.h
 *
 *  Created on: Apr 10, 2025
 *      Author: ihsan
 */

#ifndef SRC_INPUT_HANDLER_H_
#define SRC_INPUT_HANDLER_H_

/* INCLUDES */
#include <stdbool.h>

/* Project includes */
#include "config.h"
#include "pwm.h"
#include "user_commands.h"

/* PV */
extern pwm_t pwm_boost;

/* RING BUFFER */

typedef struct {
	uint8_t _p_vals[INPUT_BUFFER_SIZE];
	bool _busy;
	uint8_t _idx;
} ring_buffer_t;

/* INIT */
void rbuff_init(ring_buffer_t *const p_rbuff);

/* USE */
/* Add a value to the buffer */
command_t rbuff_add_val(ring_buffer_t *const p_rbuff, uint8_t val);

#endif /* SRC_INPUT_HANDLER_H_ */
