#include <string.h>
#include <stdint.h>

#include "startup.h"
#include "Display/vgaCore.h"
#include "Display/timing.h"
#include "Display/font8x8.h"
#include "Display/vgaText.h"
#include "Keyboard/ps2Keyboard.h"

static uint8_t _screenData[HSIZE_CHARS * VSIZE_CHARS];

using namespace Vga;

static VideoSettings Video {
	&TIMING,
	1,
	HSIZE_CHARS, VSIZE_CHARS,
	_screenData
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

	ClearScreen();
	Print("A23456789_123456789_123456789_123456789A");

	for (int32_t y = 0; y < VSIZE_CHARS; y++)
	{
		char buf[30];
		snprintf(buf, 30, "%02d", (uint16_t)y + 1);
		SetCursorPosition(0, y);
		Print(buf);
	}

	SetCursorPosition(0, VSIZE_CHARS - 1);
	Print("D23456789_123456789_123456789_123456789D");
}

extern "C" void loop()
{
	int32_t scanCode = Ps2_GetScancode();
	if (scanCode > 0)
	{
		char buf[30];
		snprintf(buf, 30, "%04lx", (uint32_t)scanCode);
		if ((scanCode & 0xFF00) == 0xF000)
		{
			// key up
			SetCursorPosition(10, 10);
		}
		else
		{
			// key down
			SetCursorPosition(10, 11);
		}
		Print(buf);
	}

	//HAL_Delay(10);
}
