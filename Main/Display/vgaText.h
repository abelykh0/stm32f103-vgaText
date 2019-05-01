#ifndef _VGATEXT_H_
#define _VGATEXT_H_

#include "vgaCore.h"

namespace Vga
{
	extern const uint8_t* font;

    extern uint8_t cursor_x;
    extern uint8_t cursor_y;

	void InitVgaText(const uint8_t* f);
    void SetCursorPosition(uint8_t x, uint8_t y);
    void Print(char* str);
    void Print(const char* str);
    void PrintChar(uint16_t x, uint16_t y, uint8_t ch);
    void ShowCursor();
    void HideCursor();
    uint8_t charFromAddress(uint32_t address);
    uint32_t addressFromChar(uint8_t character);
}

#endif
