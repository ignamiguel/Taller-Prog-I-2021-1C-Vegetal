#include "Nivel2Vista.h"
#include "FireBarrelVista.h"
#include "FuegoVista.h"
#include "PeachVista.h"
#include "DonkeyKongVista.h"
#include "DefaultConfigVista.h"
#include "../utils/Constants.hpp"

Nivel2Vista::Nivel2Vista(SDL_Renderer *renderer, bool defaultConfig)
: NivelVista(renderer) {
    barrilVista = new BarrilVista(renderer);

    entidadesVista.push_back(new FireBarrelVista(renderer));
    entidadesVista.push_back(new FuegoVista(N2_POS_X_FUEGO, N2_POS_Y_FUEGO, renderer));

    entidadesVista.push_back(new PeachVista(renderer));
    entidadesVista.push_back(new DonkeyKongVista(renderer));

    if (defaultConfig) entidadesVista.push_back(new DefaultConfigVista(renderer));

}

void Nivel2Vista::update(estadoNivel_t *estadoNivel) {
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    for (EntidadEstaticaVista *vista : entidadesVista) {
        vista->mostrar();
    }

    barrilVista->setTotal(sizeof(estadoNivel->barrels));
    for (punto_t pos : estadoNivel->barrels) {
        barrilVista->mover(pos);
        barrilVista->mostrar();
    }

    size_t i = 0;
    for (estadoMario_t estadoMario : estadoNivel->players) {
        jugadoresVista.at(i++)->mostrar(estadoMario.pos, estadoMario.estado);
    }
}

Nivel2Vista::~Nivel2Vista() {
    delete barrilVista;
}