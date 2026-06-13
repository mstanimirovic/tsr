/*
 * example: unsafe.c
 *
 * Demonstrates: tsr_draw_pixel_unsafe for raw speed.
 *               Draws a symmetrical pattern using integer math.
 *
 */

#define TSR_IMPLEMENTATION
#include "tsr.h"
#include "tsr_sdl3.h"
#include <stdlib.h>
#include <math.h>

#define W 800
#define H 600

/* simple pseudo-random for starfield */
static unsigned int g_seed = 12345;
static int fast_rand(void) {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

int main(void) {
    tsr_sdl3_ctx sdl;
    if (!tsr_sdl3_init(&sdl, "tsr - unsafe pixel", W, H)) {
        SDL_Log("init failed: %s", SDL_GetError());
        return 1;
    }

    tsr_ctx ctx = tsr_sdl3_make_ctx(&sdl);

    /* --- fill with dark blue (all pixels, unsafe) --- */
    tsr_set_color(&ctx, 0x0A0A1AFF);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            tsr_draw_pixel_unsafe(&ctx, x, y);  // no bounds check
        }
    }

    /* --- draw a grid of thin lines using unsafe pixels --- */
    tsr_set_color(&ctx, 0x1A1A3AFF);
    for (int i = 0; i < W; i += 20) {
        for (int y = 0; y < H; y++) {
            tsr_draw_pixel_unsafe(&ctx, i, y);
        }
    }
    for (int j = 0; j < H; j += 20) {
        for (int x = 0; x < W; x++) {
            tsr_draw_pixel_unsafe(&ctx, x, j);
        }
    }

    /* --- starfield with random bright pixels (unsafe) --- */
    for (int i = 0; i < 2000; i++) {
        int x = fast_rand() % W;
        int y = fast_rand() % H;
        tsr_set_color(&ctx, 0xFFFFFFFF);  /* white */
        tsr_draw_pixel_unsafe(&ctx, x, y);
    }

    /* --- mandala-like pattern (using set_color before each pixel) --- */
    int cx = W / 2, cy = H / 2;
    for (int angle = 0; angle < 360; angle += 2) {
        double rad = angle * 3.1415926535 / 180.0;
        for (int r = 10; r < 250; r += 10) {
            int x = cx + (int)(r * cos(rad));
            int y = cy + (int)(r * sin(rad));
            /* color based on radius */
            uint8_t red   = (r * 2) & 0xFF;
            uint8_t green = (angle * 2) & 0xFF;
            uint8_t blue  = 128;
            tsr_pixel color = (red << 24) | (green << 16) | (blue << 8) | 0xFF;
            tsr_set_color(&ctx, color);
            tsr_draw_pixel_unsafe(&ctx, x, y);
        }
    }

    /* --- a small cross in the center using safe pixel for contrast --- */
    tsr_set_color(&ctx, 0xFFFF00FF);
    for (int i = -20; i <= 20; i++) {
        tsr_draw_pixel(&ctx, cx + i, cy);
        tsr_draw_pixel(&ctx, cx, cy + i);
    }

    tsr_sdl3_present(&sdl, &ctx);

    while (tsr_sdl3_poll())
        SDL_Delay(16);

    tsr_sdl3_destroy(&sdl);
    return 0;
}
