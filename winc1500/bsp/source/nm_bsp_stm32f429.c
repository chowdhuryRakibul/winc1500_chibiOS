/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "winc1500/bsp/include/nm_bsp.h"
#include "winc1500/common/include/nm_common.h"
#include "winc1500/config/conf_winc.h"
#include "hal.h"

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
	/*Configure GPIO pin Output Level */
  palSetPadMode(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,PAL_MODE_RESET);
//	HAL_GPIO_WritePin(SPI_WIFI_CS_PORT, SPI_WIFI_CS_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
  palSetPadMode(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_WAKE_PORT,CONF_WINC_WAKE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,PAL_MODE_RESET);

//	HAL_GPIO_WritePin(CONF_WINC_ENABLE_PORT, CONF_WINC_ENABLE_PIN|CONF_WINC_WAKE_PIN |CONF_WINC_RESET_PIN, GPIO_PIN_RESET);

	/*Configure GPIO pin : SPI_CS_Pin */
//	GPIO_InitStruct.Pin = SPI_WIFI_CS_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(SPI_WIFI_CS_PORT, &GPIO_InitStruct);
  palSetPadMode(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,PAL_MODE_OUTPUT_PUSHPULL);

	/*Configure GPIO pins : winc_EN_Pin winc_WAKE_Pin winc_RST_Pin */
//	GPIO_InitStruct.Pin = CONF_WINC_ENABLE_PIN|CONF_WINC_WAKE_PIN |CONF_WINC_RESET_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(CONF_WINC_ENABLE_PORT, &GPIO_InitStruct);
  palSetPadMode(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(CONF_WINC_WAKE_PORT,CONF_WINC_WAKE_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,PAL_MODE_OUTPUT_PUSHPULL);

	/*Configure GPIO pin : winc_IRQ_Pin */
//	GPIO_InitStruct.Pin = CONF_WINC_IRQ_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(CONF_WINC_IRQ_PORT, &GPIO_InitStruct);
  palSetPadMode(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_MODE_INPUT_PULLUP);
  myPrintf("initialzed chip pins\n");
}

/*
 *	@fn		nm_bsp_init
 *	@brief	Initialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_init(void)
{
	/* Initialize chip IOs. */
	init_chip_pins();

	/* Perform chip reset. */
	nm_bsp_reset();

	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_deinit
 *	@brief	De-iInitialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{
	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
//	HAL_GPIO_WritePin(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,GPIO_PIN_RESET);
//	nm_bsp_sleep(10);
//	HAL_GPIO_WritePin(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,GPIO_PIN_SET);
//	nm_bsp_sleep(10);
//	HAL_GPIO_WritePin(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,GPIO_PIN_SET);
  palSetPad(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN);
  chThdSleepMilliseconds(10);
  palSetPad(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN);
  myPrintf("reset chip pins\n");
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
//	HAL_Delay(u32TimeMsec);
  chThdSleepMilliseconds(u32TimeMsec);
}

/*
 *	@fn		nm_bsp_register_isr
 *	@brief	Register interrupt service routine
 *	@param[IN]	pfIsr
 *				Pointer to ISR handler
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
//	GPIO_InitTypeDef GPIO_InitStruct;

	/* EXTI2 init ISR function - called from nm_bsp_register_isr() */

//	 __GPIOB_CLK_ENABLE();

	/*Configure GPIO pin : PA2 */
//	GPIO_InitStruct.Pin   = CONF_WINC_SPI_INT_PIN;
//	GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
//	GPIO_InitStruct.Pull  = GPIO_NOPULL;
//	HAL_GPIO_Init(CONF_WINC_SPI_INT_PORT, &GPIO_InitStruct);

	/* EXTI 2 (PA2) interrupt init*/
//	HAL_NVIC_SetPriority(CONF_WINC_EXTI_IRQN, 0x00, 0);
//	HAL_NVIC_EnableIRQ(CONF_WINC_EXTI_IRQN);
  palEnablePadEvent(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_EVENT_MODE_FALLING_EDGE);
  /************************************************************************
   * palSetPadCallback(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,HANDLER,NULL) *
   ************************************************************************/
}

/*
 *	@fn		nm_bsp_interrupt_ctrl
 *	@brief	Enable/Disable interrupts
 *	@param[IN]	u8Enable
 *				'0' disable interrupts. '1' enable interrupts
 */
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	if (1 == u8Enable)
	{
//		HAL_NVIC_SetPriority((IRQn_Type)(CONF_WINC_EXTI_IRQN), 0x01, 0);
//		HAL_NVIC_EnableIRQ((IRQn_Type)(CONF_WINC_EXTI_IRQN));
	  palEnablePadEvent(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_EVENT_MODE_FALLING_EDGE);
      /************************************************************************
      * palSetPadCallback(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,HANDLER,NULL)  *
      ************************************************************************/
	}
	else
	{
//		HAL_NVIC_DisableIRQ((IRQn_Type)(CONF_WINC_EXTI_IRQN));
	  palClearPad(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN);
	}
}
