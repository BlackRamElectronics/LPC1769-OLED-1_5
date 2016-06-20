// ==== System Includes ===
#include <stdint.h>         // Include standard types
#include <stdio.h>

// ==== Project Includes ====
#include "BlackRam_EmbedGFX.h"
#include "BR_Font_OpenSans12p.h"
#include "guage_demo.h"

// ==== Definitions ====

// ==== Function Decelerations ====

// ==== Variable Deceleration ====

//====================================================================================
void UpdateGaugeDemo(uint8_t gauge1, uint8_t gauge2, BR_GFX_Canvas canvas)
{
	uint8_t i = 0;
	uint8_t str_buffer[6];
	
	// Draw the boxes for the gauges
	BR_GFX_DrawRect(13, 13, 25, 115,  Color565(255,255,255), canvas);
	
	// Fill the gauge with a gradient
	while((i < gauge1) && (i < 100))
	{
		BR_GFX_DrawRect(14, i + 14, 24, i + 14, Color565((i * (255/100)),((100 - i) * (255/100)),0), canvas);
		
		i++;
	}
	
	// Print the value to the screen
	sprintf(str_buffer, "%d", gauge1);
	BR_GFX_DrawTextToBuffer(str_buffer, Color565(64, 64, 255), BR_OpenSans12p, 30, 30, canvas);
}
