#pragma once

#include <SDL2/SDL.h>
#include "utils/punto.h"

enum Color
{
    YELLOW,
    RED,
    WHITE,
    CYAN,
    LIME,
    MAGENTA,
    BLUE
};

class TextRenderer
{
public:
    static TextRenderer *getInstance(SDL_Renderer *);
    void renderText(punto_t punto, const char *text, float resize, Color color = YELLOW);
    bool includesCharacter(char) const;
    ~TextRenderer();

private:
    TextRenderer(SDL_Renderer *);
    static TextRenderer *instance;
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
};