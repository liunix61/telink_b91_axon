/********************************************************************************************************
 * @file	app_pwm_continue.c
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
#if(SET_PWM_MODE==PWM_CONTINUE)
#if(MCU_CORE_B91)
#define PWM_PIN		(PWM_PWM0_PB4)
#define PWM_ID		(get_pwmid(PWM_PIN))
#elif(MCU_CORE_B92)
#define PWM_ID		PWM0_ID
#define PWM_PIN		GPIO_PB4
#define PWM_FUNC    PWM0
#endif
/*
 *  pwm_clk_source is pclk or 32K
 */
#define	 PWM_PCLK		       1
#define	 PWM_32K  	           2       //If want to work properly in suspend mode (the wake source:32K_rc/32k_crystal), can set the PWM to use the 32K clock source.
#define  PWM_CLK             PWM_PCLK



_attribute_ram_code_sec_ void pwm_irq_handler(void)
{
	if(pwm_get_irq_status(FLD_PWM0_FRAME_DONE_IRQ  ))
	{
	  pwm_clr_irq_status(FLD_PWM0_FRAME_DONE_IRQ );

	  gpio_toggle(LED4);
	}
}

void user_init(void)
{
	gpio_function_en(LED2|LED3|LED4);

	gpio_output_en(LED2|LED3|LED4);

	gpio_input_dis(LED2|LED3|LED4);
#if(MCU_CORE_B91)
	pwm_set_pin(PWM_PIN);
#elif(MCU_CORE_B92)
	pwm_set_pin(PWM_PIN,PWM_FUNC);
#endif

#if(!((PWM_CLK  == PWM_32K)&&(MCU_CORE_B91)))
    //In eagle count mode,using 32k clock source, PWM_FRAME_DONE_IRQ interrupt have problem,not Recommended.
    //In B92, the issue has been fixed.
    pwm_set_irq_mask(FLD_PWM0_FRAME_DONE_IRQ);

	pwm_clr_irq_status(FLD_PWM0_FRAME_DONE_IRQ);

	core_interrupt_enable();

    plic_interrupt_enable(IRQ16_PWM);
#endif

    pwm_set_pwm0_mode(PWM_NORMAL_MODE);

#if (PWM_CLK  == PWM_PCLK)

	pwm_set_clk((unsigned char) (sys_clk.pclk*1000*1000/PWM_PCLK_SPEED-1));

    pwm_set_tcmp(PWM_ID,50 * CLOCK_PWM_CLOCK_1US);

    pwm_set_tmax(PWM_ID,100 * CLOCK_PWM_CLOCK_1US);

#elif(PWM_CLK  == PWM_32K)
	
    //there are two 32K clock sources, 32K_RC and 32K_Crystal.
	//if want higher 32K clock source accuracy, need to calibrate it.
    clock_32k_init(CLK_32K_RC);

	clock_cal_32k_rc();

	pwm_32k_chn_en(PWM_CLOCK_32K_CHN_PWM0);

    pwm_set_tcmp(PWM_ID,1 * CLOCK_PWM_32K_1MS);

    pwm_set_tmax(PWM_ID,2 * CLOCK_PWM_32K_1MS);

#endif

    pwm_start(FLD_PWM0_EN);
}

void main_loop(void)
{
	delay_ms(500);

	gpio_toggle(LED1);
}

#endif

