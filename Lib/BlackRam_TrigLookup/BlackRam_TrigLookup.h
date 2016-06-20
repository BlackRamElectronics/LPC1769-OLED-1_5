#ifndef _BLACK_RAM_TRIG_LOOKUP_H_
#define _BLACK_RAM_TRIG_LOOKUP_H_

typedef enum
{
	BLACK_RAM_SIN	= 0,
	BLACK_RAM_COS	= 1
}BlackRam_TrigType;


// This function will return the sin/cos of the angle requested *! multiplied by 100 !*
int8_t BlackRam_GetTrig(uint16_t degrees, BlackRam_TrigType type);

#endif // _BLACK_RAM_TRIG_LOOKUP_H_
