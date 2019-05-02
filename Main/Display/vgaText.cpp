#include "vgatext.h"
#include "startup.h"
#include "normalAttribute.h"
#include "inversedAttribute.h"

namespace Vga
{
const uint8_t* font;
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
bool cursor_visible = false;
uint8_t charFromAddress(uint32_t address);
uint32_t addressFromChar(uint8_t character);
}

void Vga::ShowCursor()
{
	if (!cursor_visible)
	{

		ScreenAttributes[cursor_y * settings->TextColumns + cursor_x] = (uint32_t)inversedAttribute;
		cursor_visible = true;
	}
}

void Vga::HideCursor()
{
	if (cursor_visible)
	{
		ScreenAttributes[cursor_y * settings->TextColumns + cursor_x] = (uint32_t)normalAttribute;
		cursor_visible = false;
	}
}

void Vga::InitVgaText(const uint8_t* f)
{
	font = f;
}

void Vga::SetCursorPosition(uint8_t x, uint8_t y)
{
	bool cursorShown = cursor_visible;
	if (cursorShown)
	{
		HideCursor();
	}

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

	if (cursorShown)
	{
		ShowCursor();
	}
}

void Vga::PrintChar(uint16_t x, uint16_t y, uint8_t ch)
{
	if (x >= settings->TextColumns || y >= settings->TextRows)
	{
		// Invalid
		return;
	}

	ScreenCharacters[y * settings->TextColumns + x] = ch;
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
	bool cursorShown = cursor_visible;
	if (cursorShown)
	{
		HideCursor();
	}

    while (*str)
    {
    	PrintChar(cursor_x, cursor_y, *str++);
    	cursorNext();
    }

	if (cursorShown)
	{
		ShowCursor();
	}
}

uint8_t Vga::charFromAddress(uint32_t address)
{
	return (address - (uint32_t)font) >> 3;
}

uint32_t Vga::addressFromChar(uint8_t character)
{
	return (uint32_t)font + ((uint32_t)character << 3);
}

