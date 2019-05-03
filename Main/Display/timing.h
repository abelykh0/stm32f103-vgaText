#ifndef VGA_TIMING_H
#define VGA_TIMING_H

#include "stdint.h"

namespace Vga
{

/*
 * Describes the horizontal and vertical timing for a display mode.
 */
struct Timing
{
    enum struct Polarity
    {
        positive = 0,
        negative = 1,
    };

    double pixel_frequency_mhz; // Pixel frequency in MHz

    /*
    * Horizontal timing, expressed in pixels.
    */
    uint16_t horizPixels;
    Polarity horizSyncPolarity;
    uint16_t horizWholeLine;
    uint16_t horizSyncPulse;
    uint16_t horizStartDraw;
    int16_t  horizLead; // manual adjustment as needed

    /*
    * Vertical timing, expressed in lines.
    */
    uint16_t verticalPixels;
    Polarity verticalSyncPolarity;
    uint16_t verticalWholeFrame;
    uint16_t verticalSyncPulse;
    uint16_t verticalStartLine;
};

/*
 * Canned timings for common modes.
 */
extern Timing const timing_640x480_60hz;
extern Timing const timing_640x480_60_01hz;
extern Timing const timing_800x600_56hz;

} // namespace Vga

#endif // VGA_TIMING_H
