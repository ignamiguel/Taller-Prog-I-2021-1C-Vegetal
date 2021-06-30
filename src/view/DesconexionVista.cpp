#include "../TextRenderer.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "DesconexionVista.h"

void DesconexionVista::show(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = 18.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = 112 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    TextRenderer::getInstance(renderer)->renderText(pos, "SE PERDIO LA CONEXION", 1);
    pos.x = 45.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = 122 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    TextRenderer::getInstance(renderer)->renderText(pos, "CON EL SERVIDOR", 1);

    SDL_RenderPresent(renderer);
}