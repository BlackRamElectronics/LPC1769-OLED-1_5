#include "LPC17xx.h"
#include "LPC17xx_PCLKPWR.h"

//====================================================================================
// Set the peripheral clock divider. Peripheral clock = main clock / peripheral divider
// Peripheral = CLKPWR_PCLKSEL_x
void CLKPWR_SetPCLKDiv(uint32_t peripheral, uint32_t divider)
{
	if(peripheral < 32)
	{
		LPC_SC->PCLKSEL0 &= (0x3 << peripheral);		// Clear current setting
		LPC_SC->PCLKSEL0 |= ((divider & 0x03) << peripheral);
	}
	else
	{
		LPC_SC->PCLKSEL1 &= (0x3 << peripheral);
		LPC_SC->PCLKSEL1 |= ((divider & 0x03) << peripheral);
	}
}

//====================================================================================
// Peripheral = CLKPWR_PCLKDIV_x
uint32_t CLKPWR_GetPCLKDiv(uint32_t peripheral)
{
	if(peripheral < 32)
	{
		return((0x03)&(LPC_SC->PCLKSEL0 >> peripheral));
	}
	else
	{
		return((0x03)&(LPC_SC->PCLKSEL1 >> peripheral));
	}
}

//====================================================================================
// Get current peripheral clock speed
// Peripheral = CLKPWR_PCLKSEL_x
uint32_t CLKPWR_GetPCLK(uint32_t peripheral)
{
	uint32_t retval;

	switch(CLKPWR_GetPCLKDiv(peripheral))
	{
		case 0:
			retval = SystemCoreClock / 4;
			break;

		case 1:
			retval = SystemCoreClock;
			break;

		case 2:
			retval = SystemCoreClock / 2;
			break;

		case 3:
			retval = SystemCoreClock / 8;
			break;
	}
	return(retval);
}

//====================================================================================
// Peripheral = CLKPWR_PCONP_x
void CLKPWR_EnablePPWR(uint32_t peripheral)
{
	LPC_SC->PCONP |= peripheral;
}

//====================================================================================
// Peripheral = CLKPWR_PCONP_x
void CLKPWR_DisablePPWR(uint32_t peripheral)
{
	LPC_SC->PCONP &= ~peripheral;
}

//====================================================================================
// Enter sleep mode and wait for interrupt
void CLKPWR_Sleep(void)
{
	LPC_SC->PCON = 0x00;
	__WFI();	// Wait for interrupt
}

//====================================================================================
// Enter deep sleep mode and wait for interrupt
void CLKPWR_DeepSleep(void)
{
	SCB->SCR = 0x4;	// Deep-Sleep Mode, set SLEEPDEEP bit
	LPC_SC->PCON = 0x00;
	__WFI();		// Wait for interrupt
}

//====================================================================================
// Enter power down mode and wait for interrupt
void CLKPWR_PowerDown(void)
{
	SCB->SCR = 0x4;	// Deep-Sleep Mode, set SLEEPDEEP bit
	LPC_SC->PCON = 0x01;
	__WFI();		// Wait for interrupt
}

//====================================================================================
// Enter deep power down mode and wait for interrupt
void CLKPWR_DeepPowerDown(void)
{
	SCB->SCR = 0x4;	// Deep-Sleep Mode, set SLEEPDEEP bit
	LPC_SC->PCON = 0x03;
	__WFI();		// Wait for interrupt
}
