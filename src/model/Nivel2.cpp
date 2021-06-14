#include "Nivel2.h"
#include "../utils/Constants.hpp"
#include "nivel/Escalera.h"

const int ESCALERA_1_X0 = 184;
const int ESCALERA_1_X1 = 192;
const int ESCALERA_1_Y0 = 248;
const int ESCALERA_1_Y1 = 219;

const int ESCALERA_2_X0 = 96;
const int ESCALERA_2_X1 = 104;
const int ESCALERA_2_Y0 = 219;
const int ESCALERA_2_Y1 = 182;

const int ESCALERA_3_X0 = 32;
const int ESCALERA_3_X1 = 40;
const int ESCALERA_3_Y0 = 219;
const int ESCALERA_3_Y1 = 182;

const int ESCALERA_4_X0 = 112;
const int ESCALERA_4_X1 = 120;
const int ESCALERA_4_Y0 = 182;
const int ESCALERA_4_Y1 = 149;

const int ESCALERA_5_X0 = 184;
const int ESCALERA_5_X1 = 192;
const int ESCALERA_5_Y0 = 182;
const int ESCALERA_5_Y1 = 149;

const int ESCALERA_6_X0 = 72;
const int ESCALERA_6_X1 = 80;
const int ESCALERA_6_Y0 = 149;
const int ESCALERA_6_Y1 = 118;

const int ESCALERA_7_X0 = 32;
const int ESCALERA_7_X1 = 40;
const int ESCALERA_7_Y0 = 149;
const int ESCALERA_7_Y1 = 118;

const int ESCALERA_8_X0 = 184;
const int ESCALERA_8_X1 = 192;
const int ESCALERA_8_Y0 = 118;
const int ESCALERA_8_Y1 = 87;

const int ESCALERA_9_X0 = 128;
const int ESCALERA_9_X1 = 136;
const int ESCALERA_9_Y0 = 87;
const int ESCALERA_9_Y1 = 56;

Nivel2::Nivel2() : Nivel() {
    this->inicializarPlataformas();
    this->inicializarEscaleras();
}

void Nivel2::update() {
    if (++tick % 128 == 0) addBarrel();

    this->updateBarrels();
    this->updatePlayers();
}

void Nivel2::addBarrel() {
    int x = rand() % (ANCHO_NIVEL - ANCHO_BARRIL);
    this->barriles.push_front(new Barril(x, N2_POS_Y_BARRIL));
}

void Nivel2::inicializarPlataformas() {
    this->plataformas.push_back(new Plataforma(0, 248, 223, 248));
    this->plataformas.push_back(new Plataforma(0, 219, 208, 219));
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
    
    Escalera * e2 = new Escalera("E2",
                                ESCALERA_2_X1 - 12,
                                ESCALERA_2_X0,
                                ESCALERA_2_X1,
                                ESCALERA_2_Y0,
                                ESCALERA_2_Y1);

    Escalera * e3 = new Escalera("E3",
                                ESCALERA_3_X1 - 12,
                                ESCALERA_3_X0,
                                ESCALERA_3_X1,
                                ESCALERA_3_Y0,
                                ESCALERA_3_Y1);
    
    Escalera * e4 = new Escalera("E4",
                                ESCALERA_4_X1 - 12,
                                ESCALERA_4_X0,
                                ESCALERA_4_X1,
                                ESCALERA_4_Y0,
                                ESCALERA_4_Y1);

    Escalera * e5 = new Escalera("E5",
                                ESCALERA_5_X1 - 12,
                                ESCALERA_5_X0,
                                ESCALERA_5_X1,
                                ESCALERA_5_Y0,
                                ESCALERA_5_Y1);
    
    Escalera * e6 = new Escalera("E5",
                                ESCALERA_6_X1 - 12,
                                ESCALERA_6_X0,
                                ESCALERA_6_X1,
                                ESCALERA_6_Y0,
                                ESCALERA_6_Y1);
    
    Escalera * e7 = new Escalera("E5",
                                ESCALERA_7_X1 - 12,
                                ESCALERA_7_X0,
                                ESCALERA_7_X1,
                                ESCALERA_7_Y0,
                                ESCALERA_7_Y1);

    
    Escalera * e8 = new Escalera("E5",
                                ESCALERA_8_X1 - 12,
                                ESCALERA_8_X0,
                                ESCALERA_8_X1,
                                ESCALERA_8_Y0,
                                ESCALERA_8_Y1);

    Escalera * e9 = new Escalera("E5",
                                ESCALERA_9_X1 - 12,
                                ESCALERA_9_X0,
                                ESCALERA_9_X1,
                                ESCALERA_9_Y0,
                                ESCALERA_9_Y1);

    this->escaleras_2.push_back(e1);
    this->escaleras_2.push_back(e2);
    this->escaleras_2.push_back(e3);
    this->escaleras_2.push_back(e4);
    this->escaleras_2.push_back(e5);
    this->escaleras_2.push_back(e6);
    this->escaleras_2.push_back(e7);
    this->escaleras_2.push_back(e8);
    this->escaleras_2.push_back(e9);
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
    return this->plataformas[0];
}