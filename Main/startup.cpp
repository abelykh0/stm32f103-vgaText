#include <string.h>
#include <stdint.h>

#include "startup.h"
#include "Display/vgaCore.h"
#include "Display/timing.h"
#include "Display/font8x8.h"
#include "Display/vgaText.h"
#include "Keyboard/ps2Keyboard.h"

static uint8_t _screenCharacters[HSIZE_CHARS * VSIZE_CHARS + 1];
static uint32_t _screenAttributes[HSIZE_CHARS * VSIZE_CHARS + 1];

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
	InitVgaText(font8x8);

	// Initialize PS2 Keyboard
	Ps2_Initialize();

	SetCursorPosition(10, 0);
	Print("русские буквы");

	for (int32_t y = 0; y < VSIZE_CHARS; y++)
	{
		char buf[30];
		snprintf(buf, 30, "%02d", (uint16_t)y + 1);
		SetCursorPosition(0, y);
		Print(buf);
	}

	SetCursorPosition(0, VSIZE_CHARS - 1);
	Print("D23456789_123456789_123456789_123456789D");

	SetCursorPosition(10, 10);
	Print("X");
	SetCursorPosition(10, 10);
	SetCursorPosition(11, 11);
	Print("W");
	SetCursorPosition(11, 11);
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
					Vga::SetCursorPosition(Vga::cursor_x - 1, Vga::cursor_y);
				}
				break;
			case KEY_BACKSPACE:
				if (Vga::cursor_x > 0)
				{
					Vga::SetCursorPosition(Vga::cursor_x - 1, Vga::cursor_y);
					Vga::PrintChar(Vga::cursor_x, Vga::cursor_y, ' ');
				}
				break;
			case KEY_RIGHTARROW:
				if (Vga::cursor_x < HSIZE_CHARS - 1)
				{
					Vga::SetCursorPosition(Vga::cursor_x + 1, Vga::cursor_y);
				}
				break;
			case KEY_UPARROW:
				if (Vga::cursor_y > 0)
				{
					Vga::SetCursorPosition(Vga::cursor_x, Vga::cursor_y - 1);
				}
				break;
			case KEY_DOWNARROW:
				if (Vga::cursor_y < VSIZE_CHARS - 1)
				{
					Vga::SetCursorPosition(Vga::cursor_x, Vga::cursor_y + 1);
				}
				break;
			default:
				char buf[2];
				buf[0] = Ps2_ConvertScancode(scanCode);
				if (buf[0] != '\0')
				{
					buf[1] = '\0';
					Vga::Print(buf);
				}
			}
		}
	}

	//HAL_Delay(10);
}
