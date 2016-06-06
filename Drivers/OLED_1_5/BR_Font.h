#ifndef _BR_FONT_
#define _BR_FONT_

typedef struct BR_Glyph
{
	uint8_t	    chr;
	uint8_t	    height;
	uint8_t	    width;
	int8_t	    left;
	int8_t	    top;
	uint8_t		hori_adv;
	uint8_t		vert_adv;
	uint16_t	buffer_offset;
}BR_Glyph;

typedef struct BR_Font
{
    BR_Glyph *GlyphList;
    uint8_t *BitmapBuffer;
}BR_Font;

#endif
