#include "exitVista.h"
#include "../TextRenderer.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"

namespace exitVista {
    void showDesconexion(SDL_Renderer* renderer) {
        SDL_RenderClear(renderer);
        {
            const punto_t pos{18.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                              112 * ALTO_PANTALLA / (float)ALTO_NIVEL};
            TextRenderer::getInstance(renderer)->renderText(pos, "SE PERDIO LA CONEXION", 1);
        }
        {
            const punto_t pos{45.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                              122 * ALTO_PANTALLA / (float)ALTO_NIVEL};
            TextRenderer::getInstance(renderer)->renderText(pos, "CON EL SERVIDOR", 1);
        }
        SDL_RenderPresent(renderer);
    }

    void showGameOver(SDL_Renderer *renderer) {
        SDL_RenderClear(renderer);

        const punto_t pos{32 * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                        112 * ALTO_PANTALLA / (float)ALTO_NIVEL};
        TextRenderer::getInstance(renderer)->renderText(pos, "GAME OVER", 2);

        SDL_RenderPresent(renderer);
    }
}