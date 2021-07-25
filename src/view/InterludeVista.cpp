#include "InterludeVista.h"
#include "../TextRenderer.h"
#include "../configuration.hpp"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define X_POS_CONGRATS 28 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define Y_POS_CONGRATS 36 * ALTO_PANTALLA / (float)ALTO_NIVEL
#define X_POS_GAME_OVER 72 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define Y_POS_GAME_OVER 48 * ALTO_PANTALLA / (float)ALTO_NIVEL

#define X_POS_USER 48 * ANCHO_PANTALLA / (float)ANCHO_NIVEL
#define Y_POS_USER 100 * ALTO_PANTALLA / (float)ALTO_NIVEL
#define Y_DIF_USERS 20 * ALTO_PANTALLA / (float)ALTO_NIVEL
#define OFFSET_SCORE 64 * ANCHO_PANTALLA / (float)ANCHO_NIVEL

InterludeVista::InterludeVista(SDL_Renderer *renderer) : SceneVista(), renderer{renderer} {
    auto maxPlayers = configuration::GameConfiguration::getInstance(CONFIG_FILE)->getMaxPlayers();
    if (maxPlayers < MIN_PLAYERS || MAX_PLAYERS < maxPlayers) maxPlayers = DEFAULT_MAX_PLAYERS;
    this->maxPlayers = maxPlayers;
}

void InterludeVista::update(const estadoJuego_t &estadoJuego) {
    punto_t statsJugadorPos{X_POS_USER, Y_POS_USER};
    auto textRenderer = TextRenderer::getInstance(renderer);

    textRenderer->renderText(statsJugadorPos, "USER", 1.25f, YELLOW);
    textRenderer->renderText({statsJugadorPos.x + OFFSET_SCORE, statsJugadorPos.y}, "SCORE", 1.25f, YELLOW);

    for (size_t i = 0; i < maxPlayers; ++i) {
        statsJugadorPos.y += Y_DIF_USERS;
        textRenderer->renderText(statsJugadorPos, estadoJuego.players[i].name, 1.25f, MAGENTA);
        const char score_str[7]{'0',
                                static_cast<char>('0' + (estadoJuego.players[i].score / 100) % 10),
                                static_cast<char>('0' + (estadoJuego.players[i].score / 10) % 10),
                                static_cast<char>('0' + estadoJuego.players[i].score % 10),
                                '0',
                                '0'};
        
        textRenderer->renderText({statsJugadorPos.x + OFFSET_SCORE, statsJugadorPos.y}, score_str, 1.25f, LIME);
    }
}
