/*
 * example: primitives.c
 *
 * Tests: tsr_clear, tsr_draw_pixel, tsr_draw_line,
 *        tsr_draw_rect, tsr_fill_rect
 *
 */

#define TSR_IMPLEMENTATION
#include "tsr.h"
#include "tsr_sdl3.h"

#define W 800
#define H 600

int main(void) {
    tsr_sdl3_ctx sdl;
    if (!tsr_sdl3_init(&sdl, "tsr - primitives", W, H)) {
        SDL_Log("init failed: %s", SDL_GetError());
        return 1;
    }

    tsr_ctx ctx = tsr_sdl3_make_ctx(&sdl);

    /* --- clear to dark background --- */
    tsr_set_color(&ctx, 0x1A1A2EFF);
    tsr_clear(&ctx);

    /* --- scattered pixels --- */
    tsr_set_color(&ctx, 0xE94560FF);
    for (int i = 0; i < 300; i++) {
        int x = (i * 97 + 13) % W;
        int y = (i * 53 + 7) % H;
        tsr_draw_pixel(&ctx, x, y);
    }

    /* --- lines: star burst from center --- */
    tsr_set_color(&ctx, 0x0F3460FF);
    int cx = W / 2;
    int cy = H / 2;
    for (int i = 0; i < 24; i++) {
        /*
         * cheap fixed-point angle approximation:
         * pre-computed endpoints for 24 evenly spaced directions
         * avoids sin/cos entirely
         */
        static const int dx[24] = {100, 97,  87,  71,  50,   26,  0,   -26,
                                   -50, -71, -87, -97, -100, -97, -87, -71,
                                   -50, -26, 0,   26,  50,   71,  87,  97};
        static const int dy[24] = {0,   26,  50,   71,  87,  97,  100, 97,
                                   87,  71,  50,   26,  0,   -26, -50, -71,
                                   -87, -97, -100, -97, -87, -71, -50, -26};
        tsr_draw_line(&ctx, cx, cy, cx + dx[i] * 2, cy + dy[i] * 2);
    }

    /* --- outlined rects --- */
    tsr_set_color(&ctx, 0x16213EFF);
    tsr_draw_rect(&ctx, 20, 20, 120, 80);
    tsr_draw_rect(&ctx, 20, 120, 120, 80);
    tsr_draw_rect(&ctx, 20, 220, 120, 80);

    /* --- filled rects, color ramp --- */
    tsr_set_color(&ctx, 0xE94560FF);
    tsr_fill_rect(&ctx, 660, 20, 120, 80);
    tsr_set_color(&ctx, 0x0F3460FF);
    tsr_fill_rect(&ctx, 660, 120, 120, 80);
    tsr_set_color(&ctx, 0x533483FF);
    tsr_fill_rect(&ctx, 660, 220, 120, 80);

    /* --- diagonal lines crossing the canvas --- */
    tsr_set_color(&ctx, 0x533483FF);
    tsr_draw_line(&ctx, 0, 0, W, H);
    tsr_draw_line(&ctx, W, 0, 0, H);
    tsr_draw_line(&ctx, W / 2, 0, 0, H / 2);
    tsr_draw_line(&ctx, W / 2, 0, W, H / 2);

    /* --- filled rect over center to show layering --- */
    tsr_set_color(&ctx, 0xE9456088);
    tsr_fill_rect(&ctx, cx - 60, cy - 40, 120, 80);

    tsr_sdl3_present(&sdl, &ctx);

    while (tsr_sdl3_poll())
        SDL_Delay(16);

    tsr_sdl3_destroy(&sdl);
    return 0;
}
