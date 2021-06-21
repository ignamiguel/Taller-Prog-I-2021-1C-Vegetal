#include <string>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "TextRenderer.h"

#define ANCHO_PANTALLA 448
#define ALTO_PANTALLA 512
#define ANCHO_NIVEL 224
#define ALTO_NIVEL 256

#define LETTER_X 16
#define LETTER_Y 8
#define LETTER_WIDTH 7
#define LETTER_HEIGHT 7
#define SPACING 2

using namespace std;

const string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.- ";

SDL_Renderer *TextRenderer::renderer = NULL;
SDL_Texture *TextRenderer::texture = NULL;

TextRenderer::TextRenderer(SDL_Renderer *renderer, const char *path) {
    this->renderer = renderer;
    SDL_Surface* surface = IMG_Load(path);
    //if(surface == NULL) {
        //logger::Logger::getInstance().logError("Font image not found: " + rutaImagen);
        //logger::Logger::getInstance().logDebug("Loading Font default image: " + IMG_DEFAULT);
        //surface = IMG_Load(IMG_DEFAULT.c_str());
    //} 
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srcRect = {0, 0, LETTER_WIDTH, LETTER_HEIGHT};
}

void TextRenderer::setColor(Uint8 color) {
    srcRect.y = color * LETTER_Y;
}

void TextRenderer::renderText(punto_t punto, const char *text, float resize) {
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