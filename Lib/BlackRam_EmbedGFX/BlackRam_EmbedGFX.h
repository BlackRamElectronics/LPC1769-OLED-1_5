
typedef BR_GFX_Colour uint16_t;

typedef struct BR_GFX_Canvas
{
    uint16_t Width;
    uint16_t Height;
    BR_GFX_Colour *Buffer;
}BR_GFX_Canvas;

typedef enum
{
    BR_GFX_RET_OK           = 0,
    BR_GFX_RET_ERROR        = 1,
    BR_GFX_RET_OUT_BOUNDS   = 2,
}BR_GFX_RET;

BR_GFX_RET BR_GFX_DrawPixel(uint16_t x, uint16_t y, BR_GFX_Colour colour, uint8_t alpha, BR_GFX_Canvas canvas);
BR_GFX_RET BR_GFX_DrawLine(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, BR_GFX_Colour colour, BR_GFX_Canvas canvas);
BR_GFX_RET BR_GFX_DrawTextToBuffer(uint8_t *str, uint16_t colour, BR_Font font, uint16_t x, uint16_t y, uint16_t *buffer);
BR_GFX_RET BR_GFX_DrawChar(uint8_t val, uint16_t colour, BR_Font font, uint16_t x, uint16_t y, uint16_t *buffer);
uint16_t BR_DrawCircleAA(uint16_t radius, uint16_t center_x, uint16_t center_y, BR_GFX_Colour colour, BR_GFX_Canvas canvas)
