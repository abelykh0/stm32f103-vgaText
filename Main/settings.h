#ifndef __VIDEOSETTINGS_H__
#define __VIDEOSETTINGS_H__

#define BOARD2

#ifdef BOARD2
#define GET_COLOR(x) ((x & 0x03) | ((x << 2) & 0xF0))
#else
#define GET_COLOR(x) (x)
#endif


// Colors
#define BACK_COLOR GET_COLOR(0x10)
#define FORE_COLOR GET_COLOR(0x3F)
//#define BACK_COLOR 0x00
//#define FORE_COLOR 0x0C

// Resolution
#define TIMING timing_800x600_56hz
#define	HSIZE_CHARS  38
#define	VSIZE_CHARS  37

#endif /* __VIDEOSETTINGS_H__ */
