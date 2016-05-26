//====================================================================================
// 0.96" Mono OLED Driver
// BlackRamElectronics.com
//====================================================================================

#include <stdint.h>         // Include standard types
#include "OLED_Driver.h"
#include "OLED_HWIF.h"
#include "Font_5x7.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT 128  // SET THIS TO 96 FOR 1.27"!


//====================================================================================
// OLED Driver Commands
#define OLED_CMD_OFF            0xAE    // Turn display off
#define OLED_CMD_ON             0xAF    // Turn display on
#define OLED_CMD_ADD_MODE       0x20    // Set memory address mode
#define OLED_CMD_CLOCK          0xD5    // Configure clock
#define OLED_CMD_MULTIPLEXER    0xA8    // Set multiplexer ration, 1 - 64
#define OLED_CMD_OFFSET         0xD3    // Set the deisply offset
#define OLED_CMD_CHARGE_PUMP    0x8D    // Enable(0x14) or disable(0x10) the charge pump
#define OLED_CMD_START_LINE     0x40    // Set the display start line address
#define OLED_CMD_NORMAL_MODE    0xA6    // Set display to normal mode, use 0xA7 for inverse
#define OLED_CMD_INVERSE_MODE   0xA7    // See 0xA6
#define OLED_CMD_ENABLE_DISPLAY 0xA4    // Enable display the display will also need to be on
#define OLED_CMD_SEGMENT_REMAP  0xA1    // Segmant remap
#define OLED_CMD_SCAN_DIRECTION 0xC8    // Set scan direction
#define OLED_CMD_COM_PIN_CONFIG 0xDA    // Configure the com pins for the panel
#define OLED_CMD_SET_CONTRAST   0x81    // Set contrast level (0 - 255)
#define OLED_CMD_SET_PRE_CHARGE 0xD9    // Set pre-charge period
#define OLED_CMD_SET_VCOMH      0xDB    // Set the VCOMH regulator output

// Local function decleration
void SendCMD(uint8_t cmd);
//void SendData(uint16_t data);
void SendData(uint8_t data);
void SetPageAddress(uint8_t add);
void SetColumnAddress(uint8_t add);

void ColorSet(uint32_t ptr1);

// Variables
uint8_t Contrast_level = 128;


// SSD1351 Commands
#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET   0xA2
#define SSD1351_CMD_DISPLAYALLOFF   0xA4
#define SSD1351_CMD_DISPLAYALLON    0xA5
#define SSD1351_CMD_NORMALDISPLAY   0xA6
#define SSD1351_CMD_INVERTDISPLAY   0xA7
#define SSD1351_CMD_FUNCTIONSELECT  0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON       0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL      0xB4
#define SSD1351_CMD_SETGPIO         0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT      0xB9
#define SSD1351_CMD_PRECHARGELEVEL  0xBB
#define SSD1351_CMD_VCOMH       0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO            0xCA
#define SSD1351_CMD_COMMANDLOCK         0xFD
#define SSD1351_CMD_HORIZSCROLL         0x96
#define SSD1351_CMD_STOPSCROLL          0x9E
#define SSD1351_CMD_STARTSCROLL         0x9F

__attribute__((section("RAM2"))) uint16_t OLED_Buffer[OLED_WIDTH * OLED_HEIGHT];

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
    OLED_SendByte((uint8_t)data);
}

//====================================================================================
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t c;
    c = r >> 3;
    c <<= 6;
    c |= g >> 2;
    c <<= 5;
    c |= b >> 3;

    return c;
}

//====================================================================================
void rawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fillcolor)
{
    uint16_t i;

    // Bounds check
    if((x >= OLED_WIDTH) || (y >= OLED_HEIGHT))
    {
        return;
    }

    // Y bounds check
    if(y+h > OLED_HEIGHT)
    {
        h = OLED_HEIGHT - y - 1;
    }

    // X bounds check
    if((x + w) > OLED_WIDTH)
    {
        w = OLED_WIDTH - x - 1;
    }

    // set location
    SendCMD(SSD1351_CMD_SETCOLUMN);
    SendData(x);
    SendData(x+w-1);
    SendCMD(SSD1351_CMD_SETROW);
    SendData(y);
    SendData(y+h-1);
    SendCMD(SSD1351_CMD_WRITERAM);

    for(i = 0; i < (w * h); i++)
    {
        SendData(fillcolor >> 8);
        SendData(fillcolor);
    }
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fillcolor)
{
  // Transform x and y based on current rotation.
  //switch (getRotation()) {
  //case 0:  // No rotation
    rawFillRect(x, y, w, h, fillcolor);
    //break;
  /*case 1:  // Rotated 90 degrees clockwise.
    swap(x, y);
    x = WIDTH - x - h;
    rawFillRect(x, y, h, w, fillcolor);
    break;
  case 2:  // Rotated 180 degrees clockwise.
    x = WIDTH - x - w;
    y = HEIGHT - y - h;
    rawFillRect(x, y, w, h, fillcolor);
    break;
  case 3:  // Rotated 270 degrees clockwise.
    swap(x, y);
    y = HEIGHT - y - w;
    rawFillRect(x, y, h, w, fillcolor);
    break;
  }*/
}

