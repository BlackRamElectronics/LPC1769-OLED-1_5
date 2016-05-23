//====================================================================================
// OLED Hardware Interface
//
// This file should be edited by the user to provide to a peripheral driver for
// interfacing to the OLED
//====================================================================================

// Function decleration file
#include <stdint.h>			// Include standard types
#include "OLED_HWIF.h"

// User specific header files
#include "lpc17xx_ssp.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include "BR_SysTick.h"


// PORT and PIN numbers that OLED reset pin assigned on
#define RST_PORT_NUM	2
#define RST_PIN_NUM		(1<<4)

// PORT and PIN numbers that Command/Data pin assigned on
#define CD_PORT_NUM		1
#define CD_PIN_NUM		(1<<23)


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
	GPIO_ClearValue(CD_PORT_NUM, CD_PIN_NUM);
}

//====================================================================================
void OLED_SetData(void)
{
	GPIO_SetValue(CD_PORT_NUM, CD_PIN_NUM);
}

