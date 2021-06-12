#include "Nivel2.h"
#include "../utils/Constants.hpp"
#include "nivel/Escalera.h"

const int ESCALERA_1_X0 = 176;
const int ESCALERA_1_X1 = 184;
const int ESCALERA_1_Y0 = 232;
const int ESCALERA_1_Y1 = 203;

const int MAX_BARRILES = 20;
Nivel2::Nivel2() : Nivel() {}

void Nivel2::update() {
    if (++tick % 128 == 0) addBarrel();

    this->updateBarrels();
    this->updatePlayers();
    this->inicializarEscaleras();
}

void Nivel2::addBarrel() {
    int x = rand() % (ANCHO_NIVEL - ANCHO_BARRIL);
    this->barriles.push_back(new Barril(x, N2_POS_Y_BARRIL));
}

void Nivel2::updateBarrels() {
    std::vector<Barril*>::iterator it;
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
                                ESCALERA_1_X1 -4,
                                ESCALERA_1_X0,
                                ESCALERA_1_X1,
                                ESCALERA_1_Y0,
                                ESCALERA_1_Y1);
    
    this->escaleras[0] = e1;
}

Escalera* Nivel2::getEscalera(punto_t p) {
    if (p.x >= ESCALERA_1_X0 
        && p.x <= ESCALERA_1_X1 
        && p.y <= ESCALERA_1_Y0
        && p.y >= ESCALERA_1_Y1) {
            return this->escaleras[0];
    }
    return NULL;
}

estadoNivel_t* Nivel2::getEstado() {
    
    for (unsigned int i = 0; i < MAX_BARRILES; i++) {
        if(barriles.size() < i)
            estadoNivel->barrels[i] = barriles.at(i)->getPos();
    }
    
    estadoNivel->players[0] = this->jugadores.front()->getEstado(); //TODO: Agregar todos los jugadores

    return estadoNivel;
}

Nivel2::~Nivel2() {
    std::vector<Barril*>::iterator it;
    for (it = barriles.begin(); it != barriles.end(); ++it) delete (*it);

    barriles.clear();
}