//====================================================================================
void fillScreen(uint16_t fillcolor)
{
	uint16_t i;

    //fillRect(0, 0, OLED_WIDTH, OLED_HEIGHT, fillcolor);

	// Fill the buffer with the requested color
	//memset(OLED_Buffer, 0xffff, 1024);
	/*for(i = 0; i < 15000; i++)
	{
		OLED_Buffer[i] = fillcolor;
	}*/
	// Set OLED write location
    /*SendCMD(SSD1351_CMD_SETCOLUMN);
    SendData(0);
    SendData(OLED_WIDTH);
    SendCMD(SSD1351_CMD_SETROW);
    SendData(0);
    SendData(OLED_HEIGHT);
    SendCMD(SSD1351_CMD_WRITERAM);
	
	// Send the data buffer
	OLED_SetData();
	OLED_SendBuffer(OLED_Buffer, sizeof(OLED_Buffer));*/
}

//====================================================================================
void InitOLED(void)
{
    OLED_InitIF();                      // Setup hardware interface

    OLED_ResetAssert();                 // Put display into reset
    OLED_MsDelay(200);                  // Delay for 200ms

    OLED_ResetDeassert();               // Bring display out of reset
    OLED_MsDelay(200);                  // Delay for 200ms

    // Initialization Sequence
    SendCMD(SSD1351_CMD_COMMANDLOCK);  // set command lock
    SendData(0x12);
    SendCMD(SSD1351_CMD_COMMANDLOCK);  // set command lock
    SendData(0xB1);

    SendCMD(SSD1351_CMD_DISPLAYOFF);       // 0xAE

    SendCMD(SSD1351_CMD_CLOCKDIV);         // 0xB3
    SendCMD(0xF1);                         // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)

    SendCMD(SSD1351_CMD_MUXRATIO);
    SendData(127);

    SendCMD(SSD1351_CMD_SETREMAP);
    SendData(0x74);

    SendCMD(SSD1351_CMD_SETCOLUMN);
    SendData(0x00);
    SendData(0x7F);
    SendCMD(SSD1351_CMD_SETROW);
    SendData(0x00);
    SendData(0x7F);

    SendCMD(SSD1351_CMD_STARTLINE);        // 0xA1
    if(OLED_HEIGHT == 96)
    {
        SendData(96);
    }
    else
    {
        SendData(0);
    }

    SendCMD(SSD1351_CMD_DISPLAYOFFSET);    // 0xA2
    SendData(0x0);

    SendCMD(SSD1351_CMD_SETGPIO);
    SendData(0x00);

    SendCMD(SSD1351_CMD_FUNCTIONSELECT);
    SendData(0x01); // internal (diode drop)

    SendCMD(SSD1351_CMD_PRECHARGE);        // 0xB1
    SendCMD(0x32);

    SendCMD(SSD1351_CMD_VCOMH);            // 0xBE
    SendCMD(0x05);

    SendCMD(SSD1351_CMD_NORMALDISPLAY);    // 0xA6

    SendCMD(SSD1351_CMD_CONTRASTABC);
    SendData(0xC8);
    SendData(0x80);
    SendData(0xC8);

    SendCMD(SSD1351_CMD_CONTRASTMASTER);
    SendData(0x0F);

    SendCMD(SSD1351_CMD_SETVSL);
    SendData(0xA0);
    SendData(0xB5);
    SendData(0x55);

    SendCMD(SSD1351_CMD_PRECHARGE2);
    SendData(0x01);

    SendCMD(SSD1351_CMD_DISPLAYON);        //--turn on oled panel

    while(1)
    {
        fillScreen(Color565(0xFF, 0x00, 0x00));
        OLED_MsDelay(1000);
        fillScreen(Color565(0x00, 0xFF, 0x00));
        OLED_MsDelay(1000);
        fillScreen(Color565(0x00, 0x00, 0xFF));
        OLED_MsDelay(1000);
        fillScreen(Color565(0xFF, 0xFF, 0x00));
        OLED_MsDelay(1000);
        fillScreen(Color565(0x00, 0xFF, 0xFF));
        OLED_MsDelay(1000);
        fillScreen(Color565(0xFF, 0x00, 0xFF));
        OLED_MsDelay(1000);
    }

    /*while(1)
    {
        for(int i = 0; i <
    }*/
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
void ColorSet(uint32_t ptr1)
{
    uint8_t i, j;

    SendCMD(0x15); //set column
    SendCMD(0x00);
    SendCMD(0x7f);

    SendCMD(0x75); //set row
    SendCMD(0x00);
    SendCMD(0x7f);

    SendCMD(0x5c);  //Enable MCU to write Data into RAM

    for(j=0;j<256;j++)
    {
        SendData(0xff);
    }

    for(i=0;i<126;i++)
    {
        SendData(0xffff);
        for(j=0;j<126;j++)
        {
            SendData(ptr1);
        }
        SendData(0xffff);
    }

    for(j=0;j<128;j++)
    {
        SendData(0xffff);
    }

    OLED_MsDelay(700);
}

//====================================================================================
void DisplayImage(uint8_t *img)
{
    WriteBufferToDisplay(img);  // Image already correctly formatted so output
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
    char_ptr = (uint8_t *)&FONT_5x7;                    // Init to start of font
    char_ptr += ((val - 0x20) * FONT_CELL_HEIGHT);      // Offset to the required char

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
