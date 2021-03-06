/**
 *
 * \file
 *
 * \brief This module contains STM32 BSP APIs implementation using ChibiOS
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * Modified for STM32 using ChibiOS by: Rakibul Islam Chowdhury
 *                              <rakibul.chowdhury@diasgeo.com>
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
#include "winc1500/driver/source/m2m_hif.h"

#include "hal.h"

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
  /*Reset necessary Pins */
  palSetPadMode(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_WAKE_PORT,CONF_WINC_WAKE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,PAL_MODE_RESET);

  /*Configure GPIO pin : SPI_CS_Pin */
  palSetPadMode(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,PAL_MODE_OUTPUT_PUSHPULL);

  /*Configure GPIO pins : winc_EN_Pin winc_WAKE_Pin winc_RST_Pin */
  palSetPadMode(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(CONF_WINC_WAKE_PORT,CONF_WINC_WAKE_PIN,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,PAL_MODE_OUTPUT_PUSHPULL);

/*Configure GPIO pin : winc_IRQ_Pin */
  palSetPadMode(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_MODE_INPUT_PULLUP);
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
  palSetPadMode(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_WAKE_PORT,CONF_WINC_WAKE_PIN,PAL_MODE_RESET);
  palSetPadMode(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN,PAL_MODE_RESET);
  return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
  palClearPad(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN);
  palClearPad(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN);
  nm_bsp_sleep(10);

  palSetPad(CONF_WINC_ENABLE_PORT,CONF_WINC_ENABLE_PIN);
  nm_bsp_sleep(10);
  palSetPad(CONF_WINC_RESET_PORT,CONF_WINC_RESET_PIN);
  nm_bsp_sleep(10);
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
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
  /* Initializing ISR function - called from nm_bsp_register_isr() */
  palEnablePadEvent(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_EVENT_MODE_FALLING_EDGE);
  palSetPadCallback(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,isr,NULL);
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
	  palEnablePadEvent(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,PAL_EVENT_MODE_FALLING_EDGE);
      palSetPadCallback(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN,isr,NULL);
	}
	else
	{
	  palClearPad(CONF_WINC_IRQ_PORT,CONF_WINC_IRQ_PIN);
	}
}
