#include "vgatext.h"
#include "startup.h"

namespace Vga
{
const uint8_t* font;
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
}

void Vga::InitVgaText(const uint8_t* f)
{
	font = f;
}

void Vga::SetCursorPosition(uint8_t x, uint8_t y)
{
	cursor_x = x;
	cursor_y = y;
	if (cursor_x >= settings->TextColumns)
	{
		cursor_x = settings->TextColumns - 1;
	}
	if (cursor_y >= settings->TextRows)
	{
		cursor_y = settings->TextRows - 1;
	}
}

void Vga::PrintChar(uint16_t x, uint16_t y, uint8_t ch)
{
	if (x >= Vga::settings->TextColumns || y >= Vga::settings->TextRows)
	{
		// Invalid
		return;
	}

	Vga::videoMemoryPixels[y * Vga::settings->TextColumns + x] = ch;
}

static void cursorNext()
{
    uint8_t x = Vga::cursor_x;
    uint8_t y = Vga::cursor_y;
    if (x < Vga::settings->TextColumns - 1)
    {
        x++;
    }
    else
    {
        if (y < Vga::settings->TextRows - 1)
        {
            x = 0;
            y++;
        }
    }
    Vga::SetCursorPosition(x, y);
}

void Vga::Print(const char* str)
{
	Print((char*)str);
}

void Vga::Print(char* str)
{
    while (*str)
    {
    	PrintChar(cursor_x, cursor_y, *str++);
    	cursorNext();
    }
}
