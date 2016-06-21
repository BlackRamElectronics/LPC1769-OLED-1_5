// ==== System Includes ===
#include <stdint.h>         // Include standard types

// ==== Project Includes ====
#include "BR_Font.h"
#include "BlackRam_EmbedGFX.h"

// ==== Definitions ====

// ==== Function Declerations ====
uint16_t AlphaBlend(uint16_t fg, uint16_t bg, uint8_t alpha);

// ==== Variable Declerations ====
BR_GFX_ENDIENNESS DisplayEndien = BR_GFX_LITTLE_ENDIAN;

//====================================================================================
BR_GFX_RET BR_GFX_Init(BR_GFX_ENDIENNESS endienness)
{
	DisplayEndien = endienness;
	return(BR_GFX_RET_OK);
}

//====================================================================================
// Set the requested pixel in the buffer to the blended colour
//====================================================================================
BR_GFX_RET BR_GFX_DrawPixel(uint16_t x, uint16_t y, BR_GFX_Colour colour, uint8_t alpha, BR_GFX_Canvas canvas)
{
    BR_GFX_Colour *pixel;

    // Check that the line fits in the display
    if((x > canvas.Width) | (y > canvas.Height))
    {
        return(BR_GFX_RET_OUT_BOUNDS);
    }

    // Get the pixel location in the buffer
    pixel = canvas.Buffer + x + (y * canvas.Width);

    // Blend the two colours and set the pixel
    *pixel = AlphaBlend(colour, *pixel, alpha);

    return(BR_GFX_RET_OK);
}

//====================================================================================
// Convert individual red, green and blue colour values to a single 16-bit representation
//====================================================================================
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t c;

	if(DisplayEndien == BR_GFX_LITTLE_ENDIAN)
	{
		// Use little endianness
		c = r >> 3;
		c <<= 6;
		c |= g >> 2;
		c <<= 5;
		c |= b >> 3;

		return((c >> 8)|(c << 8));
	}
	else
	{
		// Use big endianness
		c = r >> 3;
		c <<= 6;
		c |= g >> 2;
		c <<= 5;
		c |= b >> 3;

		return(c);
	}
}

//====================================================================================
// Perform blending of two colours weighted by alpha
//====================================================================================
uint16_t AlphaBlend(uint16_t fg, uint16_t bg, uint8_t alpha)
{
	uint16_t fg_temp, bg_temp, result;

	if(DisplayEndien == BR_GFX_LITTLE_ENDIAN)
	{
    	// Use little endianness
    	fg_temp = ((fg >> 8)|(fg << 8));
    	bg_temp = ((bg >> 8)|(bg << 8));
	}
	else
	{
    	// Use big endianness
    	fg_temp = fg;
    	bg_temp = bg;
    }

	// Split foreground into components
    unsigned fg_r = fg_temp >> 11;
    unsigned fg_g = (fg_temp >> 5) & ((1u << 6) - 1);
    unsigned fg_b = fg_temp & ((1u << 5) - 1);

    // Split background into components
    unsigned bg_r = bg_temp >> 11;
    unsigned bg_g = (bg_temp >> 5) & ((1u << 6) - 1);
    unsigned bg_b = bg_temp & ((1u << 5) - 1);

    // Alpha blend components
    unsigned out_r = (fg_r * alpha + bg_r * (255 - alpha)) / 255;
    unsigned out_g = (fg_g * alpha + bg_g * (255 - alpha)) / 255;
    unsigned out_b = (fg_b * alpha + bg_b * (255 - alpha)) / 255;

    // Pack result
    result = (unsigned short) ((out_r << 11) | (out_g << 5) | out_b);


	if(DisplayEndien == BR_GFX_LITTLE_ENDIAN)
	{
    	// Use little endianness
		return((result >> 8)|(result << 8));
	}
	else
	{
    	// Use big endianness
    	return(result);
    }
}

