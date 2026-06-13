#ifndef TSR_SDL3_H
#define TSR_SDL3_H

/*
 * tsr_sdl3.h - thin SDL3 window + texture wrapper for tsr examples
 *
 * Usage:
 *   tsr_sdl3_ctx sdl;
 *   tsr_sdl3_init(&sdl, "title", 800, 600);
 *   tsr_ctx ctx = tsr_sdl3_make_ctx(&sdl);
 *   -- draw with tsr --
 *   tsr_sdl3_present(&sdl, &ctx);
 *   -- poll events --
 *   tsr_sdl3_destroy(&sdl);
 */

#include <SDL3/SDL.h>

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *texture;
    int           w, h;
    uint32_t     *pixels;
} tsr_sdl3_ctx;

static int tsr_sdl3_init(tsr_sdl3_ctx *s, const char *title, int w, int h)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return 0;

    s->w = w;
    s->h = h;

    s->window = SDL_CreateWindow(title, w, h, 0);
    if (!s->window)
        return 0;

    s->renderer = SDL_CreateRenderer(s->window, NULL);
    if (!s->renderer)
        return 0;

    s->texture = SDL_CreateTexture(
        s->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );
    if (!s->texture)
        return 0;

    s->pixels = (uint32_t *)SDL_malloc(w * h * sizeof(uint32_t));
    if (!s->pixels)
        return 0;

    return 1;
}

static tsr_ctx tsr_sdl3_make_ctx(tsr_sdl3_ctx *s)
{
    tsr_ctx ctx;
    ctx.pixels     = (uint8_t *)s->pixels;
    ctx.w          = s->w;
    ctx.h          = s->h;
    ctx.stride     = s->w * sizeof(uint32_t);
    ctx.draw_color = 0;
    return ctx;
}

static void tsr_sdl3_present(tsr_sdl3_ctx *s, tsr_ctx *ctx)
{
    SDL_UpdateTexture(s->texture, NULL, ctx->pixels, ctx->stride);
    SDL_RenderClear(s->renderer);
    SDL_RenderTexture(s->renderer, s->texture, NULL, NULL);
    SDL_RenderPresent(s->renderer);
}

static void tsr_sdl3_destroy(tsr_sdl3_ctx *s)
{
    SDL_free(s->pixels);
    SDL_DestroyTexture(s->texture);
    SDL_DestroyRenderer(s->renderer);
    SDL_DestroyWindow(s->window);
    SDL_Quit();
}

/*
 * returns 1 if the app should keep running, 0 if quit was requested
 */
static int tsr_sdl3_poll(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT)
            return 0;
        if (e.type == SDL_EVENT_KEY_DOWN &&
            e.key.key == SDLK_ESCAPE)
            return 0;
    }
    return 1;
}

#endif /* TSR_SDL3_H */
