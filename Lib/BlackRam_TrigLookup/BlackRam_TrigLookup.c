// ==== System Includes ===
#include <stdint.h>         // Include standard types

// ==== Project Includes ====
#include "BlackRam_TrigLookup.h"

//====================================================================================
// The following table is the cosine values of 0-90 degrees at 1 degree steps,
// the values are multiplied by 100 to avoid using floating point arithmetic
const uint8_t TrigLookupTable[] =
{
	100, 100, 100, 100, 100, 100,  99,  99,
	 99,  99,  98,  98,  98,  97,  97,  97,
	 96,  96,  95,  95,  94,  93,  93,  92,
	 91,  91,  90,  89,  88,  87,  87,  86,
	 85,  84,  83,  82,  81,  80,  79,  78,
	 77,  75,  74,  73,  72,  71,  69,  68,
	 67,  66,  64,  63,  62,  60,  59,  57,
	 56,  54,  53,  52,  50,  48,  47,  45,
	 44,  42,  41,  39,  37,  36,  34,  33,
	 31,  29,  28,  26,  24,  22,  21,  19,
	 17,  16,  14,  12,  10,   9,   7,   5,
	  3,   2,   0
};

//====================================================================================
uint8_t BlackRam_GetTrig(uint16_t degrees, BlackRam_TrigType trig_type)
{
	int8_t cos_adjust = 1, sin_adjust = 1;
	uint16_t degree_temp = degees;
	
	// Ensure the degree value is less than 360
	while(degree_temp >= 360)
	{
		degree_temp -= 360;
	}
	
	// Between 91 and 180
	if((degree_temp > 90) && (degree_temp <= 180))
	{
		degree_temp = 180 - degree_temp;
		cos_adjust = -1;
	}
	
	// Between 181 and 270
	if((degree_temp > 180) && (degree_temp <= 270))
	{ 
		degree_temp = degree_temp - 180;
		cos_adjust = -1;
		sin_adjust = -1;
	}

	// Between 271 and 360
	if((degree_temp > 270) && (degree_temp <= 360))
	{ 
		degree_temp = 360 - degree_temp;
		sin_adjust = -1;
	}
	
	if(trig_type == BLACK_RAM_COS) // Cosine
	{
		return(cos_adjust * TrigLookupTable[degree_temp]);
	}
	else //sine
	{
		return(sin_adjust * TrigLookupTable[90 - degree_temp]);
	}
}