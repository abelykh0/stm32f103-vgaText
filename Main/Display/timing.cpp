#include "timing.h"

namespace Vga
{

Timing const timing_640x480_60hz =
{
	.pixel_frequency_mhz = 25.175,

	.horizPixels = 640,
    .horizSyncPolarity = Timing::Polarity::positive,
    .horizWholeLine = 800,
    .horizSyncPulse = 96,
    .horizStartDraw = 96 - 16,
	.horizLead = 0,

	.verticalPixels = 480,
    .verticalSyncPolarity = Timing::Polarity::positive,
    .verticalWholeFrame = 525,
    .verticalSyncPulse = 2,
    .verticalStartLine = 35
};

Timing const timing_640x480_60_01hz =
{
	.pixel_frequency_mhz = 24.00,

	.horizPixels = 640,
    .horizSyncPolarity = Timing::Polarity::negative,
    .horizWholeLine = 792,
    .horizSyncPulse = 88,
    .horizStartDraw = 140,
	.horizLead = 0,

	.verticalPixels = 480,
    .verticalSyncPolarity = Timing::Polarity::negative,
    .verticalWholeFrame = 505,
    .verticalSyncPulse = 5,
    .verticalStartLine = 38
};

Timing const timing_800x600_56hz =
{
	.pixel_frequency_mhz = 36.0,

	.horizPixels = 800,
    .horizSyncPolarity = Timing::Polarity::positive,
    .horizWholeLine = 1024,
    .horizSyncPulse = 72,
    .horizStartDraw = 72 - 24,
	.horizLead = -90,

	.verticalPixels = 600,
    .verticalSyncPolarity = Timing::Polarity::positive,
    .verticalWholeFrame = 625,
    .verticalSyncPulse = 2,
    .verticalStartLine = 25
};

} // namespace Vga
