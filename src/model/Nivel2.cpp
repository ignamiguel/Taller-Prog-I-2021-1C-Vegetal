#include "Nivel2.h"
#include "../utils/Constants.hpp"
#include "nivel/Escalera.h"

const int ESCALERA_1_X0 = 184;
const int ESCALERA_1_X1 = 192;
const int ESCALERA_1_Y0 = 248;
const int ESCALERA_1_Y1 = 219;

Nivel2::Nivel2() : Nivel() {}

void Nivel2::update() {
    if (++tick % 128 == 0) addBarrel();

    this->updateBarrels();
    this->updatePlayers();
    this->inicializarEscaleras();
}

void Nivel2::addBarrel() {
    int x = rand() % (ANCHO_NIVEL - ANCHO_BARRIL);
    this->barriles.push_front(new Barril(x, N2_POS_Y_BARRIL));
}

void Nivel2::updateBarrels() {
    std::list<Barril*>::iterator it;
    for (it = barriles.begin(); it != barriles.end();) {
        (*it)->mover();
        if (!(*it)->estaEnNivel()) {
            delete (*it);
            it = this->barriles.erase(it);
        } else ++it;
    }
}

void Nivel2::inicializarEscaleras() {
    Escalera * e1 = new Escalera("E1",
                                ESCALERA_1_X1 - 12,
                                ESCALERA_1_X0,
                                ESCALERA_1_X1,
                                ESCALERA_1_Y0,
                                ESCALERA_1_Y1);
    
    this->escaleras[0] = e1;
    this->escaleras_2.push_back(e1);
}

estadoNivel_t* Nivel2::getEstado() {
    estadoNivel->barrels.clear();
    for (Barril *barril : this->barriles) {
        estadoNivel->barrels.push_back(barril->getPos());
    }
    estadoNivel->players.clear();
    for (Mario *player : this->jugadores) {
        estadoNivel->players.push_back(player->getEstado());
    }
    return estadoNivel;
}

Nivel2::~Nivel2() {
    std::list<Barril*>::iterator it;
    for (it = barriles.begin(); it != barriles.end(); ++it) delete (*it);

    barriles.clear();
}

Plataforma* Nivel2::getPlataformaInicial() {
    this->plataformas.push_back(new Plataforma(0, 248, 48, 248));
    return this->plataformas[0];
}