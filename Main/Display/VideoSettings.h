#ifndef _VIDEOSETTINGS_H
#define _VIDEOSETTINGS_H

#include <stdint.h>
#include "timing.h"

namespace Vga
{

class VideoSettings
{
  public:
	const Vga::Timing* Timing;

	// text resolution
	uint8_t TextColumns;
	uint8_t TextRows;

	// video memory
	uint16_t* ScreenCharacters;
	uint32_t* ScreenAttributes;
};

}

#endif
