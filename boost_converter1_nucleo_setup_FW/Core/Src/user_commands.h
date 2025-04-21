/*
 * user_commands.h
 *
 *  Created on: Apr 11, 2025
 *      Author: ihsan
 */

#ifndef SRC_USER_COMMANDS_H_
#define SRC_USER_COMMANDS_H_

/* AVAILABLE COMMANDS */
typedef enum {
	BUFF_BUSY = -2, INVALID_COMMAND = -1, NO_COMMAND = 0,
	ACK = 1,

	//PWM
	PWM_EN,
	PWM_DIS,
	PWM_DC_SET,

} command_t;

#endif /* SRC_USER_COMMANDS_H_ */
