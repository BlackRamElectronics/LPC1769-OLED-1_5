#include "LPC17xx.h"
#include "LPC17xx_SPI.h"
#include "LPC17xx_PCLKPWR.h"

//====================================================================================
// Register bit definitions
//====================================================================================

//====================================================================================
// Control register 0
#define SSP_CR0_DSS(n)			((uint32_t)((n-1)&0xF))		// Data size select
#define SSP_CR0_FRF_SPI			((uint32_t)(0<<4))			// Frame format = SPI
#define SSP_CR0_FRF_TI			((uint32_t)(1<<4))			// Frame format = TI
#define SSP_CR0_FRF_MICOWIRE	((uint32_t)(2<<4))			// Microwire
#define SSP_CR0_CPOL			((uint32_t)(1<<6))			// Clock out polarity
#define SSP_CR0_CPHA			((uint32_t)(1<<7))			// Clock out phase
#define SSP_CR0_SCR(n)   		((uint32_t)((n&0xFF)<<8))	//Serial clock rate

//====================================================================================
// Control register 1
#define SSP_CR1_LBM				((uint32_t)(1<<0))			// Loop back mode
#define SSP_CR1_SSE				((uint32_t)(1<<1))			// SPI controller enable
#define SSP_CR1_MS				((uint32_t)(1<<2))			// 0 = Master, 1 = Slave
#define SSP_CR1_SOD				((uint32_t)(1<<3))			// Slave output disable

//====================================================================================
// Status register
#define SSP_SR_TFE				((uint32_t)(1<<0))			// Transmit FIFO Empty
#define SSP_SR_TNF				((uint32_t)(1<<1))			// Transmit FIFO Not Full
#define SSP_SR_RNE				((uint32_t)(1<<2))			// Receive FIFO Not Empty
#define SSP_SR_RFF				((uint32_t)(1<<3))			// Receive FIFO Full
#define SSP_SR_BSY				((uint32_t)(1<<4))			// Busy

//====================================================================================
// Clock prescale register
#define SSP_CPSR_CPDVSR(n)		((uint32_t)(n&0xFF))		// Clock prescaler value

//====================================================================================
// Interrupt mask set/clear register
#define SSP_IMSC_RORIM			((uint32_t)(1<<0))			// Enable interrupt on receive overrun
#define SSP_IMSC_RTIM			((uint32_t)(1<<1))			// Enable interrupt on receive time-out
#define SSP_IMSC_RXIM			((uint32_t)(1<<2))			// Enable interrupt on rx FIFO at least half full
#define SSP_IMSC_TXIM			((uint32_t)(1<<3))			// Enable interrupt on tx FIFO at least half empty

//====================================================================================
// Raw interrupt status register
#define SSP_RIS_RORRIS			((uint32_t)(1<<0))			// Receive overrun interrupt flag
#define SSP_RIS_RTIM			((uint32_t)(1<<1))			// Receive time-out interrupt flag
#define SSP_RIS_RXIM			((uint32_t)(1<<2))			// Rx FIFO at least half full interrupt flag
#define SSP_RIS_TXIM			((uint32_t)(1<<3))			// Tx FIFO at least half empty interrupt flag

//====================================================================================
// Masked interrupt status register
#define SSP_MIS_RORRIS			((uint32_t)(1<<0))			// Receive overrun interrupt flag
#define SSP_MIS_RTIM			((uint32_t)(1<<1))			// Receive time-out interrupt flag
#define SSP_MIS_RXIM			((uint32_t)(1<<2))			// Rx FIFO at least half full interrupt flag
#define SSP_MIS_TXIM			((uint32_t)(1<<3))			// Tx FIFO at least half empty interrupt flag

//====================================================================================
// Interrupt clear register
#define SSP_ICR_RORIC			((uint32_t)(1<<0))			// Clear receive overrun interrupt flag
#define SSP_ICR_RTIC			((uint32_t)(1<<1))			// Clear receive time-out interrupt flag

//====================================================================================
// DMA control register
#define SSP_DMACR_RXDMAE		((uint32_t)(1<<0))			// Receive DMA enable
#define SSP_DMACR_TXDMAE		((uint32_t)(1<<1))			// Transmit DME enable


//====================================================================================
void SPI_SetConfigDefault(SPI_CFG_Type *config)
{
	config->CPHA		= SPI_CFG_CPHA_FIRST;	// First clock edge
	config->CPOL		= SPI_CFG_CPOL_HIGH;	// High level
	config->Mode		= SPI_CFG_MODE_MASTER;	// Master mode
	config->DataBits	= 8;					// Set to 8 data bits
	config->FrameFormat	= SPI_CFG_FORAMT_SPI;	// Use standard SPI format
	config->Clock		= 100000;				// Set to 100kHz data rate
}

