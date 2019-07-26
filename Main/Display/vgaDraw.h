#ifndef VGA_DRAW_H
#define VGA_DRAW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void vgaDraw(uint8_t* font, uint16_t* characters, uint32_t* attributes, uint8_t* dest);

#ifdef __cplusplus
}
#endif

#endif  // DRAW4_H
