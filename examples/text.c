/*
 * example: text.c
 *
 * Demonstrates: tsr_draw_char, tsr_draw_char_bg,
 *               tsr_draw_text, tsr_draw_text_bg, tsr_draw_line
 *
 */

#define TSR_IMPLEMENTATION
#include "tsr.h"
#include "tsr_sdl3.h"

#define W 800
#define H 600

int main(void) {
    tsr_sdl3_ctx sdl;
    if (!tsr_sdl3_init(&sdl, "tsr - text", W, H)) {
        SDL_Log("init failed: %s", SDL_GetError());
        return 1;
    }

    tsr_ctx ctx = tsr_sdl3_make_ctx(&sdl);

    /* clear to dark blue */
    tsr_set_color(&ctx, 0x0A0A1AFF);
    tsr_clear(&ctx);

    /* --- single characters without background (transparent) --- */
    tsr_set_color(&ctx, 0xE94560FF);   /* red */
    tsr_draw_char(&ctx, 20, 20, 'H', 2);
    tsr_draw_char(&ctx, 40, 20, 'E', 2);
    tsr_draw_char(&ctx, 60, 20, 'L', 2);
    tsr_draw_char(&ctx, 80, 20, 'L', 2);
    tsr_draw_char(&ctx, 100, 20, 'O', 2);

    /* --- same with background --- */
    tsr_set_color(&ctx, 0x00FFFFFF);               /* white foreground */
    tsr_pixel bg = 0x533483FF;                      /* purple background */
    tsr_draw_char_bg(&ctx, 20, 60, 'H', bg, 2);
    tsr_draw_char_bg(&ctx, 40, 60, 'E', bg, 2);
    tsr_draw_char_bg(&ctx, 60, 60, 'L', bg, 2);
    tsr_draw_char_bg(&ctx, 80, 60, 'L', bg, 2);
    tsr_draw_char_bg(&ctx, 100, 60, 'O', bg, 2);

    /* --- complete string, no background --- */
    tsr_set_color(&ctx, 0x0F3460FF);
    tsr_draw_text(&ctx, 20, 120, "Tiny Software Renderer", 2);

    /* --- string with background, different scale --- */
    tsr_set_color(&ctx, 0xE94560FF);
    tsr_draw_text_bg(&ctx, 20, 170, "SCADA HMI ready", 0x16213EFF, 1);

    /* --- scaled text (4x) --- */
    tsr_set_color(&ctx, 0x533483FF);
    tsr_draw_text(&ctx, 20, 230, "SCALE 4X", 4);

    /* --- mix of character backgrounds in a single line --- */
    const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    tsr_set_color(&ctx, 0x00FF00FF);   /* green */
    for (int i = 0; alphabet[i]; i++) {
        /* alternate background every character */
        tsr_pixel bg_even = (i % 2 == 0) ? 0x333333FF : 0x555555FF;
        tsr_draw_char_bg(&ctx, 20 + i * 12, 300, alphabet[i], bg_even, 1);
    }

    /* --- draw a box around the first text group --- */
    tsr_set_color(&ctx, 0x888888FF);
    tsr_draw_rect(&ctx, 15, 15, 105, 50);

    /* --- separator line --- */
    tsr_draw_line(&ctx, 0, 400, W, 400);

    /* --- multiline text (manual) --- */
    tsr_set_color(&ctx, 0xCCCCCCFF);
    tsr_draw_text(&ctx, 20, 420, "Version 0.1", 1);
    tsr_draw_text(&ctx, 20, 440, "Single header library", 1);
    tsr_draw_text(&ctx, 20, 460, "No standard library required", 1);

    tsr_sdl3_present(&sdl, &ctx);

    while (tsr_sdl3_poll())
        SDL_Delay(16);

    tsr_sdl3_destroy(&sdl);
    return 0;
}
