/********************************************************************************************************
 * @file	app_config.h
 *
 * @brief	This is the header file for B91m
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
#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define	 PWM_CONTINUE		    1
#define	 PWM_COUNT			    2
#define	 PWM_IR			        3
#define	 PWM_IR_FIFO		    4
#define	 PWM_IR_DMA		        5
#if(MCU_CORE_B92)
#define  PWM_CENTER_ALIGNED     6
#endif
#define	 SET_PWM_MODE		   PWM_CONTINUE

#if(MCU_CORE_B92)
#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PD2
#define LED4            GPIO_PD3
#elif(MCU_CORE_B91)
#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6
#define LED4            GPIO_PB7
#endif

#define     PWM_PCLK_SPEED				12000000 //pwm clock 12M.
#define     PWM_32K_SPEED               32000    //pwm 32K


/**
 * This configure is for PWM Ticks.
 */
enum{
	CLOCK_PWM_CLOCK_1S = PWM_PCLK_SPEED,
	CLOCK_PWM_CLOCK_1MS = (CLOCK_PWM_CLOCK_1S / 1000),
	CLOCK_PWM_CLOCK_1US = (CLOCK_PWM_CLOCK_1S / 1000000),
};

enum{
	CLOCK_PWM_32K_1S = PWM_32K_SPEED,
	CLOCK_PWM_32K_1MS = (CLOCK_PWM_32K_1S  / 1000),
};











/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
