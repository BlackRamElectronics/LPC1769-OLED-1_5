// ==== System Includes ===

// ==== Project Includes ====
#include "clock_demo.h"
#include "BlackRam_TrigLookup.h"
#include "ClockFace.h"

// ==== Definitions ====
#define HOUR_LENGTH = 30
#define MINUTE_LENGTH = 59
#define SECOND_LENGTH = 59

// ==== Function Decelerations ====

// ==== Variable Deceleration ====
uint8_t Hours = 0;
uint8_t Minutes = 0;
uint8_t Seconds = 0;

//====================================================================================
void ClockDemo_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	Hours = hours;
	Minutes = minutes;
	Seconds = seconds;
}

//====================================================================================
void ClockDemo_1sUpdate(uint8_t hours, uint8_t minutes, uint8_t seconds, BR_GFX_Canvas canvas)
{

	// Update the time
	Seconds++;
	if(Seconds >= 60)
	{
		Seconds = 0;
		Minutes++;
		if(Minutes >= 60)
		{
			Minutes = 0;
			Hours++;
			if(Hours >= 24)
			{
				Hours = 0;
			}
		}
	}

	// Draw the background image
	BR_GFX_DrawImage(BR_ImageBuffer_clock, 0, 0, 128, 128, canvas);
	
	// Draw the hour hand
	BR_GFX_DrawLine(63, 63, BlackRam_GetTrig(hours * (360/24), BLACK_RAM_COS) * HOUR_LENGTH, BlackRam_GetTrig(hours * (360/24), BLACK_RAM_SIN) * HOUR_LENGTH, Color565(255,0,0), canvas);
	
	// Draw the minute hand
	BR_GFX_DrawLine(63, 63, BlackRam_GetTrig(minutes * (360/60), BLACK_RAM_COS) * MINUTE_LENGTH, BlackRam_GetTrig(minutes * (360/60), BLACK_RAM_SIN) * MINUTE_LENGTH, Color565(0,255,0), canvas);
	
	// Draw the second hand
	BR_GFX_DrawLine(63, 63, BlackRam_GetTrig(seconds * (360/60), BLACK_RAM_COS) * SECOND_LENGTH, BlackRam_GetTrig(seconds * (360/60), BLACK_RAM_SIN) * SECOND_LENGTH, Color565(0,0,255), canvas);
}


