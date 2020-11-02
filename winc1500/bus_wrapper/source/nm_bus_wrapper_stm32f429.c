/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
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
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>     /* Included for uint_t */

#include "winc1500/bsp/include/nm_bsp.h"
#include "winc1500/common/include/nm_common.h"
#include "winc1500/bus_wrapper/include/nm_bus_wrapper.h"
#include "winc1500/config/conf_winc.h"
#include "hal.h"
#include "ch.h"


const SPIConfig spicfg = {
      false,
      NULL,
      SPI_WIFI_CS_PORT,
      SPI_WIFI_CS_PIN,
      SPI_CR1_BR_0 | SPI_CR1_MSTR | SPI_CR1_SPE,
      0
    };

#define NM_BUS_MAX_TRX_SZ	256
//SPI_HandleTypeDef hspi2;
//SPIDriver SPID1;
tstrNmBusCapabilities egstrNmBusCapabilities =
{
	NM_BUS_MAX_TRX_SZ
};

#ifdef CONF_WINC_USE_SPI

static uint8 spiDummyBuf[300] = {0};

static sint8 spi_rw(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
//	HAL_StatusTypeDef status;
	/* Start SPI transaction - polling method */
//	HAL_GPIO_WritePin(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,GPIO_PIN_RESET);
  spiSelect(&SPID2);
	/* Transmit/Recieve */
	if (pu8Mosi == NULL)
	{
//		status = HAL_SPI_TransmitReceive(&hspi2,spiDummyBuf,pu8Miso,u16Sz,1000);
	  spiExchange(&SPID2,u16Sz,spiDummyBuf,pu8Miso);
	}
	else if(pu8Miso == NULL)
	{
//		status = HAL_SPI_TransmitReceive(&hspi2,pu8Mosi,spiDummyBuf,u16Sz,1000);
	  spiExchange(&SPID2,u16Sz,pu8Mosi,spiDummyBuf);
		memset(spiDummyBuf,0, u16Sz);
	}
	else
	{
	  spiExchange(&SPID2,u16Sz,pu8Mosi,pu8Miso);
//		status = HAL_SPI_TransmitReceive(&hspi2,pu8Mosi,pu8Miso,u16Sz,1000);
	}

	/* Handle Transmit/Recieve error */
//	if (status != HAL_OK)
//	{
//		M2M_ERR("%s: HAL_SPI_TransmitReceive failed. error (%d)\n",__FUNCTION__,status);
//		return status;
//	}
	spiUnselect(&SPID2);
//	HAL_GPIO_WritePin(SPI_WIFI_CS_PORT,SPI_WIFI_CS_PIN,GPIO_PIN_SET);
	return M2M_SUCCESS;
}
#endif


void nm_bus_wifi_spi_init(SPIDriver *SPID)
{
	/* Peripheral clock enable */
//	__HAL_RCC_SPI2_CLK_ENABLE();
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//
//	GPIO_InitTypeDef  GPIO_InitStruct = {0};
  palSetPadMode(SPI_WIFI_MISO_PORT, SPI_WIFI_MISO_PIN,PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
//	GPIO_InitStruct.Pin = SPI_WIFI_MISO_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//	HAL_GPIO_Init(SPI_WIFI_MISO_PORT, &GPIO_InitStruct);
  palSetPadMode(SPI_WIFI_MOSI_PORT, SPI_WIFI_MOSI_PIN,PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);

//	GPIO_InitStruct.Pin = SPI_WIFI_MOSI_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//	HAL_GPIO_Init(SPI_WIFI_MOSI_PORT, &GPIO_InitStruct);
  palSetPadMode(SPI_WIFI_SCK_PORT, SPI_WIFI_SCK_PIN,PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);

//	GPIO_InitStruct.Pin = SPI_WIFI_SCK_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//	HAL_GPIO_Init(SPI_WIFI_SCK_PORT, &GPIO_InitStruct);
  //palSetPadMode(SPI_WIFI_CS_PORT, SPI_WIFI_CS_PIN,PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  spiStart(SPID,&spicfg);
}

/*
*	@fn		nm_bus_init
*	@brief	Initialize the bus wrapper
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*/
sint8 nm_bus_init(void *pvinit)
{
	sint8 result = M2M_SUCCESS;

	/* SPI2 parameter configuration*/
//	hspi2.Instance = SPI2;
//	hspi2.Init.Mode = SPI_MODE_MASTER;
//	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
//	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
//	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
//	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
//	hspi2.Init.NSS = SPI_NSS_SOFT;
//	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
//	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
//	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	hspi2.Init.CRCPolynomial = 10;
//	if (HAL_SPI_Init(&hspi2) != HAL_OK)
//	{
//		M2M_ERR("SPI bus Initialization error\r\n");
//	}
//	HAL_SPI_MspInit(&hspi2);
	nm_bus_wifi_spi_init(&SPID2);
	return result;
}

/*
*	@fn		nm_bus_ioctl
*	@brief	send/receive from the bus
*	@param[IN]	u8Cmd
*					IOCTL command for the operation
*	@param[IN]	pvParameter
*					Arbitrary parameter depenging on IOCTL
*	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@note	For SPI only, it's important to be able to send/receive at the same time
*/
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
	sint8 s8Ret = 0;
	switch(u8Cmd)
	{
		case NM_BUS_IOCTL_RW: {
			tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
			s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
		}
		break;

		default:
			s8Ret = -1;
			M2M_ERR("invalide ioclt cmd\n");
			break;
	}

	return s8Ret;
}

/*
*	@fn		nm_bus_deinit
*	@brief	De-initialize the bus wrapper
*/
sint8 nm_bus_deinit(void)
{
	sint8 result = M2M_SUCCESS;
	return result;
}

/*
*	@fn			nm_bus_reinit
*	@brief		re-initialize the bus wrapper
*	@param [in]	void *config
*					re-init configuration data
*	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
*	@author		Dina El Sissy
*	@date		19 Sept 2012
*	@version	1.0
*/
sint8 nm_bus_reinit(void* config)
{
	return M2M_SUCCESS;
}

