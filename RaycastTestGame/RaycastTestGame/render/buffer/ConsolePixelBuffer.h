#ifndef CONSOLE_BUFFER_H
#define CONSOLE_BUFFER_H
	
#include <stdint.h>

typedef uint8_t CBufferOffset_T;
typedef uint16_t CBufferSize_T;
typedef uint32_t CBufferIndex_T; // Must be 2x buffer size type

typedef struct
{
	char* data;
	CBufferOffset_T x;
	CBufferOffset_T y;
	CBufferSize_T width;
	CBufferSize_T height;
	CBufferIndex_T size;
} ConsoleViewport;

#ifdef __cplusplus // Prevents mangling
extern "C" {
#endif

void CreateConsoleViewport(ConsoleViewport* buffer);
void ClearConsoleViewport(ConsoleViewport* buffer);

void WriteConsoleViewportPixel(ConsoleViewport* buffer, CBufferOffset_T x, CBufferOffset_T y, uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif