#include "../TextRenderer.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "DesconexionVista.h"

static const char* IMG_FONT = "res/font.png";

void DesconexionVista::mostrar(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);

    TextRenderer* textRenderer = new TextRenderer(renderer, IMG_FONT);
    
    punto_t pos;
    pos.x = (5 + 2) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (110 + 2) * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, "Desconexion del servidor", 1);

    SDL_RenderPresent(renderer);
    delete textRenderer;
}