//====================================================================================
void SPI_Init(uint8_t port, SPI_CFG_Type *config)
{	
	uint32_t prescale, clk_divider, ref_clk, ssp_clk;
	
	if(port == SPI_PORT_0)
	{	
		// Clear Current pin configuration
		LPC_PINCON->PINSEL0 &= ~((1<<30) | (1<<31));	// P0.15 - SCK0
		LPC_PINCON->PINSEL1 &= ~((1<<0) | (1<<1));		// P0.16 - SSEL0
		LPC_PINCON->PINSEL1 &= ~((1<<2) | (1<<3));		// P0.17 - MISO0
		LPC_PINCON->PINSEL1 &= ~((1<<4) | (1<<5));		// P0.18 - MOSI0
	
		// Enable pins for SPI
		LPC_PINCON->PINSEL0 |= (1<<31);	// P0.15 - SCK0
		LPC_PINCON->PINSEL1 |= (1<<1);	// P0.16 - SSEL0
		LPC_PINCON->PINSEL1 |= (1<<3);	// P0.17 - MISO0
		LPC_PINCON->PINSEL1 |= (1<<5);	// P0.18 - MOSI0

		// Enable power for SPI module
		CLKPWR_EnablePPWR(CLKPWR_PCONP_PCSSP0);

		// Get peripheral clock speed
		ssp_clk = CLKPWR_GetPCLK(CLKPWR_PCLKDIV_SSP0);
		
		// Configure SPI module
		LPC_SSP0->CR0 = 0;	// Init to 0
		
		// Set data bits
		LPC_SSP0->CR0 |= SSP_CR0_DSS(config->DataBits);
		
		// Set clock phase
		if(config->CPHA == SPI_CFG_CPHA_SECOND)
		{
			LPC_SSP0->CR0 |= SSP_CR0_CPHA;
		}
		
		// Set clock polarity
		if(config->CPOL == SPI_CFG_CPOL_HIGH)
		{
			LPC_SSP0->CR0 |= SSP_CR0_CPOL;
		}
		
		// The following code will find the smallest possible prescale value that
		// we can use to get the requested clock rate. The smallest possilbe prescale
		// value will give us the highest accuracy clock possible.
		clk_divider = 0;					// Start with zero divider 
		ref_clk = 0xFFFFFFFF;
		prescale = 2;						// Min prescale available is 2
		while(ref_clk > config->Clock)
		{
			ref_clk = ssp_clk / ((clk_divider + 1) * prescale);
			if(ref_clk > config->Clock)
			{
				clk_divider++;
				if(clk_divider > 0xFF)
				{
					clk_divider = 0;
					prescale += 2;
				}
			}
		}

		// Write computed prescaler and divider back to register
		LPC_SSP0->CR0 |= SSP_CR0_SCR(clk_divider);	// Set serial clock speed
		LPC_SSP0->CPSR = prescale;					// Set clock prescaler
		
		LPC_SSP0->CR1 = SSP_CR1_SSE;				// Enable SPI controller
	}
	else if(port == SPI_PORT_1)
	{
		// Clear Current pin configuration
		LPC_PINCON->PINSEL0 &= ~((1<<14) | (1<<15));	// P0.7 - SCK1
		LPC_PINCON->PINSEL0 &= ~((1<<12) | (1<<13));	// P0.6 - SSEL1
		LPC_PINCON->PINSEL0 &= ~((1<<16) | (1<<17));	// P0.8 - MISO1
		LPC_PINCON->PINSEL0 &= ~((1<<18) | (1<<19));	// P0.9 - MOSI1
		
		// Enable pins for SPI
		LPC_PINCON->PINSEL0 |= (1<<15);	// P0.7 - SCK1
		LPC_PINCON->PINSEL0 |= (1<<13);	// P0.6 - SSEL1
		LPC_PINCON->PINSEL0 |= (1<<17);	// P0.8 - MISO1
		LPC_PINCON->PINSEL0 |= (1<<19);	// P0.9 - MOSI1

		// Enable power for SPI module
		CLKPWR_EnablePPWR(CLKPWR_PCONP_PCSSP1);

		// Get peripheral clock speed
		ssp_clk = CLKPWR_GetPCLK(CLKPWR_PCLKDIV_SSP1);
		
		// Configure SPI module
		LPC_SSP1->CR0 = 0;	// Init to 0
		
		// Set data bits
		LPC_SSP1->CR0 |= SSP_CR0_DSS(config->DataBits);
		
		// Set clock phase
		if(config->CPHA == SPI_CFG_CPHA_SECOND)
		{
			LPC_SSP1->CR0 |= SSP_CR0_CPHA;
		}
		
		// Set clock polarity
		if(config->CPOL == SPI_CFG_CPOL_HIGH)
		{
			LPC_SSP1->CR0 |= SSP_CR0_CPOL;
		}
		
		// The following code will find the smallest possible prescale value that
		// we can use to get the requested clock rate. The smallest possilbe prescale
		// value will give us the highest accuracy clock possible.
		clk_divider = 0;					// Start with zero divider 
		ref_clk = 0xFFFFFFFF;
		prescale = 2;						// Min prescale available is 2
		while(ref_clk > config->Clock)
		{
			ref_clk = ssp_clk / ((clk_divider + 1) * prescale);
			if(ref_clk > config->Clock)
			{
				clk_divider++;
				if(clk_divider > 0xFF)
				{
					clk_divider = 0;
					prescale += 2;
				}
			}
		}

		// Write computed prescaler and divider back to register
		LPC_SSP1->CR0 |= SSP_CR0_SCR(clk_divider);	// Set serial clock speed
		LPC_SSP1->CPSR = prescale;					// Set clock prescaler
		
		LPC_SSP1->CR1 = SSP_CR1_SSE;				// Enable SPI controller	
	}
	else if(port == SPI_BIT_BASH)
	{
#ifdef SPI_BIT_BASH_ENABLE
		LPC_GPIO0->FIOSET = 1 << 15;
		LPC_GPIO0->FIOSET = 1 << 16;
		LPC_GPIO0->FIOSET = 1 << 18;
		
		LPC_GPIO0->FIODIR |= 1 << 15;	// SCK output
		LPC_GPIO0->FIODIR |= 1 << 16;	// SSEL output
		LPC_GPIO0->FIODIR |= 1 << 18;	// MOSI output
#endif // SPI_BIT_BASH_ENABLE
	}
}

