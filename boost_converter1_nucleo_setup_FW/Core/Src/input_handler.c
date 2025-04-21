/*
 * input_handler.c
 *
 *  Created on: Apr 10, 2025
 *      Author: ihsan
 */

/* INCLUDES */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Project includes */
#include "config.h"
#include "input_handler.h"
#include "use_printf.h"
#include "pwm.h"
#include "user_commands.h"

/* PV */

/* PRIVATE FUNCTION DECLARATIONS */

uint8_t* const _rbuff_p_vals(const ring_buffer_t *const p_rbuff);
const uint8_t _rbuff_idx(const ring_buffer_t *const p_rbuff);
const bool _rbuff_busy(const ring_buffer_t *const p_rbuff);

command_t _rbuff_parse(const ring_buffer_t *const p_rbuff);

/* API */
uint8_t* const _rbuff_p_vals(const ring_buffer_t *const p_rbuff) {
	return (uint8_t* const ) p_rbuff->_p_vals;
}
const uint8_t _rbuff_idx(const ring_buffer_t *const p_rbuff) {
	return (const uint8_t) p_rbuff->_idx;
}
const bool _rbuff_busy(const ring_buffer_t *const p_rbuff) {
	return (const bool) p_rbuff->_busy;
}

/* INIT */
void rbuff_init(ring_buffer_t *const p_rbuff) {

	p_rbuff->_busy = true;
	p_rbuff->_idx = 0;

	for (int idx = 0; idx < INPUT_BUFFER_SIZE; idx++) {
		p_rbuff->_p_vals[idx] = '\0';
	}

	p_rbuff->_busy = false;
}

/* USE */
/* Add a value to the buffer */
command_t rbuff_add_val(ring_buffer_t *const p_rbuff, uint8_t val) {

	if (debug_level() >= 2) {
		printf("[DBG] In rbuff_add_val()...\r\n");
	}

	if (_rbuff_busy(p_rbuff)) {
		if (debug_level() >= 2) {
			printf("[DBG] \tBuff busy - return.\r\n");
		}
		return BUFF_BUSY;
	}

	p_rbuff->_busy = true;

	//if buffer is full, wrap around
	if (_rbuff_idx(p_rbuff) == INPUT_BUFFER_SIZE) {
		p_rbuff->_idx = 0;
	}

	//Add value to the buffer
	_rbuff_p_vals(p_rbuff)[_rbuff_idx(p_rbuff)] = val;

	//handle deletes
	if (val == 8 || val == 127) {
		p_rbuff->_idx -= 2;
	}

	//check if it is time to parse
	if (val == '\r' || val == '\n') {

		if (_rbuff_idx(p_rbuff) <= 1) {
			printf("\r\n");  //newline
			rbuff_init(p_rbuff); //re-init the buffer
			return NO_COMMAND;
		}

		if (debug_level() >= 2) {
			printf("[DBG] \tAbout to parse...\r\n");
		}

		printf("\r\n"); //print a newline
		command_t ret_val = _rbuff_parse(p_rbuff);
		rbuff_init(p_rbuff);
		p_rbuff->_busy = false;
		return ret_val;
	}

	p_rbuff->_idx++; //increment buffer index

	p_rbuff->_busy = false; //update buffer avail

	return NO_COMMAND;

}

/* Parse */
command_t _rbuff_parse(const ring_buffer_t *const p_rbuff) {

	if (debug_level() >= 2) {
		printf("[DBG] In _rbuff_parse()...\r\n");
	}

	const char p_delims[4] = { '\r', ' ', '\0', '\n' };
	const char *const p_cmd = strtok((char*) _rbuff_p_vals(p_rbuff), p_delims);

	if (p_cmd == NULL) {
		return NO_COMMAND;
	}

	if (debug_level() >= 2) {
		printf("[DBG] \tCmd is %s\r\n", p_cmd);
	}

	if (!strcmp(p_cmd, "ack")) {
		printf("[INFO] Ack!\r\n");
		return ACK;
	}

	else if (!strcmp(p_cmd, "pwm")) {
		const char *const p_cmd_2 = strtok(NULL, p_delims);
		if (debug_level() >= 2) {
			printf("[DBG] \tIn PWM commands.\r\n");
		}

		if (p_cmd_2 == NULL) {
			if (debug_level() >= 2) {
				printf("[DBG] \tNo second command.\r\n");
			}
			return NO_COMMAND;
		}

		if (!strcmp(p_cmd_2, "dc")) {

			const char *const p_cmd_3 = strtok(NULL, p_delims);

			if (debug_level() >= 2) {
				printf("[DBG] \tIn duty cycle commands.\r\n");
			}

			if (p_cmd_3 == NULL) {
				if (debug_level() >= 2) {
					printf("[DBG] \tNo duty cycle provided.\r\n");
				}
				printf("[ERR] Invalid command. No duty cycle provided.\r\n");
				return NO_COMMAND;
			}

			const float dc_percent = atof(p_cmd_3);
			const float dc = dc_percent / 100;

			return pwm_set_dc(&pwm_boost, dc);

		} else if (!strcmp(p_cmd_2, "en")) {
			return pwm_en(&pwm_boost);
		} else if (!strcmp(p_cmd_2, "dis")) {
			return pwm_dis(&pwm_boost);
		}
	}

	printf("[ERR] Invalid command.\r\n");
	return INVALID_COMMAND;

}
