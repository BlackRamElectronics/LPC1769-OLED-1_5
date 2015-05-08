#ifndef _LPC17XX_PCLKPWR_H_
#define _LPC17XX_PCLKPWR_H_

//====================================================================================
// Peripheral Clock Divider Definitions
//====================================================================================
#define	CLKPWR_PCLKDIV_WDT		((uint32_t)(0))		// WDT clock divider offset
#define	CLKPWR_PCLKDIV_TIMER0	((uint32_t)(2))		// TIMER0 clock divider offset
#define	CLKPWR_PCLKDIV_TIMER1	((uint32_t)(4))		// TIMER1 clock divider offset
#define	CLKPWR_PCLKDIV_UART0	((uint32_t)(6))		// UART0 clock divider offset
#define	CLKPWR_PCLKDIV_UART1	((uint32_t)(8))		// UART1 clock divider offset
#define	CLKPWR_PCLKDIV_PWM1		((uint32_t)(12))	// PWM1 clock divider offset
#define	CLKPWR_PCLKDIV_I2C0		((uint32_t)(14))	// I2C0 clock divider offset
#define	CLKPWR_PCLKDIV_SPI		((uint32_t)(16))	// SPI clock divider offset
#define	CLKPWR_PCLKDIV_SSP1		((uint32_t)(20))	// SSP1 clock divider offset
#define	CLKPWR_PCLKDIV_DAC		((uint32_t)(22))	// DAC clock divider offset
#define	CLKPWR_PCLKDIV_ADC		((uint32_t)(24))	// ADC clock divider offset
#define	CLKPWR_PCLKDIV_CAN1		((uint32_t)(26))	// CAN1 clock divider offset
#define	CLKPWR_PCLKDIV_CAN2		((uint32_t)(28))	// CAN2 clock divider offset
#define	CLKPWR_PCLKDIV_ACF		((uint32_t)(30))	// ACF clock divider offset
#define	CLKPWR_PCLKDIV_QEI		((uint32_t)(32))	// QEI clock divider offset
#define	CLKPWR_PCLKDIV_PCB		((uint32_t)(36))	// PCB clock divider offset
#define	CLKPWR_PCLKDIV_I2C1		((uint32_t)(38))	// I2C1 clock divider offset
#define	CLKPWR_PCLKDIV_SSP0		((uint32_t)(42))	// SSP0 clock divider offset
#define	CLKPWR_PCLKDIV_TIMER2	((uint32_t)(44))	// TIMER2 clock divider offset
#define	CLKPWR_PCLKDIV_TIMER3	((uint32_t)(46))	// TIMER3 clock divider offset
#define	CLKPWR_PCLKDIV_UART2	((uint32_t)(48))	// UART2 clock divider offset
#define	CLKPWR_PCLKDIV_UART3	((uint32_t)(50))	// UART3 clock divider offset
#define	CLKPWR_PCLKDIV_I2C2		((uint32_t)(52))	// I2C2 clock divider offset
#define	CLKPWR_PCLKDIV_I2S		((uint32_t)(54))	// I2S clock divider offset
#define	CLKPWR_PCLKDIV_RIT		((uint32_t)(58))	// RIT clock divider offset
#define	CLKPWR_PCLKDIV_SYSCON	((uint32_t)(60))	// SYSCON clock divider offset
#define	CLKPWR_PCLKDIV_MC		((uint32_t)(62))	// MC clock divider offset

// Peripheral clock divider definition, the value of 6 is only for 
// CAN and 8 for all others.
#define	CLKPWR_PCLK_DIV_1		((uint32_t)(1))	// Peripheral clock divider = 1
#define	CLKPWR_PCLK_DIV_2		((uint32_t)(2))	// Peripheral clock divider = 2
#define	CLKPWR_PCLK_DIV_4		((uint32_t)(0))	// Peripheral clock divider = 4
#define	CLKPWR_PCLK_DIV_6		((uint32_t)(3))	// Peripheral clock divider = 6 (only applicable to CAN)
#define	CLKPWR_PCLK_DIV_8		((uint32_t)(3))	// Peripheral clock divider = 8 (not applicable to CAN)

