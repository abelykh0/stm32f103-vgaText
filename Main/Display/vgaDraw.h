#ifndef VGA_DRAW_H
#define VGA_DRAW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void vgaDraw(uint32_t* characters, uint32_t vline, uint32_t* drawLookup, uint8_t* dest);

#ifdef __cplusplus
}
#endif

#endif  // DRAW4_H
