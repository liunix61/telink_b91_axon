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
#pragma once
#include "driver.h"
#include "printf.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define	 PKE_ECDH			1
#define	 PKE_ECDSA			2
#define	 PKE_X25519			3
#if (MCU_CORE_B92 || MCU_CORE_B93)
#define  PKE_RSA			4
#endif


#define	 PKE_TEST_MODE		PKE_ECDH

#if(MCU_CORE_B91)
#define LED1            		GPIO_PB4
#define LED2            		GPIO_PB5
#define LED3            		GPIO_PB6
#define LED4            		GPIO_PB7

#elif(MCU_CORE_B92)
#define LED1            		GPIO_PD0
#define LED2            		GPIO_PD1
#define LED3            		GPIO_PD2
#define LED4            		GPIO_PD3

#elif(MCU_CORE_B93)
#define LED1            		GPIO_PD0
#define LED2            		GPIO_PD1
#define LED3            		GPIO_PD2
#define LED4            		GPIO_PD3
#define LED5            		GPIO_PD4
#define LED6            		GPIO_PD5
#endif


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
