/*
 * pwm.c
 *
 *  Created on: Apr 10, 2025
 *      Author: ihsan
 */

/* INCLUDES */
#include <stdlib.h>
#include <stdio.h>

/* Project includes */
#include "config.h"
#include "main.h"
#include "pwm.h"
#include "main.h"
#include "use_printf.h"
#include "user_commands.h"


/* INIT */
void pwm_init(pwm_t *const p_pwm, char id[PWM_INSTANCE_ID_LEN],
		control_mode_t control_mode, const TIM_HandleTypeDef *const p_timer) {
	p_pwm->_is_en = false;
	p_pwm->_dc = DFLT_PWM_DC;
	p_pwm->_control_mode = control_mode;
	p_pwm->_p_timer = (TIM_HandleTypeDef *)p_timer;

	for (int i = 0; i < PWM_INSTANCE_ID_LEN; i++) {
		p_pwm->_id[i] = id[i];
	}
}

/* PWM CONROL */
command_t pwm_en(pwm_t *const p_pwm) {
	if (p_pwm == NULL) {
		return NO_COMMAND;
	}

	else if (p_pwm->_control_mode == ASYNC_BOOST) {
		HAL_TIM_PWM_Start(p_pwm->_p_timer, TIM_CHANNEL_1);
		printf("[INFO] %s PWM enabled.\r\n", p_pwm->_id);
	}

	return PWM_EN;
}

command_t pwm_dis(pwm_t *const p_pwm) {
	if (p_pwm == NULL) {
		return INVALID_COMMAND;
	}

	else if (p_pwm->_control_mode == ASYNC_BOOST) {
		HAL_TIM_PWM_Stop(p_pwm->_p_timer, TIM_CHANNEL_1);
		printf("[INFO] %s PWM disabled.\r\n", p_pwm->_id);
	}

	return PWM_DIS;
}

command_t pwm_set_dc(pwm_t *const p_pwm, float dc) {
	if (p_pwm == NULL) {
		return INVALID_COMMAND;
	}
	if (dc < PWM_MIN_DC || dc > PWM_MAX_DC) {
		printf(
				"[ERR] Duty cycle %1.1f%% is not within the allowed range of %1.1f%% to %1.1f%%.",
				dc * 100, PWM_MIN_DC * 100, PWM_MAX_DC * 100);
		return INVALID_COMMAND;
	}

	p_pwm->_p_timer->Instance->CCR1 = DFLT_CNT_PER * dc;
	p_pwm->_dc = dc;

	printf("[INFO] %s PWM duty cycle set to %1.1f%% (%lu).\r\n", p_pwm->_id, dc * 100, p_pwm->_p_timer->Instance->CCR1);

	return PWM_DC_SET;

}
