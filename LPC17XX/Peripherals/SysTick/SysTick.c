#include "LPC17xx.h"
#include "SysTick.h"

volatile uint32_t msTicks = 0; // counter for 1ms SysTicks

//====================================================================================
void SysTickInit(uint32_t period)
{
	SysTick_Config(period);		// Generate interrupt every 1 ms
}
	
//====================================================================================
void SysTick_Handler(void)
{
	msTicks++;
}

//====================================================================================
void MsDelay(uint32_t ms)
{
	uint32_t timer_mark;

	timer_mark = msTicks;					// Take timer snapshot 
	while(msTicks < (timer_mark + ms));	// Wait until ms has passed
}

