#include "Nivel2Vista.h"
#include "FireBarrelVista.h"
#include "FuegoVista.h"
#include "PaulineVista.h"
#include "DonkeyKongVista.h"
#include "DefaultConfigVista.h"
#include "../utils/Constants.hpp"

Nivel2Vista::Nivel2Vista(SDL_Renderer *renderer, bool defaultConfig)
: NivelVista(renderer) {
    barrilVista = new BarrilVista(renderer);

    entidadesVista.push_back(new FireBarrelVista(renderer));
    entidadesVista.push_back(new FuegoVista(N2_POS_X_FUEGO, N2_POS_Y_FUEGO, renderer));

    entidadesVista.push_back(new PaulineVista(renderer));
    entidadesVista.push_back(new DonkeyKongVista(renderer));

    if (defaultConfig) entidadesVista.push_back(new DefaultConfigVista(renderer));

}

void Nivel2Vista::update(estadoNivel_t *estadoNivel) {
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    for (EntidadEstaticaVista *vista : entidadesVista) {
        vista->mostrar();
    }

    barrilVista->startRender();
    for (punto_t pos : estadoNivel->barrels) {
        if (pos.y == 0) break;
        barrilVista->mover(pos);
        barrilVista->mostrar();
    }

    size_t i = 0;
    for(MarioVista *player : this->jugadoresVista) {
        estadoMario_t estado = estadoNivel->players[i++];
        player->mostrar(estado.pos, estado.estado);
    }
}

Nivel2Vista::~Nivel2Vista() {
    delete barrilVista;
}