//====================================================================================
// Draw anti-aliased line
// Credit to: http://www.codeproject.com/Articles/13360/Antialiasing-Wu-Algorithm
//====================================================================================
BR_GFX_RET BR_GFX_DrawLine(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, uint16_t colour, BR_GFX_Canvas canvas)
{
    unsigned short IntensityShift, error_adj, error_acc;
    unsigned short error_acc_temp, weighting, WeightingComplementMask;
    short delta_x, delta_y, temp, x_dir;
    uint16_t x0 = start_x, x1 = end_x, y0 = start_y, y1 = end_y;

    // Check that the line fits in the display
    if((start_x > canvas.Width) | (end_x > canvas.Width) | (start_y > canvas.Height) | (end_y > canvas.Height))
    {
        return(BR_GFX_RET_OUT_BOUNDS);
    }

    // Make sure the line runs top to bottom
    if(start_y > end_y)
    {
        x0 = end_x;
        x1 = start_x;
        y0 = end_y;
        y1 = start_y;
    }

    // Draw the initial pixel, which is always exactly intersected by the line and so needs no weighting
    BR_GFX_DrawPixel(x0, y0, colour, 255, canvas);

    // Find the x and y distance of travel
    delta_x = x1 - x0;
    delta_y = y1 - y0;

    // What direction is our x going in
    if(delta_x >= 0)
    {
        x_dir = 1;
    }
    else
    {
        x_dir = -1;
        delta_x = -delta_x;   // make delta_x positive
    }

    // Special case horizontal, vertical, and diagonal lines, which require
    // no weighting because they go right through the centre of every pixel

    // If line is horizontal
    if(delta_y == 0)
    {
        while(delta_x-- != 0)
        {
            x0 += x_dir;
            BR_GFX_DrawPixel(x0, y0, colour, 255, canvas);
        }
        return(BR_GFX_RET_OK);
    }

    // If line is vertical
    if(delta_x == 0)
    {
        while(y0 <= y1)
        {
            BR_GFX_DrawPixel(x0, y0, colour, 255, canvas);
            y0++;
        }
        return(BR_GFX_RET_OK);
    }

    // If line is diagonal
    if(delta_x == delta_y)
    {
        while(y0 <= y1)
        {
            x0 += x_dir;
            y0++;
            BR_GFX_DrawPixel(x0, y0, colour, 255, canvas);
        }
        return(BR_GFX_RET_OK);
    }

    // Line is not horizontal, diagonal, or vertical
    error_acc = 0;  // initialize the line error accumulator to 0

    // Is this an X-major or Y-major line?
    if(delta_y > delta_x)
    {
      /* Y-major line; calculate 16-bit fixed-point fractional part of a
         pixel that X advances each time Y advances 1 pixel, truncating the
         result so that we won't overrun the endpoint along the X axis */
        error_adj = ((uint32_t)delta_x << 16) / (uint32_t)delta_y;

        // Draw all pixels other than the first and last
        while(--delta_y)
        {
            error_acc_temp = error_acc;   // remember current accumulated error
            error_acc += error_adj;      // calculate error for next pixel

            if(error_acc <= error_acc_temp)
            {
                // The error accumulator turned over, so advance the X coord
                x0 += x_dir;
            }

            y0++; // Y-major, so always advance Y

         /* The IntensityBits most significant bits of error_acc give us the
            intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
            weighting = error_acc >> 8;
            BR_GFX_DrawPixel(x0, y0, colour, 255 - (uint8_t)weighting, canvas);
            BR_GFX_DrawPixel(x0 + x_dir, y0, colour, (uint8_t)weighting, canvas);
        }

        // Draw the final pixel, which is always exactly intersected by the line and so needs no weighting
        BR_GFX_DrawPixel(x1, y1, colour, 255, canvas);
        return(BR_GFX_RET_OK);
    }

    /* It's an X-major line; calculate 16-bit fixed-point fractional part of a
        pixel that Y advances each time X advances 1 pixel, truncating the
        result to avoid overrunning the endpoint along the X axis */
    error_adj = ((uint32_t)delta_y << 16) / (uint32_t)delta_x;

    // Draw all pixels other than the first and last
    while(--delta_x)
    {
        error_acc_temp = error_acc;   // remember current accumulated error
        error_acc += error_adj;      // calculate error for next pixel
        if(error_acc <= error_acc_temp)
        {
            // The error accumulator turned over, so advance the Y coord
            y0++;
        }

        x0 += x_dir; // X-major, so always advance X
        /* The IntensityBits most significant bits of error_acc give us the
            intensity weighting for this pixel, and the complement of the
            weighting for the paired pixel */
        weighting = error_acc >> 8;
        BR_GFX_DrawPixel(x0, y0, colour, 255 - weighting, canvas);
        BR_GFX_DrawPixel(x0, y0 + 1, colour, weighting, canvas);
    }

    // Draw the final pixel, which is always exactly intersected by the line and so needs no weighting
    BR_GFX_DrawPixel(x1, y1, colour, 255, canvas);

    return(BR_GFX_RET_OK);
}

//====================================================================================
BR_GFX_RET BR_GFX_DrawRect(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y, BR_GFX_Colour colour, BR_GFX_Canvas canvas)
{
	// Create the rectangle from four lines
	
	// Draw top line
	BR_GFX_DrawLine(start_x, start_y, end_x, start_y, colour, canvas);
	
	// Draw bottom line
	BR_GFX_DrawLine(start_x, end_y, end_x, end_y, colour, canvas);
	
	// Draw left line
	BR_GFX_DrawLine(start_x, start_y, start_x, end_y, colour, canvas);
	
	// Draw right line
	BR_GFX_DrawLine(end_x, start_y, end_x, end_y, colour, canvas);
}

//====================================================================================
BR_GFX_RET BR_DrawCircleAA(uint16_t radius, uint16_t center_x, uint16_t center_y, BR_GFX_Colour colour, BR_GFX_Canvas canvas)
{
	/*uint16_t rad_squ;
    double i, j, remainder, inverse_remainder, integer;
    BR_TYPE_COLOUR_RGBA new_colour;
	BR_TYPE_POSITION pos;

    rad_squ = radius * radius;    // Store the radius squared as this will be used a number of times

    j = radius;
    for(i = 0; i < j; i++)  // This loop will continue for 1/8th of a circle
    {
        j = sqrt(rad_squ - (i * i));

        remainder = modf(j, &integer);
        inverse_remainder = 1.0 - remainder;

		pos.X = center_x + (uint16_t)i;
		pos.Y = center_y + (uint16_t)j;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x + (uint16_t)i;
		pos.Y = center_y + (uint16_t)j -1;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x + (uint16_t)j;
		pos.Y = center_y + (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x + (uint16_t)j - 1;
		pos.Y = center_y + (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x + (uint16_t)i;
		pos.Y = center_y - (uint16_t)j;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x + (uint16_t)i;
		pos.Y = center_y - (uint16_t)j + 1;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x + (uint16_t)j;
		pos.Y = center_y - (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x + (uint16_t)j - 1;
		pos.Y = center_y - (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x - (uint16_t)i;
		pos.Y = center_y + (uint16_t)j;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x - (uint16_t)i;
		pos.Y = center_y + (uint16_t)j - 1;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x - (uint16_t)j;
		pos.Y = center_y + (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
        pos.X = center_x - (uint16_t)j + 1;
		pos.Y = center_y + (uint16_t)i);
		BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x - (uint16_t)i;
		pos.Y = center_y - (uint16_t)j;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x - (uint16_t)i;
		pos.Y = center_y - (uint16_t)j + 1;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);

		pos.X = center_x - (uint16_t)j;
		pos.Y = center_y - (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, remainder);
        SetPixel(canvas, pos, new_colour);
		pos.X = center_x - (uint16_t)j + 1;
		pos.Y = center_y - (uint16_t)i;
        BlendColour(GetPixel(canvas, pos), colour, &new_colour, inverse_remainder);
        SetPixel(canvas, pos, new_colour);
    }*/

    return(BR_GFX_RET_OK);
}

//====================================================================================
BR_GFX_RET BR_GFX_DrawTextToBuffer(uint8_t *str, uint16_t colour, BR_Font font, uint16_t x, uint16_t y, BR_GFX_Canvas canvas)
{
    while(*str != 0)	// TODO: Check for out of bounds
    {
    	x += BR_GFX_DrawChar(*str++, colour, font, x, y, canvas);
    }
}

//====================================================================================
uint8_t BR_GFX_DrawChar(uint8_t val, uint16_t colour, BR_Font font, uint16_t x, uint16_t y, BR_GFX_Canvas canvas)
{
	BR_Glyph glyph;
	uint8_t *bitmap_ptr;
	uint16_t *buffer_ptr;
	uint16_t i, j;

	// TODO: change how we find our glyph
	glyph = font.GlyphList[val - ' '];

	bitmap_ptr = font.BitmapBuffer;

	bitmap_ptr += glyph.buffer_offset;

	for(i = 0; i < glyph.height; i++)
	{
		buffer_ptr = canvas.Buffer + (((y - glyph.top + i) * canvas.Width) + x + glyph.left);
		for(j = 0; j < glyph.width; j++)
		{
			*buffer_ptr = AlphaBlend(colour, *buffer_ptr, *bitmap_ptr);
			bitmap_ptr++;
			buffer_ptr++;
		}
	}

	return(glyph.hori_adv);
}

//====================================================================================
BR_GFX_RET BR_GFX_DrawImage(uint16_t *image_buffer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, BR_GFX_Canvas canvas)
{
	uint16_t current_x, current_y;
	uint16_t *buffer_ptr;
	
	// TODO: add out of bounds test
	
	for(current_y = y; current_y < (y + height); current_y++)
	{
		buffer_ptr = canvas.Buffer + x + (current_y * canvas.Width);
	
		for(current_x = x; current_x < (x + width); current_x++)
		{
			if(DisplayEndien == BR_GFX_LITTLE_ENDIAN)
			{
    			// Use little endianness
    			*buffer_ptr++ = ((*image_buffer >> 8)|(*image_buffer << 8));
    			image_buffer++;
			}
			else
			{
    			// Use big endianness
				*buffer_ptr++ = *image_buffer++;
    		}
		}
	}
}

