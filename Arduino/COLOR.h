#pragma once

#include "stdint.h"

// unsigned char reverse(unsigned char b) {
//    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
//    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
//    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
//    return b;
// }

// static const uint16_t EGA565[] = {
//   0x0000,
//   0x0014,
//   0x0540,
//   0x0554,
//   0xA000,
//   0xA014,
//   0xA2A0,
//   0xA554,
//   0x52AA,
//   0x52BF,
//   0x57EA,
//   0x57FF,
//   0xFAAA,
//   0xFABF,
//   0xFFEA,
//   0xFFFF,
// };

static const uint16_t EGAILI[] = {
  0x0000,
  0x0014,
  0x0540,
  0x0554,
  0xA000,
  0xA014,
  0xA2A0,
  0x8210,    // 1000001000010000 = 0x8210   // (BLUE: 00000-000000-11111 = 0x001F),// (GREEN: 00000-111111-00000 = 0x07E0), // 0xA554,
  0x5550, // 0x52AA,
  0x52BF,
  0x57EA,
  0x57FF,
  0xFAAA,
  0xFABF,
  0xFFEA,
  0xFFFF,
};

// static const uint16_t EGA565[] = {
//     0x0000, 0x0014, 0x0540, 0x0555,
//     0xA800, 0xA815, 0xAAA0, 0xAD55,
//     0x52AA, 0x52BF, 0x57EA, 0x57FF,
//     0xFAAA, 0xFABF, 0xFFEA, 0xFFFF
// };

void egargb(uint8_t ega, uint8_t& red, uint8_t& green, uint8_t& blue) {
  red   = 85 * (((ega >> 1) & 2) | (ega >> 5) & 1);
  green = 85 * (( ega       & 2) | (ega >> 4) & 1);
  blue  = 85 * (((ega << 1) & 2) | (ega >> 3) & 1);
}

// uint16_t RGB565(uint8_t red, uint8_t green, uint8_t blue) {
//   return ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3);
// }

// static const uint32_t EGARGB[] = {
//     0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
//     0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
//     0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
//     0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
// };

// uint16_t fixed_color565(int index) {
//   uint8_t r, g, b;
//   egargb(index, r, g, b);
//   return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
//   // return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
//   // return ((r & 0xF8) << 8) | ((g & 0xF8) << 2) | (b >> 3);
// }

#define Color uint16_t // RGB565

// #define COLOR(index) (fixed_color565(index))
#define COLOR(index) (EGAILI[index])
// Color COLOR(int index) {
//   int red, green, blue;
//   EGARGB(index, red, green, blue);
//   return RGB565(red, green, blue);
// }
