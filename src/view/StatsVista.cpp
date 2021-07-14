#include "StatsVista.h"
#include <SDL2/SDL_image.h>
#include "../TextRenderer.h"
#include "../logger.h"
#include "../configuration.hpp"

#define MARGEN 4 * ALTO_PANTALLA / (float)ALTO_NIVEL

#define POS_LEFT 4 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define POS_CENTER_LEFT 59 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define POS_CENTER 86 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define POS_CENTER_RIGHT 115 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define POS_RIGHT 170 * ANCHO_PANTALLA / (float)ANCHO_NIVEL

#define OFFSET_SCORE_X 2 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define OFFSET_SCORE_Y 11 * ALTO_PANTALLA / (float)ALTO_NIVEL

#define ANCHO_ICON 8
#define ANCHO_NOMBRE 27

const std::string IMG_ICONS{"res/PlayerIcon.png"};

SDL_Renderer *StatsVista::renderer{nullptr};
SDL_Texture *StatsVista::iconsTexture{nullptr};

StatsVista::StatsVista(SDL_Renderer *renderer) {
    if (iconsTexture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_ICONS.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_ICONS);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        iconsTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    auto maxPlayers = configuration::GameConfiguration::getInstance(CONFIG_FILE)->getMaxPlayers();
    if (maxPlayers < MIN_PLAYERS || MAX_PLAYERS < maxPlayers)
        maxPlayers = DEFAULT_MAX_PLAYERS;

    switch(maxPlayers) {
        case 1:
            posiciones.assign({POS_CENTER});
            break;
        case 2:
            posiciones.assign({POS_LEFT, POS_RIGHT});
            break;
        case 3:
            posiciones.assign({POS_LEFT, POS_CENTER, POS_RIGHT});
            break;
        case 4:
            posiciones.assign({POS_LEFT, POS_CENTER_LEFT, POS_CENTER_RIGHT, POS_RIGHT});
            break;
    }
}

void StatsVista::mostrar(const estadoJugador_t &estado, size_t nroJugador) {
    const punto_t top_left{posiciones.at(nroJugador), MARGEN};
    const punto_t score_pos{top_left.x + OFFSET_SCORE_X, top_left.y + OFFSET_SCORE_Y};
    const char score_str[6]{static_cast<char>('0' + (estado.score / 100) % 10),
                            static_cast<char>('0' + (estado.score / 10) % 10),
                            static_cast<char>('0' + estado.score % 10),
                            '0',
                            '0'};

    srcRect.x = nroJugador * ANCHO_ICON;
    dstRect.x = round(top_left.x + ANCHO_NOMBRE * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    for (int i = 0; i < estado.lives; ++i) {
        SDL_RenderCopy(renderer, iconsTexture, &srcRect, &dstRect);
        dstRect.x += round(ANCHO_ICON * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    }

    TextRenderer::getInstance(renderer)->renderText(top_left, estado.name, 1, WHITE);
    TextRenderer::getInstance(renderer)->renderText(score_pos, score_str, 1, WHITE);
}