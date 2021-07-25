#include <iostream>
#include "Nivel1Vista.h"
#include "PoleaVista.h"
#include "FuegoVista.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../controller/AudioController.h"
#include "../utils/Constants.hpp"

Nivel1Vista::Nivel1Vista(SDL_Renderer *renderer, const char* clientUsername)
: NivelVista(renderer, clientUsername), plataformaVista(renderer), enemigoVista(renderer) {

    const auto &stages = configuration::GameConfiguration::getInstance(CONFIG_FILE)->getStages();
    if (stages.size() > 0)
    {
        const std::string rutaImagen = stages.at(0).getBackgrounds().at(0);
        logger::Logger::getInstance().logDebug("Stage 1 background img: " + rutaImagen);
        this->setBackground(rutaImagen);
    }

    entidadesVista.push_back(new PoleaVista(N1_POS_X1_POLEA, N1_POS_Y1_POLEA, 0, 0, renderer));
    entidadesVista.push_back(new PoleaVista(N1_POS_X2_POLEA, N1_POS_Y1_POLEA, 1, 0, renderer));
    entidadesVista.push_back(new PoleaVista(N1_POS_X1_POLEA, N1_POS_Y2_POLEA, 0, 1, renderer));
    entidadesVista.push_back(new PoleaVista(N1_POS_X2_POLEA, N1_POS_Y2_POLEA, 1, 1, renderer));

    entidadesVista.push_back(new FuegoVista(N1_POS_X_FUEGO1, N1_POS_Y_FUEGO, renderer));
    entidadesVista.push_back(new FuegoVista(N1_POS_X_FUEGO2, N1_POS_Y_FUEGO, renderer));
    entidadesVista.push_back(new FuegoVista(N1_POS_X_FUEGO3, N1_POS_Y_FUEGO, renderer));
    entidadesVista.push_back(new FuegoVista(N1_POS_X_FUEGO4, N1_POS_Y_FUEGO, renderer));
}

void Nivel1Vista::update(const estadoJuego_t &estadoJuego) {
    size_t clientIndex = MAX_PLAYERS;
    for(unsigned int j = 0; j < this->jugadoresVista.size(); ++j) {
        if(strcmp(estadoJuego.players[j].name, clientUsername) == 0) {
            AudioController::playSounds(estadoJuego.estadoNivel.players[j].sounds);
            clientIndex = j;
        }
    }

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    for (auto &pos : estadoJuego.estadoNivel.hammers) {
        if (pos.y != 0) { //No se muestran los martillos agarrados (tienen pos.y = 0)
            hammerVista.mostrar(pos);
        }
    }

    for (EntidadEstaticaVista *vista : entidadesVista) {
        vista->mostrar();
    }

    for (auto &pos : estadoJuego.estadoNivel.platforms) {
        plataformaVista.mover(pos);
        plataformaVista.mostrar();
    }

    enemigoVista.startRender();
    for (auto &pos : estadoJuego.estadoNivel.enemies) {
        if(pos.y == 0) break; //No se muestran los enemigos muertos
        enemigoVista.mover(pos);
        enemigoVista.mostrar();
    }

    size_t i = 0;
    for (auto &player : this->jugadoresVista) {
        player.setColor((i + 1) * estadoJuego.estadoNivel.players[i].isEnabled);
        if (i != clientIndex) {
            player.mostrar(estadoJuego.estadoNivel.players[i]);
        }
        statsVista.mostrar(estadoJuego.players[i], i);
        ++i;
    }
    jugadoresVista[clientIndex].mostrar(estadoJuego.estadoNivel.players[clientIndex]); 
}