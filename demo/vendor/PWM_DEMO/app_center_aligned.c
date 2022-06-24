/********************************************************************************************************
 * @file	app_center_aligned.c
 *
 * @brief	This is the source file for B91m
 *
 * @author	Driver Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "app_config.h"
#if(SET_PWM_MODE==PWM_CENTER_ALIGNED)
#define PWM_ID		PWM1_ID
#define PWM_PIN		GPIO_PB4
#define PWM_FUNC    PWM1
#define PWM_ID1		PWM2_ID
#define PWM_PIN1	GPIO_PB5
#define PWM_FUNC1   PWM2


#define ALIGNED_NEGATION      1
#define DELAY                 2
#define ALIGNED_MODE          1

void user_init(void)
{
	gpio_function_en(LED2|LED3|LED4);

	gpio_output_en(LED2|LED3|LED4);

	gpio_input_dis(LED2|LED3|LED4);

	pwm_set_pin(PWM_PIN,PWM_FUNC);
	pwm_set_pin(PWM_PIN1,PWM_FUNC1);

	pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));

	pwm_set_tcmp(PWM_ID1,40 * CLOCK_PWM_CLOCK_1US);

	pwm_set_tmax(PWM_ID1,50 * CLOCK_PWM_CLOCK_1US);
#if(ALIGNED_MODE==ALIGNED_NEGATION )
	pwm_set_tcmp(PWM_ID,10 * CLOCK_PWM_CLOCK_1US);

	pwm_set_tmax(PWM_ID,50 * CLOCK_PWM_CLOCK_1US);
    //aligned
	pwm_set_align_en(PWM_ID1);
	pwm_set_align_en(PWM_ID);
	//negation
	pwm_set_polarity_en(PWM_ID1);
	pwm_set_polarity_en(PWM_ID);


#elif(ALIGNED_MODE==DELAY)
	pwm_set_tcmp(PWM_ID,40 * CLOCK_PWM_CLOCK_1US);

	pwm_set_tmax(PWM_ID,50 * CLOCK_PWM_CLOCK_1US);

	pwm_set_shift_time(PWM_ID1,PWM_PCLK_SPEED*(50 * CLOCK_PWM_CLOCK_1US/2));


#endif

   pwm_start(FLD_PWM1_EN|FLD_PWM2_EN);

}

void main_loop(void)
{
	delay_ms(500);

	gpio_toggle(LED1);
}

#endif

