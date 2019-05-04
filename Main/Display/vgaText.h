#ifndef _VGATEXT_H_
#define _VGATEXT_H_

#include "vgaCore.h"

namespace Vga
{
	extern const uint8_t* font;

    extern uint8_t cursor_x;
    extern uint8_t cursor_y;

	void InitVgaText(const uint8_t* f);

    void Print(char* str);
    void Print(char* str, uint32_t attribute);
    void Print(const char* str);
    void Print(const char* str, uint32_t attribute);
    void PrintUtf8(const char* str);
    void PrintChar(uint16_t x, uint16_t y, uint8_t ch);
    void PrintChar(uint16_t x, uint16_t y, uint8_t ch, uint32_t attribute);
    void SetAttribute(uint16_t x, uint16_t y, uint32_t attribute);

    void SetCursorPosition(uint8_t x, uint8_t y);
    void ShowCursor();
    void HideCursor();
}

#endif
