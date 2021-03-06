/*#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include "OLED_Driver.h"
#include "SysTick.h"
#include "string.h"*/

#include "lpc17xx_spi.h"
#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "OLED_Driver.h"
#include "BR_SysTick.h"

// Image data files
//#include "BR_logo_anim.h"
//#include "BR_Logo_LCD.h"

void SplashScreen(void);
void TextDemo(void);
void ImageDemo(void);
void AnimationDemo(void);

// Graphics buffer for OLED display
//uint8_t DisplayBuffer [(X_PIXELS * Y_PIXELS) / 8];

// PORT and PIN numbers that LED pin assigned on
#define LED_PORT_NUM	1
#define LED_PIN_NUM		(1<<18)

//====================================================================================
int main()
{
	// Init SysTick
	BR_SysTickInit();

	// Init on-board LED as output
	GPIO_SetDir(LED_PORT_NUM, LED_PIN_NUM, 1);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_ClearValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_ClearValue(LED_PORT_NUM, LED_PIN_NUM);
	BR_MsDelay(100);

	GPIO_SetValue(LED_PORT_NUM, LED_PIN_NUM);

	// Init the OLED display and required hardware
	InitOLED();

	for(;;)
	{
		//SplashScreen();
		//TextDemo();
		//ImageDemo();
		//AnimationDemo();
	}

	return(0);
}

//====================================================================================
void SplashScreen(void)
{
/*	memset(DisplayBuffer, 0, sizeof(DisplayBuffer));

	DrawTextToBuffer(0, " 0.96\" OLED Display  ", DisplayBuffer);
	DrawTextToBuffer(2, "      BLACK RAM       ", DisplayBuffer);
	DrawTextToBuffer(3, "     ELECTRONICS      ", DisplayBuffer);
	DrawTextToBuffer(5, "Dev Boards & Displays ", DisplayBuffer);
	DrawTextToBuffer(6, "        With          ", DisplayBuffer);
	DrawTextToBuffer(7, "Examples & Tutorials  ", DisplayBuffer);

	WriteBufferToDisplay(DisplayBuffer);
	MsDelay(10000);*/
}

//====================================================================================
void TextDemo(void)
{
/*	uint32_t row = 0, col = 0, i;

	memset(DisplayBuffer, 0, sizeof(DisplayBuffer));
	DrawTextToBuffer(3, "      TEXT DEMO      ", DisplayBuffer);
	WriteBufferToDisplay(DisplayBuffer);
	MsDelay(1000);

	memset(DisplayBuffer, 0, sizeof(DisplayBuffer));
	for(i = 0; i <= 96; i++)
	{
		DrawChar(33 + i, row, col++, DisplayBuffer);
		if(col >= TEXT_CHARACTERS_PER_ROW)
		{
			row++;
			col = 0;
		}
	}

	WriteBufferToDisplay(DisplayBuffer);
	MsDelay(2000);*/
}

//====================================================================================
void ImageDemo(void)
{
	/*memset(DisplayBuffer, 0, sizeof(DisplayBuffer));
	DrawTextToBuffer(3, "     IMAGE DEMO      ", DisplayBuffer);
	WriteBufferToDisplay(DisplayBuffer);
	MsDelay(1000);

	DisplayImage(BR_LOGO_LCD);
	MsDelay(2000);*/
}

//====================================================================================
void AnimationDemo(void)
{
	/*uint32_t frame_index;

	// Splash screen
	memset(DisplayBuffer, 0, sizeof(DisplayBuffer));
	DrawTextToBuffer(3, "   ANIMATION DEMO    ", DisplayBuffer);
	WriteBufferToDisplay(DisplayBuffer);
	MsDelay(1000);

	for(frame_index = 0; frame_index < FRAME_COUNT; frame_index++)
	{
		MsDelay(FRAME_DEALY);
		DisplayImage((unsigned char*)BR_LOGO_ANIM[frame_index]);
	}
	MsDelay(2000);*/
}
