#include "ConsolePixelBuffer.h"

#define CBUFFER_PIXEL_DATA "\033[48;2;000;000;000m "
#define CBUFFER_POSITION_DATA "\033[000;000H"
#define CBUFFER_CLEAR_COLOR_DATA "\033[0m"

// -1 for removing null terminator
#define CBUFFER_PIXEL_SIZE (sizeof(CBUFFER_PIXEL_DATA) - 1)
#define CBUFFER_POSITION_SIZE (sizeof(CBUFFER_POSITION_DATA) - 1)
#define CBUFFER_CLEAR_COLOR_SIZE (sizeof(CBUFFER_CLEAR_COLOR_DATA) - 1)

// how many bytes until we hit each color value
#define CBUFFER_R_OFFSET 7
#define CBUFFER_G_OFFSET 11
#define CBUFFER_B_OFFSET 15

#define CBUFFER_X_OFFSET 6
#define CBUFFER_Y_OFFSET 2

// LUT table that contains pixel data as '000;' ... '255;'
#include "CBufferColorLUT.inl"

#include <stdio.h>
#include <stdlib.h>

// Writing
static void WritePositionData(CBufferSize_T x, CBufferSize_T y, char* data);
static void WritePixelData(uint8_t r, uint8_t g, uint8_t b, char* data);

void CreateConsoleViewport(ConsoleViewport* buffer)
{
	if (buffer == NULL)
		return;

	// The format per pixel is "\033[48;2;{R};{G};{B}m "
	// 48 is telling the console to set the background color
	// 2 is telling to use 24-bit color followed by r,g,b values
	// 5 would be 8-bit color

	// More info here https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

	// Must use decimal representation ie 255, 000, 064 etc
	// This makes the data 20 bytes per pixel...
	// Not ideal since it would normally be 3 so an almost 7x increase but 
	// we're rendering in the console so who cares!

	// For reposition the cursor for the next row this is used
	// "\033[{y};{x}H"

	buffer->data = NULL;

	buffer->size = buffer->height * ((buffer->width) * CBUFFER_PIXEL_SIZE + (CBUFFER_POSITION_SIZE + CBUFFER_CLEAR_COLOR_SIZE));

	buffer->data = malloc(buffer->size + 1); // + 1 full null terminator

	if (buffer->data == NULL)
		return;

	const char* colorSrc = CBUFFER_PIXEL_DATA;
	const char* newRowSrc = CBUFFER_POSITION_DATA;
	const char* clrColorSrc = CBUFFER_CLEAR_COLOR_DATA;

	CBufferIndex_T offset = 0; // Tracks the current byte we're writing too in the buffer
	for (CBufferSize_T y = 0; y < buffer->height; y++)
	{
		// Write reposition cursor data
		memcpy(buffer->data + offset, newRowSrc, CBUFFER_POSITION_SIZE);
		WritePositionData(buffer->x, buffer->y + y, buffer->data + offset);
		offset += CBUFFER_POSITION_SIZE;

		for (CBufferSize_T x = 0; x < buffer->width; x++)
		{
			memcpy(buffer->data + offset, colorSrc, CBUFFER_PIXEL_SIZE);
			offset += CBUFFER_PIXEL_SIZE;
		}

		// Write clear color data
		memcpy(buffer->data + offset, clrColorSrc, CBUFFER_CLEAR_COLOR_SIZE);
		offset += CBUFFER_CLEAR_COLOR_SIZE;
	}

	buffer->data[offset] = '\0';
}

void ClearConsoleViewport(ConsoleViewport* buffer)
{
	if (buffer == NULL)
		return;

	free(buffer->data);
	buffer->data = NULL;

	buffer->x = 0;
	buffer->y = 0;
	buffer->width = 0;
	buffer->height = 0;
	buffer->size = 0;
}

void WriteConsoleViewportPixel(ConsoleViewport* buffer, CBufferOffset_T x, CBufferOffset_T y, uint8_t r, uint8_t g, uint8_t b)
{
	if (buffer == NULL)
		return;

	if (x >= buffer->width || y >= buffer->height)
		return;

	CBufferIndex_T offset = y * ((buffer->width) * CBUFFER_PIXEL_SIZE + (CBUFFER_POSITION_SIZE + CBUFFER_CLEAR_COLOR_SIZE))
		+ CBUFFER_POSITION_SIZE
		+ x * CBUFFER_PIXEL_SIZE;

	WritePixelData(r, g, b, buffer->data + offset);
}

static void WritePositionData(CBufferSize_T x, CBufferSize_T y, char* data)
{
	if (y > 999) // Enforce max size
		memcpy(data + CBUFFER_Y_OFFSET, "999", 3);
	else
	{
		data[CBUFFER_Y_OFFSET]		= '0' + (y / 100) % 10;
		data[CBUFFER_Y_OFFSET + 1]	= '0' + (y / 10) % 10;
		data[CBUFFER_Y_OFFSET + 2]	= '0' + y % 10;
	}

	if (x > 999) // Enforce max size
		memcpy(data + CBUFFER_X_OFFSET, "999", 3);
	else
	{
		data[CBUFFER_X_OFFSET] = '0' + (x / 100) % 10;
		data[CBUFFER_X_OFFSET + 1] = '0' + (x / 10) % 10;
		data[CBUFFER_X_OFFSET + 2] = '0' + x % 10;
	}
}

static void WritePixelData(uint8_t r, uint8_t g, uint8_t b, char* data)
{
	memcpy(data + CBUFFER_R_OFFSET, &cbufferDecimalLUT[r], 3);
	memcpy(data + CBUFFER_G_OFFSET, &cbufferDecimalLUT[g], 3);
	memcpy(data + CBUFFER_B_OFFSET, &cbufferDecimalLUT[b], 3);
}