//====================================================================================
void SPI_Deinit(uint8_t port)
{
	if(port == SPI_PORT_0)
	{
	
	}
	else if(port == SPI_PORT_1)
	{
	
	}
	else if(port == SPI_BIT_BASH)
	{
		
	}
}

//====================================================================================
void SPI_SendByte(uint8_t port, uint8_t data)
{
	if(port == SPI_PORT_0)
	{
		while(LPC_SSP0->SR & SSP_SR_BSY);
		LPC_SSP0->DR = data;
	}
	else if(port == SPI_PORT_1)
	{
		while(LPC_SSP1->SR & SSP_SR_BSY);
		LPC_SSP1->DR = data;
	}
	else if(port == SPI_BIT_BASH)
	{
#ifdef SPI_BIT_BASH_ENABLE
		uint32_t delay, bit_count;
		uint8_t temp_byte = data;
		
		LPC_GPIO0->FIOCLR = 1 << 16;	// CS low
		
		for(bit_count = 0; bit_count < 8; bit_count++)
		{
			LPC_GPIO0->FIOCLR = 1 << 15;	// SCK low
			if(temp_byte & 0x80)
			{
				LPC_GPIO0->FIOSET = 1 << 18;	// SCK high
			}
			else
			{
				LPC_GPIO0->FIOCLR = 1 << 18;	// SCK low
			}
			temp_byte= temp_byte << 1;
			
			for(delay = 0; delay < 10; delay++);
			
			LPC_GPIO0->FIOSET = 1 << 15;	// SCK high
			for(delay = 0; delay < 10; delay++);
		}
		
		LPC_GPIO0->FIOSET = 1 << 16;	// CS high
		for(delay = 0; delay < 10; delay++);
#endif // SPI_BIT_BASH_ENABLE
	}
}

//====================================================================================
// This routine will wait while the SPI module is busy
void SPI_WaitWhileBusy(uint8_t port)
{
	if(port == SPI_PORT_0)
	{
		while(LPC_SSP0->SR & SSP_SR_BSY);
	}
	else if(port == SPI_PORT_1)
	{
		while(LPC_SSP1->SR & SSP_SR_BSY);
	}
	else if(port == SPI_BIT_BASH)
	{
		// No need to wait in bit-bash mode
	}
}
