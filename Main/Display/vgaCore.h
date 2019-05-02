#ifndef _VGACORE_H_
#define _VGACORE_H_

#include <stdint.h>
#include "videosettings.h"

/*
	Pinout
	======

	PB6 - VSync
	PB0 - HSync

	PA0, PA1 - Red
	PA2, PA3 - Green
	PA4, PA5 - Blue

	Resistors
	=========
	680 Ohm on PA0, PA2, PA4 (0.33 V)
	470 Ohm on PA1, PA2, PA3
	====
	Parallel: 278 Ohm (0.7 V)

*/

namespace Vga
{
	extern const Vga::VideoSettings* settings;
	extern uint8_t* ScreenCharacters;
	extern uint32_t* ScreenAttributes;
	extern uint16_t horizontalPixelCount;

	void InitVga(VideoSettings* videoSettings);
    void ClearScreen();

	extern void HBlankInterrupt();
}

#endif
