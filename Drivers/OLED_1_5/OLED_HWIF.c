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
#include "LPC17xx.h"
#include "LPC17xx_SPI.h"
#include "SysTick.h"

#define OLED_SPI_PORT SPI_PORT_0
//#define OLED_SPI_PORT SPI_BIT_BASH

//====================================================================================
void OLED_InitIF(void)
{
	SPI_CFG_Type config;	// Configuration structure used by the SPI peripheral driver

	// The OLED display requires two GPIO lines, one for reset(RST) and the other to 
	// switch between command and data(CD) mode
	LPC_GPIO0->FIOSET = 1 << 21;
	LPC_GPIO0->FIOSET = 1 << 22;
	LPC_GPIO0->FIODIR |= 1 << 22;	// RST output
	LPC_GPIO0->FIODIR |= 1 << 21;	// CD output
	
	// Set clock phase to second edge
	config.CPHA = SPI_CFG_CPHA_SECOND;			
	
	// The clock is in low idle state beteen data
	config.CPOL = SPI_CFG_CPOL_HIGH; 			
	
	// We are the master the display is the slave on the bus
	config.Mode = SPI_CFG_MODE_MASTER;			
	
	// Standard SPI frame foramt
	config.FrameFormat = SPI_CFG_FORAMT_SPI;	
	
	// 8 data bit used per transation
	config.DataBits = 8;						
	
	// Set the clock to 1MHz
	config.Clock = 1000000;						
	
	SPI_Init(OLED_SPI_PORT, &config);
}

//====================================================================================
void OLED_MsDelay(uint32_t ms_delay)
{
	MsDelay(ms_delay);
}

//====================================================================================
void OLED_SendByte(uint8_t data)
{
	SPI_SendByte(OLED_SPI_PORT, data);	
}

//====================================================================================
void OLED_ResetAssert(void)
{
	LPC_GPIO0->FIOCLR = 1 << 22;			// Turn the LED off
}

//====================================================================================
void OLED_ResetDeassert(void)
{
	LPC_GPIO0->FIOSET = 1 << 22;			// Turn the LED on
}

//====================================================================================
void OLED_SetCommand(void)
{
	SPI_WaitWhileBusy(OLED_SPI_PORT);	// Wait while data getting sent before the set command below
	LPC_GPIO0->FIOCLR = 1 << 21;	// CD high
}

//====================================================================================
void OLED_SetData(void)
{
	SPI_WaitWhileBusy(OLED_SPI_PORT);	// Wait while data getting sent before the set command below
	LPC_GPIO0->FIOSET = 1 << 21;	// CD low
}

