#include <string.h>
#include <stdint.h>

#include "startup.h"
#include "Display/vgaCore.h"
#include "Display/timing.h"
#include "Display/font8x16.h"
#include "Display/vgaText.h"
#include "Keyboard/ps2Keyboard.h"

static uint16_t _screenCharacters[HSIZE_CHARS * VSIZE_CHARS + 1];
static uint32_t _screenAttributes[HSIZE_CHARS * VSIZE_CHARS + 1];
static uint8_t _allColorsAttribute[256];

using namespace Vga;

static VideoSettings Video {
	&TIMING,
	HSIZE_CHARS, VSIZE_CHARS,
	_screenCharacters, _screenAttributes
};
extern "C" void initialize()
{
}

extern "C" void setup()
{
	InitVga(&Video);
	InitVgaText(font8x16);

    // Display frame
    PrintChar(0, 0, '\xC9'); // ╔
    PrintChar(HSIZE_CHARS - 1, 0, '\xBB'); // ╗
    PrintChar(0, VSIZE_CHARS - 1, '\xC8'); // ╚
    PrintChar(HSIZE_CHARS - 1, VSIZE_CHARS - 1, '\xBC'); // ╝
    for (int i = 1; i < HSIZE_CHARS - 1; i++)
    {
    	PrintChar(i, 0, '\x0CD'); // ═
    	PrintChar(i, VSIZE_CHARS - 1, '\x0CD'); // ═
    }
    for (int i = 1; i < VSIZE_CHARS - 1; i++)
    {
    	PrintChar(0, i, '\x0BA'); // ║
    	PrintChar(HSIZE_CHARS - 1, i, '\x0BA'); // ║
    }

    // This is used to display 64 colors using a space character
    for (uint8_t color = 0; color < 64; color++)
    {
    	for (uint8_t j = 0; j < 4; j++)
    	{
            _allColorsAttribute[(color << 2) + j] = color;
    	}
    }

    char buf[20];
    for (int i = 0; i < 64; i++)
    {
    	sprintf(buf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(i));

    	SetCursorPosition(4 + (i % 4) * 7, 4 + (i / 4) * 2);
    	Print("      ", (uint32_t)&_allColorsAttribute[i * 4]);

    	SetCursorPosition(4 + (i % 4) * 7, 3 + (i / 4) * 2);
    	Print(buf);
    }

	// Initialize PS2 Keyboard
	Ps2_Initialize();

	SetCursorPosition(1, 1);
	ShowCursor();
}

extern "C" void loop()
{
	int32_t scanCode = Ps2_GetScancode();
	if (scanCode > 0)
	{
		//char buf[30];
		//snprintf(buf, 30, "%04lx", (uint32_t)scanCode);
		//Print(buf);

		if ((scanCode & 0xFF00) != 0xF000)
		{
			// key down
			switch (scanCode)
			{
			case KEY_LEFTARROW:
				if (Vga::cursor_x > 0)
				{
					Vga::SetAttribute(Vga::cursor_x - 1, Vga::cursor_y, (int32_t)Vga::StandardAttribute);
					Vga::SetCursorPosition(Vga::cursor_x - 1, Vga::cursor_y);
				}
				break;
			case KEY_BACKSPACE:
				if (Vga::cursor_x > 0)
				{
					Vga::SetCursorPosition(Vga::cursor_x - 1, Vga::cursor_y);
					Vga::PrintChar(Vga::cursor_x, Vga::cursor_y, ' ', (int32_t)Vga::StandardAttribute);
				}
				break;
			case KEY_RIGHTARROW:
				if (Vga::cursor_x < HSIZE_CHARS - 1)
				{
					Vga::SetAttribute(Vga::cursor_x + 1, Vga::cursor_y, (int32_t)Vga::StandardAttribute);
					Vga::SetCursorPosition(Vga::cursor_x + 1, Vga::cursor_y);
				}
				break;
			case KEY_UPARROW:
				if (Vga::cursor_y > 0)
				{
					Vga::SetAttribute(Vga::cursor_x, Vga::cursor_y - 1, (int32_t)Vga::StandardAttribute);
					Vga::SetCursorPosition(Vga::cursor_x, Vga::cursor_y - 1);
				}
				break;
			case KEY_DOWNARROW:
				if (Vga::cursor_y < VSIZE_CHARS - 1)
				{
					Vga::SetAttribute(Vga::cursor_x, Vga::cursor_y + 1, (int32_t)Vga::StandardAttribute);
					Vga::SetCursorPosition(Vga::cursor_x, Vga::cursor_y + 1);
				}
				break;
			default:
				char buf[2];
				buf[0] = Ps2_ConvertScancode(scanCode);
				if (buf[0] != '\0')
				{
					buf[1] = '\0';
					Vga::Print(buf, (int32_t)Vga::StandardAttribute);
				}
			}
		}
	}

	//HAL_Delay(10);
}
