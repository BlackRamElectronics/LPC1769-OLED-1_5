#ifndef _OLED_DRIVER_H_
#define _OLED_DRIVER_H_

#define X_PIXELS                   	128	// Display width
#define Y_PIXELS                    64	// Display Height
#define TEXT_CHARACTERS_PER_ROW     21	// Number of text characters per row
#define TEXT_ROWS                   8	// Number of rows of text

// Setup the hardware interface and configure teh display
void InitOLED(void);

// Draw the img buffer to the display
void DisplayImage(uint8_t *img);

// Output the graphic data to the display
void WriteBufferToDisplay(uint8_t *buffer);

// Draw a string to the buffer at teh line requested
void DrawTextToBuffer(uint8_t line, uint8_t *str, uint8_t *buffer);

// Draw a text character at the text cell requested
void DrawChar(uint8_t val, uint32_t row, uint32_t column, uint8_t *buffer);

// Set or clear a pixel in the buffer provided
void SetPixel(uint32_t xpos, uint32_t ypos, uint32_t val, uint8_t *buffer);

#endif // _OLED_DRIVER_H_
