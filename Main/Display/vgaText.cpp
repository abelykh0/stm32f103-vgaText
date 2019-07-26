#include <string.h>
#include "vgatext.h"
#include "startup.h"

namespace Vga
{
const uint8_t* font;
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
bool cursor_visible = false;
void utf8ToCP866(char* utf8, char* result);
void cursorNext(uint32_t attribute);
}

void Vga::ShowCursor()
{
	if (!cursor_visible)
	{
		ScreenAttributes[cursor_y * settings->TextColumns + cursor_x] += 16 * 4;
		cursor_visible = true;
	}
}

void Vga::HideCursor()
{
	if (cursor_visible)
	{
		ScreenAttributes[cursor_y * settings->TextColumns + cursor_x] -= 16 * 4;
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
	Vga::PrintChar(x, y, ch, 0);
}
void Vga::PrintChar(uint16_t x, uint16_t y, uint8_t ch, uint32_t attribute)
{
	if (x >= settings->TextColumns || y >= settings->TextRows)
	{
		// Invalid
		return;
	}

	int offset = y * settings->TextColumns + x;
	ScreenCharacters[offset] = ch << 1;
	if (attribute != 0)
	{
		ScreenAttributes[offset] = attribute;
		if (cursor_visible && x == cursor_x && y == cursor_y)
		{
			ScreenAttributes[offset] += 16 * 4;
		}
	}
}

void Vga::SetAttribute(uint16_t x, uint16_t y, uint32_t attribute)
{
	if (x >= settings->TextColumns || y >= settings->TextRows)
	{
		// Invalid
		return;
	}

	int offset = y * settings->TextColumns + x;
	ScreenAttributes[offset] = attribute;
	if (cursor_visible && x == cursor_x && y == cursor_y)
	{
		ScreenAttributes[offset] += 16 * 4;
	}
}

void Vga::Print(const char* str)
{
	Print((char*)str);
}
void Vga::Print(const char* str, uint32_t attribute)
{
	Print((char*)str, attribute);
}

void Vga::Print(char* str)
{
	Print(str, 0);
}
void Vga::Print(char* str, uint32_t attribute)
{
	char buf[HSIZE_CHARS];
	utf8ToCP866(str, buf);
	char* convertedStr = buf;

	bool cursorShown = cursor_visible;
	if (cursorShown)
	{
		HideCursor();
	}

    while (*convertedStr)
    {
    	PrintChar(cursor_x, cursor_y, *convertedStr++, attribute);
    	cursorNext(attribute);
    }

	if (cursorShown)
	{
		ShowCursor();
	}
}

void Vga::PrintUtf8(const char* str)
{
	char buf[HSIZE_CHARS];
	utf8ToCP866((char*)str, buf);
	Print(buf);
}

void Vga::cursorNext(uint32_t attribute)
{
    uint8_t x = cursor_x;
    uint8_t y = cursor_y;
    if (x < settings->TextColumns - 1)
    {
        x++;
    }
    else
    {
        if (y < settings->TextRows - 1)
        {
            x = 0;
            y++;
        }
    }

    if (cursor_visible && attribute != 0)
    {
    	SetAttribute(x, y, attribute);
    }
    SetCursorPosition(x, y);
}

void Vga::utf8ToCP866(char* utf8, char* result)
{
	// Converts some UTF8 characters to code page 866
	int strLen = strlen(utf8);
	uint16_t twoByteSequence = 0;
	for (int i = 0; i < strLen; i++)
	{
		char ch = utf8[i];
		if (twoByteSequence > 0)
		{
			twoByteSequence = (twoByteSequence << 8) + ch;

			if (twoByteSequence >= 0xD090 && twoByteSequence <= 0xD0BF)
			{
				// "À".."ï"
				*result = (char)(twoByteSequence - 0xD010);
			}
			else if (twoByteSequence >= 0xD180 && twoByteSequence <= 0xD18F)
			{
				// "ð".."ÿ"
				*result = (char)(twoByteSequence - 0xD0A0);
			}

			twoByteSequence = 0;
		}
		else
		{
			if ((ch & 0B11100000) == 0B11000000)
			{
				twoByteSequence = ch;
				continue;
			}

			*result = utf8[i];
		}

		result++;
	}

	*result = '\0';
}
