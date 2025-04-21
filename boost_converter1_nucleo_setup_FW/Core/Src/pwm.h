/*
 * pwm.h
 *
 *  Created on: Apr 10, 2025
 *      Author: ihsan
 */

#ifndef SRC_PWM_H_
#define SRC_PWM_H_

/* INCLUDES */
#include <stdbool.h>

/* Project includes */
#include "main.h"
#include "user_commands.h"


/* EV */
//extern typedef enum command_t;

/* MACROS */
#define PWM_INSTANCE_ID_LEN 16

/* CONTROL MODE ENUM */
typedef enum {
	ASYNC_BOOST,
} control_mode_t;

/* PWM STRUCT */
typedef struct {
	float _dc;
	control_mode_t _control_mode;
	TIM_HandleTypeDef *_p_timer;
	char _id[PWM_INSTANCE_ID_LEN];
	bool _is_en;
} pwm_t;

/* INIT */
void pwm_init(pwm_t *const p_pwm, char id[PWM_INSTANCE_ID_LEN],
		control_mode_t control_mode, const TIM_HandleTypeDef *const p_timer);
/* PWM CONTROL */

command_t pwm_en(pwm_t *const p_pwm);
command_t pwm_dis(pwm_t *const p_pwm);

command_t pwm_set_dc(pwm_t *const p_pwm, float dc);

#endif /* SRC_PWM_H_ */
