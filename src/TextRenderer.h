#pragma once

#include <SDL2/SDL.h>
#include "utils/punto.h"

typedef enum color
{
    YELLOW,
    RED,
    WHITE,
    CYAN,
    LIME,
    MAGENTA,
    BLUE
} color_t;

class TextRenderer
{
public:
    static TextRenderer *getInstance(SDL_Renderer *);
    void renderText(punto_t punto, const char *text, float resize, color_t color = YELLOW);
    bool includesCharacter(char);
    ~TextRenderer();

private:
    TextRenderer(SDL_Renderer *);
    static TextRenderer *instance;
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
};