//====================================================================================
// OLED Hardware Interface
//
// This file should be edited by the user to provide to a peripheral driver for
// interfacing to the OLED
//====================================================================================

// Function decleration file
#include <stdint.h>			// Include standard types
#include "OLED_HWIF.h"
#include "string.h"

// User specific header files
#include "lpc17xx_ssp.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_gpdma.h"

#include "BR_SysTick.h"


// PORT and PIN numbers that OLED reset pin assigned on
#define RST_PORT_NUM	2
#define RST_PIN_NUM		(1<<4)

// PORT and PIN numbers that Command/Data pin assigned on
#define CD_PORT_NUM		1
#define CD_PIN_NUM		(1<<23)


#define SSP_USE_DMA

// Terminal Counter flag for Channel 0
__IO uint32_t Channel0_TC;

// Error Counter flag for Channel 0
__IO uint32_t Channel0_Err;

//====================================================================================
void OLED_InitIF(void)
{
	PINSEL_CFG_Type pinsel_cfg;
	// SSP Configuration structure variable
	SSP_CFG_Type SSP_ConfigStruct;

	// Initialize SPI pin connect
	// 	P2.20 - SCK;
	// 	P2.21 - SSEL - used as GPIO
	// 	Not used - MISO
	// 	P2.24 - MOSI

	pinsel_cfg.Funcnum = PINSEL_FUNC_3;
	pinsel_cfg.OpenDrain = 0;
	pinsel_cfg.Pinmode = 0;
	pinsel_cfg.Portnum = PINSEL_PORT_1;
	pinsel_cfg.Pinnum = PINSEL_PIN_20;
	PINSEL_ConfigPin(&pinsel_cfg);
	pinsel_cfg.Pinnum = PINSEL_PIN_24;
	PINSEL_ConfigPin(&pinsel_cfg);
	pinsel_cfg.Pinnum = PINSEL_PIN_21;
	PINSEL_ConfigPin(&pinsel_cfg);

	// Initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);

	SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
	SSP_ConfigStruct.CPOL = SSP_CPOL_LO;
	SSP_ConfigStruct.ClockRate = 8000000;

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP0, &SSP_ConfigStruct);
	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP0, ENABLE);

#ifdef SSP_USE_DMA
	// GPDMA Interrupt configuration section
	// preemption = 1, sub-priority = 1
	//NVIC_SetPriority(SSP0_IRQn, ((0x01<<3)|0x01));
	// Enable SSP0 interrupt
	//NVIC_EnableIRQ(SSP0_IRQn);

	// Initialize GPDMA controller
	GPDMA_Init();

	// Setting GPDMA interrupt
    // Disable interrupt for DMA
    NVIC_DisableIRQ (DMA_IRQn);
    // preemption = 1, sub-priority = 1
    NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));




#endif

	// Configure OLED reset and c/d lines and set high
	GPIO_SetDir(RST_PORT_NUM, RST_PIN_NUM, 1);
	GPIO_SetDir(CD_PORT_NUM, CD_PIN_NUM, 1);

	GPIO_SetValue(RST_PORT_NUM, RST_PIN_NUM);
	GPIO_SetValue(CD_PORT_NUM, CD_PIN_NUM);
}

//====================================================================================
void OLED_MsDelay(uint32_t ms_delay)
{
	BR_MsDelay(ms_delay);
}

//====================================================================================
void OLED_SendByte(uint8_t data)
{
	SSP_SendData(LPC_SSP0,data);
}


//====================================================================================
void OLED_SendBuffer(uint8_t *data, uint16_t length)
{
#ifndef SSP_USE_DMA
	uint16_t i;

	for(i = 0; i < length; i++)
	{
		SSP_SendData(LPC_SSP0, *data++);
	}
#else
	GPDMA_Channel_CFG_Type GPDMACfg;

	// Configure GPDMA channel 0 -------------------------------------------------------------
    // DMA Channel 0
    GPDMACfg.ChannelNum = 0;
	// Source memory
	GPDMACfg.SrcMemAddr = (uint32_t) &data;
	// Destination memory - Not used
	GPDMACfg.DstMemAddr = 0;
	// Transfer size
	//GPDMACfg.TransferSize = length;
	GPDMACfg.TransferSize = 50;
	// Transfer width - not used
	GPDMACfg.TransferWidth = 0;
	// Transfer type
	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
	// Source connection - unused
	GPDMACfg.SrcConn = 0;
	// Destination connection
	GPDMACfg.DstConn = GPDMA_CONN_SSP0_Tx;
	// Linker List Item - unused
	GPDMACfg.DMALLI = 0;
	// Setup channel with given parameter
	GPDMA_Setup(&GPDMACfg);

	// Reset terminal counter
	Channel0_TC = 0;
	// Reset Error counter
	Channel0_Err = 0;

	// Enable TX DMA on SSP0
	SSP_DMACmd (LPC_SSP0, SSP_DMA_TX, ENABLE);

	// Enable Tx and Rx DMA on SSP0
	//SSP_DMACmd (LPC_SSP0, SSP_DMA_RX, ENABLE);

	// Enable GPDMA channel 0
	GPDMA_ChannelCmd(0, ENABLE);

    // Enable interrupt for DMA
    NVIC_EnableIRQ (DMA_IRQn);
    GPIO_ClearValue(1, 1<<18);
#endif
}

#ifdef SSP_USE_DMA
//====================================================================================
void DMA_IRQHandler (void)
{
    GPIO_SetValue(1, 1<<18);

	// check GPDMA interrupt on channel 0
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0)){
		// Check counter terminal status
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)){
			// Clear terminate counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
				Channel0_TC++;
		}
		// Check error terminal status
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)){
			// Clear error counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
			Channel0_Err++;
		}
	}
	// check GPDMA interrupt on channel 1
	if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 1)){
		// Check counter terminal status
		if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 1)){
			// Clear terminate counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 1);
				//Channel1_TC++;
		}
		// Check error terminal status
		if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 1)){
			// Clear error counter Interrupt pending
			GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 1);
			//Channel1_Err++;
		}
	}
}
#endif

//====================================================================================
void OLED_ResetAssert(void)
{
	GPIO_ClearValue(RST_PORT_NUM, RST_PIN_NUM);
}

//====================================================================================
void OLED_ResetDeassert(void)
{
	GPIO_SetValue(RST_PORT_NUM, RST_PIN_NUM);
}

//====================================================================================
void OLED_SetCommand(void)
{
    while(SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY) == SET);
	GPIO_ClearValue(CD_PORT_NUM, CD_PIN_NUM);
}

//====================================================================================
void OLED_SetData(void)
{
    while(SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY) == SET);
	GPIO_SetValue(CD_PORT_NUM, CD_PIN_NUM);
}

