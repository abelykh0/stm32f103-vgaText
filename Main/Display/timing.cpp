#include "timing.h"

namespace Vga
{

Timing const timing_640x480_85hz =
{
	.pixel_frequency_mhz = 36.00,

	.line_pixels = 832,
	.sync_pixels = 56,
	.back_porch_pixels = 80,
	.video_lead = 20,
	.video_pixels = 640,
	.hsync_polarity = Timing::Polarity::negative,

	.vsync_start_line = 1,
	.vsync_end_line = 1 + 3,
	.video_start_line = 1 + 3 + 25,
	.video_end_line = 1 + 3 + 25 + 480,
	.vsync_polarity = Timing::Polarity::negative,
};

Timing const timing_640x480_60hz =
{
	.pixel_frequency_mhz = 25.17,

	.line_pixels = 800,
	.sync_pixels = 96,
	.back_porch_pixels = 48,
	.video_lead = 10,
	.video_pixels = 640,
	.hsync_polarity = Timing::Polarity::positive,

	.vsync_start_line = 10,
	.vsync_end_line = 10 + 2,
	.video_start_line = 10 + 2 + 33,
	.video_end_line = 10 + 2 + 33 + 480,
	.vsync_polarity = Timing::Polarity::positive,
};

Timing const timing_720x576_48hz =
{
	.pixel_frequency_mhz = 28.8,

	.line_pixels = 968,
	.sync_pixels = 104,
	.back_porch_pixels = 32,
	.video_lead = 100,
	.video_pixels = 720,
	.hsync_polarity = Timing::Polarity::negative,

	.vsync_start_line = 10,
	.vsync_end_line = 10 + 5,
	.video_start_line = 10 + 5 + 11,
	.video_end_line = 10 + 5 + 11 + 576,
	.vsync_polarity = Timing::Polarity::negative,
};

Timing const timing_800x600_56hz =
{
	.pixel_frequency_mhz = 36.0,

	.line_pixels = 1024,
	.sync_pixels = 72,
	.back_porch_pixels = 128,
	.video_lead = 100,
	.video_pixels = 800,
	.hsync_polarity = Timing::Polarity::negative,

	.vsync_start_line = 1,
	.vsync_end_line = 1 + 2,
	.video_start_line = 1 + 2 + 22,
	.video_end_line = 1 + 5 + 22 + 600,
	.vsync_polarity = Timing::Polarity::negative,
};

Timing const timing_640x480_60_01hz =
{
	.pixel_frequency_mhz = 24.00,

	.line_pixels = 792,
	.sync_pixels = 88,
	.back_porch_pixels = 32,
	.video_lead = 25,
	.video_pixels = 640,
	.hsync_polarity = Timing::Polarity::negative,

	.vsync_start_line = 10,
	.vsync_end_line = 10 + 5,
	.video_start_line = 10 + 5 + 10,
	.video_end_line = 10 + 5 + 10 + 480,
	.vsync_polarity = Timing::Polarity::negative,
};

} // namespace Vga
