#include "ExitVista.h"
#include "../TextRenderer.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "../utils/Messages.h"

namespace ExitVista {
    void showDesconexion(SDL_Renderer* renderer) {
        SDL_RenderClear(renderer);
        {
            const punto_t pos{18.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                              112 * ALTO_PANTALLA / (float)ALTO_NIVEL};
            TextRenderer::getInstance(renderer)->renderText(pos, MSG_DESCONEXION, 1);
        }
        {
            const punto_t pos{45.5f * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                              122 * ALTO_PANTALLA / (float)ALTO_NIVEL};
            TextRenderer::getInstance(renderer)->renderText(pos, MSG_DESCONEXION_SERVIDOR, 1);
        }
        SDL_RenderPresent(renderer);
    }

    void showGameOver(SDL_Renderer *renderer) {
        SDL_RenderClear(renderer);

        const punto_t pos{32 * ANCHO_PANTALLA / (float)ANCHO_NIVEL,
                        112 * ALTO_PANTALLA / (float)ALTO_NIVEL};
        TextRenderer::getInstance(renderer)->renderText(pos, MSG_GAME_OVER, 2);

        SDL_RenderPresent(renderer);
    }

    void showGameCompletedPage(SDL_Renderer *renderer)
    {
        SDL_RenderClear(renderer);

        punto_t pos;
        pos.x = (42) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
        pos.y = 112 * ALTO_PANTALLA / (float)ALTO_NIVEL;
        TextRenderer::getInstance(renderer)->renderText(pos, MSG_FELICITACIONES, 1);
        pos.x = (42) * (ANCHO_PANTALLA / (float)ANCHO_NIVEL);
        pos.y = (122) * (ALTO_PANTALLA / (float)ALTO_NIVEL);
        TextRenderer::getInstance(renderer)->renderText(pos, MSG_JUEGO_COMPLETADO, 1);

        SDL_RenderPresent(renderer);
    }
}