//====================================================================================
// Power Control for Peripherals Definitions
//====================================================================================
#define	 CLKPWR_PCONP_PCTIM0	((uint32_t)(1<<1))	// Timer0 clock enable bit
#define	 CLKPWR_PCONP_PCTIM1	((uint32_t)(1<<2))	// Timer1 clock enable bit
#define	 CLKPWR_PCONP_PCUART0	((uint32_t)(1<<3))	// UART0 clock enable bit
#define	 CLKPWR_PCONP_PCUART1	((uint32_t)(1<<4))	// UART1 clock enable bit
#define	 CLKPWR_PCONP_PCPWM1	((uint32_t)(1<<6))	// PWM1 clock enable bit
#define	 CLKPWR_PCONP_PCI2C0	((uint32_t)(1<<7))	// I2C0 clock enable bit
#define	 CLKPWR_PCONP_PCSPI		((uint32_t)(1<<8))	// SPI clock enable bit
#define	 CLKPWR_PCONP_PCRTC		((uint32_t)(1<<9))	// RTC clock enable bit
#define	 CLKPWR_PCONP_PCSSP1	((uint32_t)(1<<10))	// SSP1 clock enable bit
#define	 CLKPWR_PCONP_PCAD		((uint32_t)(1<<12))	// ADC clock enable bit
#define	 CLKPWR_PCONP_PCAN1		((uint32_t)(1<<13))	// CAN1 clock enable bit
#define	 CLKPWR_PCONP_PCAN2		((uint32_t)(1<<14))	// CAN2 clock enable bit
#define	CLKPWR_PCONP_PCGPIO		((uint32_t)(1<<15))	// GPIO clock enable bit
#define	CLKPWR_PCONP_PCRIT		((uint32_t)(1<<16))	// RIT clock enable bit
#define CLKPWR_PCONP_PCMC		((uint32_t)(1<<17))	// MC clock enable bit
#define CLKPWR_PCONP_PCQEI		((uint32_t)(1<<18))	// QE clock enable bit
#define	 CLKPWR_PCONP_PCI2C1	((uint32_t)(1<<19))	// I2C1 clock enable bit
#define	 CLKPWR_PCONP_PCSSP0	((uint32_t)(1<<21))	// SSP0 clock enable bit
#define	 CLKPWR_PCONP_PCTIM2	((uint32_t)(1<<22))	// Timer2 clock enable bit
#define	 CLKPWR_PCONP_PCTIM3	((uint32_t)(1<<23))	// Timer3 clock enable bit
#define	 CLKPWR_PCONP_PCUART2	((uint32_t)(1<<24))	// UART2 clock enable bit
#define	 CLKPWR_PCONP_PCUART3	((uint32_t)(1<<25))	// UART3 clock enable bit
#define	 CLKPWR_PCONP_PCI2C2	((uint32_t)(1<<26))	// I2C clock enable bit
#define	 CLKPWR_PCONP_PCI2S		((uint32_t)(1<<27))	// I2S clock enable bit
#define	 CLKPWR_PCONP_PCGPDMA	((uint32_t)(1<<29))	// GPDMA clock enable bit
#define	 CLKPWR_PCONP_PCENET	((uint32_t)(1<<30))	// Ethernet clock enable bit
#define	 CLKPWR_PCONP_PCUSB		((uint32_t)(1<<31))	// USB clock enable bit

void CLKPWR_SetPCLKDiv(uint32_t peripheral, uint32_t divider);
uint32_t CLKPWR_GetPCLKDiv(uint32_t peripheral);
uint32_t CLKPWR_GetPCLK(uint32_t peripheral);
void CLKPWR_EnablePPWR(uint32_t peripheral);
void CLKPWR_DisablePPWR(uint32_t peripheral);
void CLKPWR_Sleep(void);
void CLKPWR_DeepSleep(void);
void CLKPWR_PowerDown(void);
void CLKPWR_DeepPowerDown(void);

#endif // _LPC17XX_PCLKPWR_H_
