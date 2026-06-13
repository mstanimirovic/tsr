# TSR - Tiny Software Renderer

A minimal, zero-dependency 2D software renderer written in C99 for freestanding environments.  
No standard library required. Single-header. Just a framebuffer and a compiler.

## What it does

TSR draws into a user-provided memory buffer (the framebuffer) using only integer arithmetic. It is intended for embedded HMIs, operating system kernels, tiny microcontrollers, or anywhere a full graphics stack is impractical.

## Features (v0.1.0)

- **Immediate mode** - draw pixels, lines, rectangles, text directly
- **Compile-time pixel format** - RGBA8888 (default) or RGB565
- **Bresenham line drawing** - no floating point
- **Built‑in 8×8 monospace font** - ASCII 0x20‑0x7E
- **Text with optional background fill** and per‑character scaling
- **Safe and unsafe pixel writes** - skip bounds checks where safe

## Quick start

```c
#define TSR_IMPLEMENTATION
#include "tsr.h"

// Framebuffer: 320x240 pixels, 16-bit colour
static uint16_t fb[320 * 240];
tsr_ctx ctx;

int main(void) {
    tsr_ctx_init(&ctx, (uint8_t *)fb, 320, 240, 320 * 2);
    tsr_set_color(&ctx, 0xF800);           // red (RGB565)
    tsr_fill_rect(&ctx, 0, 0, 320, 240);  // fill whole screen

    tsr_set_color(&ctx, 0xFFFF);           // white
    tsr_draw_text(&ctx, 20, 100, "Hello, TSR!", 2);
    // … copy framebuffer to display …
}
```

## Building

No dependencies beyond a C99 compiler. The library is header‑only; to compile, include it in one `.c` file with `TSR_IMPLEMENTATION` defined.

Example (with SDL3 for desktop testing):

```bash
gcc example_primitives.c -o example_primitives $(pkg-config --cflags --libs sdl3) -I.
```

## Examples

Three example programs are provided:

- `examples/primitives.c` - lines, rects, scattered pixels
- `examples/text.c` - all text rendering functions at different scales
- `examples/unsafe.c` - direct pixel writes for maximum speed

All require SDL3 only for windowing and are intended for desktop testing.

## Roadmap

- **v0.2** - retained mode scene management, dirty flags
- **v0.3** - bitmap blitting, clipping
- **v0.4** - fixed‑point coordinates, anti‑aliasing
- **v1.0** - complete HMI renderer

## License

Free for personal and commercial use without restriction. See the header file for full details.
