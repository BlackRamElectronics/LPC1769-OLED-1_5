//====================================================================================
// 0.96" Mono OLED Driver
// BlackRamElectronics.com
//====================================================================================

#include <stdint.h>			// Include standard types
#include "OLED_Driver.h"
#include "OLED_HWIF.h"
#include "Font_5x7.h"

//====================================================================================
// OLED Driver Commands
#define OLED_CMD_OFF			0xAE	// Turn display off
#define OLED_CMD_ON				0xAF	// Turn display on
#define OLED_CMD_ADD_MODE		0x20	// Set memory address mode
#define OLED_CMD_CLOCK			0xD5	// Configure clock
#define OLED_CMD_MULTIPLEXER	0xA8	// Set multiplexer ration, 1 - 64
#define OLED_CMD_OFFSET			0xD3	// Set the deisply offset
#define OLED_CMD_CHARGE_PUMP	0x8D	// Enable(0x14) or disable(0x10) the charge pump
#define OLED_CMD_START_LINE		0x40	// Set the display start line address
#define OLED_CMD_NORMAL_MODE	0xA6	// Set display to normal mode, use 0xA7 for inverse
#define OLED_CMD_INVERSE_MODE	0xA7	// See 0xA6
#define OLED_CMD_ENABLE_DISPLAY	0xA4	// Enable display the display will also need to be on
#define OLED_CMD_SEGMENT_REMAP	0xA1	// Segmant remap
#define OLED_CMD_SCAN_DIRECTION	0xC8	// Set scan direction
#define OLED_CMD_COM_PIN_CONFIG	0xDA	// Configure the com pins for the panel
#define OLED_CMD_SET_CONTRAST	0x81	// Set contrast level (0 - 255)
#define OLED_CMD_SET_PRE_CHARGE	0xD9	// Set pre-charge period
#define OLED_CMD_SET_VCOMH		0xDB	// Set the VCOMH regulator output

// Local function decleration
void SendCMD(uint8_t cmd);
void SendData(uint8_t data);
void SetPageAddress(uint8_t add);
void SetColumnAddress(uint8_t add);

// Variables
uint8_t Contrast_level = 128;

//====================================================================================
void SendCMD(uint8_t cmd)
{
	OLED_SetCommand();
	OLED_SendByte(cmd);
}

//====================================================================================
void SendData(uint8_t data)
{
	OLED_SetData();
	OLED_SendByte(data);
}

//====================================================================================
void InitOLED(void)
{
	OLED_InitIF();						// Setup hardware interface
	
	OLED_ResetAssert();					// Put display into reset
	OLED_MsDelay(200);					// Delay for 200ms
	
	OLED_ResetDeassert();				// Bring display out of reset
	OLED_MsDelay(200);					// Delay for 200ms
	
 	SendCMD(OLED_CMD_OFF);				// turn off oled panel

	SendCMD(OLED_CMD_CLOCK);			// set display clock divide ratio/oscillator frequency
	SendCMD(0x80);						// set divide ratio

	SendCMD(OLED_CMD_MULTIPLEXER);		// set multiplex ratio(1 to 64)
	SendCMD(0x3f);						// 1/64 duty

	SendCMD(OLED_CMD_OFFSET);			// set display offset
	SendCMD(0x00);						// not offset

	SendCMD(OLED_CMD_CHARGE_PUMP);		// set Charge Pump enable/disable
	SendCMD(0x14);						// set(0x10) disable

	SendCMD(OLED_CMD_START_LINE);		// set start line address

	SendCMD(OLED_CMD_NORMAL_MODE);		// set normal display
	
	SendCMD(OLED_CMD_ENABLE_DISPLAY);	// Disable Entire Display On

	SendCMD(OLED_CMD_SEGMENT_REMAP);	// set segment re-map 128 to 0

	SendCMD(OLED_CMD_SCAN_DIRECTION);	// Set COM Output Scan Direction 64 to 0

	SendCMD(OLED_CMD_COM_PIN_CONFIG);	// set com pins hardware configuration
	SendCMD(0x12);

	SendCMD(OLED_CMD_SET_CONTRAST);		// set contrast control register
	SendCMD(Contrast_level);

	SendCMD(OLED_CMD_SET_PRE_CHARGE);	// set pre-charge period
	SendCMD(0xf1);

	SendCMD(OLED_CMD_SET_VCOMH);		// set vcomh
	SendCMD(0x40);
	
	SendCMD(OLED_CMD_ADD_MODE);
	SendCMD(0x02);
	
	SendCMD(OLED_CMD_ON);				// turn on oled panel
}

//====================================================================================
void SetPageAddress(uint8_t add)
{
    SendCMD(0xB0 | add);
}

//====================================================================================
void SetColumnAddress(uint8_t add)
{
    SendCMD((0x10 | (add >> 4)));
	SendCMD((0x0f & add));
}

//====================================================================================
void DisplayImage(uint8_t *img)
{
    WriteBufferToDisplay(img);	// Image already correctly formatted so output
}

//====================================================================================
void DrawTextToBuffer(uint8_t line, uint8_t *str, uint8_t *buffer)
{
	uint8_t i = 0;
	
	while((*str != 0) && (i < (TEXT_CHARACTERS_PER_ROW -1)))
	{
		DrawChar(*str++, line, i++, buffer);
	}
}

//====================================================================================
void WriteBufferToDisplay(uint8_t *buffer)
{
	uint8_t i,j;
	
	for(i = 0; i < 0x08; i++)
	{
		SetPageAddress(i);
		SetColumnAddress(0x00);
        for(j = 0; j < 0x80; j++)
		{
			SendData(*buffer++);
		}
	}
}

//====================================================================================
void DrawChar(uint8_t val, uint32_t row, uint32_t column, uint8_t *buffer)
{
    uint8_t *char_ptr ;
    uint8_t char_row_data;
    uint32_t char_row, pixel_no;

	// Is the requested destination with in the display area
    if(((row + 1) > TEXT_ROWS) || ((column + 1) > TEXT_CHARACTERS_PER_ROW))
	{
        return;
    }

	// Get a pointer to the char in our font file
    char_ptr = (uint8_t *)&FONT_5x7;					// Init to start of font
    char_ptr += ((val - 0x20) * FONT_CELL_HEIGHT);		// Offset to the required char

	// Draw each row of the char
    for(char_row = 0; char_row < FONT_CELL_HEIGHT; char_row++)
	{
        char_row_data = *char_ptr;
        for(pixel_no = 0; pixel_no < FONT_CELL_WIDTH; pixel_no++)
		{
            if(char_row_data & 0x80)
			{
                SetPixel((column * FONT_CELL_WIDTH) + pixel_no, (row * FONT_CELL_HEIGHT) + char_row, 1, buffer);
            }
            else
			{
                SetPixel((column * FONT_CELL_WIDTH) + pixel_no, (row * FONT_CELL_HEIGHT) + char_row, 0, buffer);
            }
            char_row_data <<= 1;
        }
        char_ptr++;
    }
}

//====================================================================================
void SetPixel(uint32_t xpos, uint32_t ypos, uint32_t val, uint8_t *buffer)
{
	uint8_t *ptr;
	uint32_t bit_number;

	// Is the pixel out side of display area
	if((xpos >= X_PIXELS) || (ypos >= Y_PIXELS))
	{
        return;
    }

	ptr = buffer + xpos;
	ptr += 128 * (ypos / 8);
	bit_number = ypos % 8;

    if(val)
	{
		*ptr |= (1 << bit_number);
    }
    else
	{
		*ptr &= ~(1 << bit_number);
    }
}
