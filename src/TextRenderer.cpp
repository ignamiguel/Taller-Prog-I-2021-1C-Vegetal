#include <string>
#include <SDL2/SDL_image.h>
#include "TextRenderer.h"
#include "utils/Constants.hpp"
#include "utils/window.hpp"

#define LETTER_X 16
#define LETTER_Y 8
#define LETTER_WIDTH 7
#define LETTER_HEIGHT 7
#define SPACING 2

const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.- ";
const char *IMG_FONT = "res/font.png";

TextRenderer *TextRenderer::instance = NULL;
SDL_Renderer *TextRenderer::renderer = NULL;
SDL_Texture *TextRenderer::texture = NULL;

TextRenderer *TextRenderer::getInstance(SDL_Renderer *renderer) {
    if (instance == NULL) instance = new TextRenderer(renderer);
    return instance;
}

TextRenderer::TextRenderer(SDL_Renderer *renderer) {
    this->renderer = renderer;
    SDL_Surface* surface = IMG_Load(IMG_FONT);

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srcRect = {0, 0, LETTER_WIDTH, LETTER_HEIGHT};
}

void TextRenderer::renderText(punto_t punto, const char *text, float resize, color_t color) {
    srcRect.y = color * LETTER_Y;
    dstRect.x = punto.x;
    dstRect.y = punto.y;
    dstRect.w = round(resize * (LETTER_WIDTH * ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    dstRect.h = round(resize * (LETTER_HEIGHT * ALTO_PANTALLA / (float)ALTO_NIVEL));
    char c;
    while ((c = *text++) != '\0') {
        srcRect.x = CHARACTERS.find_first_of(toupper(c)) * LETTER_X;

        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
        dstRect.x += round(resize * ((LETTER_WIDTH + SPACING) * ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    }
}

bool TextRenderer::includesCharacter(char character) {
    return CHARACTERS.find_first_of(toupper(character)) != CHARACTERS.npos;
}

TextRenderer::~TextRenderer() {
    SDL_DestroyTexture(texture);
    texture = NULL;
};