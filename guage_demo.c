// ==== System Includes ===
#include <stdint.h>         // Include standard types
#include <stdio.h>

// ==== Project Includes ====
#include "BlackRam_EmbedGFX.h"
#include "BR_Font_OpenSans16p.h"
#include "guage_demo.h"

// ==== Definitions ====

// ==== Function Decelerations ====

// ==== Variable Deceleration ====


//====================================================================================
void DrawGauge(uint8_t value, uint8_t x_offset, uint8_t width, BR_GFX_Canvas canvas)
{
	uint8_t i = 0;
	uint8_t str_buffer[6];
	
	// Draw the boxes for the gauges
	BR_GFX_DrawRect(x_offset, 0, x_offset + width + 1, 102,  Color565(255,255,255), canvas);
	
	// Fill the gauge with a gradient
	while((i < value) && (i < 100))
	{
		BR_GFX_DrawLine(x_offset + 1, (100 - i) + 1, x_offset + width, (100 - i) + 1, Color565((i * (255/100)),((100 - i) * (255/100)),0), canvas);	
		i++;
	}
	
	// Print the value to the screen
	sprintf(str_buffer, "%d", value);
	BR_GFX_DrawTextToBuffer(str_buffer, Color565(64, 64, 255), BR_OpenSans16p, x_offset, 125, canvas);
}

//====================================================================================
void UpdateGaugeDemo(uint8_t gauge1, uint8_t gauge2, BR_GFX_Canvas canvas)
{
	DrawGauge(gauge1, 30, 16, canvas);
	DrawGauge(gauge2, 80, 16, canvas);
}
