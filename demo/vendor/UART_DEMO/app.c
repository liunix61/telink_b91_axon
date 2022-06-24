/********************************************************************************************************
 * @file	app.c
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

#if (UART_MODE==UART_NDMA)

#if( FLOW_CTR==USE_CTS)
	#define STOP_VOLT   	1			//0 :Low level stops TX.  1 :High level stops TX.
#endif

#if (FLOW_CTR==USE_RTS)
	#define RTS_MODE		UART0_RTS_MODE_AUTO 		    //It can be UART_RTS_MODE_AUTO/UART_RTS_MODE_MANUAL.
	#define RTS_THRESH		5			//UART_RTS_MODE_AUTO need.It indicates RTS trigger threshold.
	#define RTS_INVERT		1			//UART_RTS_MODE_AUTO need.1 indicates RTS_pin will change from low to hign.
	#define RTS_POLARITY	0			//UART_RTS_MODE_MANUAL need. It indicates RTS_POLARITY .
#endif

#if(BASE_TX == FLOW_CTR)
#define UART_TX_BYTE	               1
#define UART_TX_HWORD			       2
#define UART_TX_WORD                   3

#define UART_TANS_MODE				   UART_TX_BYTE//when FLOW_CTR=BASE_TX	,it will work
#endif

volatile unsigned char uart_rx_flag=0;
volatile unsigned int  uart_irq_cnt=0;
volatile unsigned char uart_irq_index=0;
volatile unsigned char uart_cts_count=0;

volatile unsigned char uart_rx_buff_byte[UART_RX_IRQ_LEN] __attribute__((aligned(4))) ={0x00};

volatile unsigned char uart_tx_buff_byte[16] __attribute__((aligned(4))) ={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
volatile unsigned short uart_tx_buff_hword[8] __attribute__((aligned(4))) ={0x1100,0x3322,0x5544,0x7766,0x9988,0xbbaa,0xddcc,0xffee};
volatile  unsigned int uart_tx_buff_word[4] __attribute__((aligned(4))) = {0x33221100,0x77665544,0xbbaa9988,0xffeeddcc};

volatile  unsigned char uart_rx_trig_level=1; //B91 can only be 1,B92 can be 1 or 4.

void user_init()
{
	unsigned short div=0;
	unsigned char bwpc=0;

	gpio_function_en(LED1|LED2|LED3|LED4);
	gpio_output_en(LED1|LED2|LED3|LED4); 		//enable output
	gpio_input_dis(LED1|LED2|LED3|LED4);		//disable input

	uart_reset(UART0);
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
#if(MCU_CORE_B91)
	uart_set_rtx_pin(UART0_RTX_PIN);// uart0 rtx pin set
#elif(MCU_CORE_B92)
	uart_set_rtx_pin(UART0,UART0_RTX_PIN);// uart0 rtx pin set
#endif
	uart_rtx_en(UART0);
#elif(( UART_WIRE_MODE == UART_2WIRE_MODE))
#if(MCU_CORE_B91)
	uart_set_pin(UART0_TX_PIN,UART0_RX_PIN);
#elif(MCU_CORE_B92)
	uart_set_pin(UART0,UART0_TX_PIN,UART0_RX_PIN );// second parameter is tx_pin,three parameter is rx_pin.

#endif
#endif
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	uart_tx_irq_trig_level(UART0, 0);
	uart_rx_irq_trig_level(UART0, uart_rx_trig_level);
#if( FLOW_CTR == NORMAL)
	uart_set_irq_mask(UART0, UART_RX_IRQ_MASK|UART_ERR_IRQ_MASK);
#if(MCU_CORE_B92)
	uart_set_rx_timeout(UART0, bwpc, 12, UART_BW_MUL2);//uart use rx_timeout to decide the end of a packet,UART_BW_MUL2 means rx_timeout is 2-byte.
	uart_rxdone_sel(UART0,1);// no_dma:can optionally turn it on,if you use rx_done,it must be on; dma: whether rx_done is used or not, it must be closed.
	uart_set_irq_mask(UART0, UART_RXDONE_MASK);
#endif
	plic_interrupt_enable(IRQ19_UART0); //if you want to use UART1,the parameter is - IRQ18_UART1
	core_interrupt_enable();
#elif( FLOW_CTR == USE_CTS)

    uart_cts_config(UART0,UART0_CTS_PIN,STOP_VOLT);
    uart_clr_irq_mask(UART0, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK);
    uart_set_cts_en(UART0);

#elif( FLOW_CTR == USE_RTS)
	uart_set_rts_en(UART0);
	uart_rts_config(UART0,UART0_RTS_PIN,RTS_INVERT,UART_RTS_MODE_AUTO);
	uart_clr_irq_mask(UART0, UART_RX_IRQ_MASK | UART_TX_IRQ_MASK);
#if(MCU_CORE_B92)
	uart_clr_irq_mask(UART0, UART_RXDONE_MASK);
#endif
	uart_rts_trig_level_auto_mode(UART0, RTS_THRESH);
#endif
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	delay_ms(500);
	gpio_toggle(LED1);
#if( FLOW_CTR == BASE_TX)

#if(UART_TANS_MODE==UART_TX_BYTE)

	for(unsigned char i=0;i<16;i++)
	{
		uart_send_byte(UART0, uart_tx_buff_byte[i]);
#if(UART_WIRE_MODE == UART_1WIRE_MODE)
		uart_rtx_pin_tx_trig(UART0);
#endif
	}
	uart_tx_buff_byte[0]++;
#elif(UART_TANS_MODE==UART_TX_HWORD)
	for(unsigned char i=0;i<8;i++)
	{
		uart_send_hword(UART0, uart_tx_buff_hword[i]);
#if(UART_WIRE_MODE == UART_1WIRE_MODE)
		uart_rtx_pin_tx_trig(UART0);
#endif
	}
	uart_tx_buff_hword[0]++;
#elif(UART_TANS_MODE==UART_TX_WORD)

	for(unsigned char i=0;i<4;i++)
	{
		uart_send_word(UART0, uart_tx_buff_word[i]);
#if(UART_WIRE_MODE == UART_1WIRE_MODE)
		uart_rtx_pin_tx_trig(UART0);
#endif
	}
	uart_tx_buff_word[0]++;
#endif

#elif( FLOW_CTR == NORMAL)
	if(uart_rx_flag>0)
	{
		uart_irq_cnt=0; //Clear uart_irq_cnt
		uart_rx_flag=0;
		for(unsigned char i=0;i<UART_RX_IRQ_LEN;i++)
		{
		    uart_send_byte(UART0, uart_rx_buff_byte[i]);
#if(UART_WIRE_MODE == UART_1WIRE_MODE)
	        uart_rtx_pin_tx_trig(UART0);
#endif
		}
	}
#elif( FLOW_CTR == USE_CTS)
	uart_send_byte(UART0, uart_tx_buff_byte[uart_cts_count]);
	uart_cts_count++;
	if(uart_cts_count == 16)
	{
		uart_cts_count = 0;
	}
#elif( FLOW_CTR == USE_RTS)
	while(1);
#endif
}

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_sec_ void uart0_irq_handler(void)// if you want to use UART1,the function is - uart1_irq_handler()
{
#if( FLOW_CTR == NORMAL)
	if(uart_get_irq_status(UART0, UART_RX_ERR))
	{
		uart_clr_irq_status(UART0,UART_CLR_RX);// it will clear rx_fifo and rx_err_irq ,rx_buff_irq,so it won't enter rx_buff interrupt.
		uart_reset(UART0); //clear hardware pointer
		uart_clr_rx_index(UART0); //clear software pointer
		uart_irq_cnt=0;
		uart_rx_flag=0;
	}
	if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS))
	{
		gpio_set_high_level(LED3);
		if(uart_rx_flag==0)
		{
			for(int i=0;i<uart_rx_trig_level;i++)
			{
				uart_rx_buff_byte[uart_irq_cnt++] = uart_read_byte(UART0);
				if((uart_irq_cnt%UART_RX_IRQ_LEN==0)&&(uart_irq_cnt!=0))
				{
					uart_rx_flag=1;
				}
			}
		}
		else
		{
			unsigned char uart_fifo_cnt=uart_get_rxfifo_num(UART0);
		    if(uart_fifo_cnt!=0){
		    	for(int j=0;j<uart_fifo_cnt;j++)
				{
		    		uart_read_byte(UART0);
				}
		    }

		}
	}
#if (MCU_CORE_B92)
	if(uart_get_irq_status(UART0, UART_RXDONE))
	{
		gpio_set_high_level(LED4);
		unsigned char uart_fifo_cnt=uart_get_rxfifo_num(UART0);
	    if(uart_fifo_cnt!=0)
	    {
	    	if(uart_rx_flag==0)
	    	{
		        for(int j=0;j<uart_fifo_cnt;j++)
			    {

				     if((uart_irq_cnt%UART_RX_IRQ_LEN==0)&&(uart_irq_cnt!=0))
					 {
						 uart_rx_flag=1;
						 break;
					 }

				     uart_rx_buff_byte[uart_irq_cnt++] = uart_read_byte(UART0);
		      	}
	    	}
	    }
	    //In b92, clearing rx_fifo does not clear rx_done.
	    uart_clr_irq_status(UART0,UART_CLR_RXDONE);
	    uart_clr_irq_status(UART0,UART_CLR_RX);
	    uart_clr_rx_index(UART0);
	 }
#endif
#endif
}
#endif












