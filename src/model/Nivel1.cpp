#include "Nivel1.h"
#include "nivel/Escalera.h"
#include "Plataforma.h"

const int ESCALERA_1_X0 = 32;
const int ESCALERA_1_X1 = 40;
const int ESCALERA_1_Y0 = 248;
const int ESCALERA_1_Y1 = 200;

const int ESCALERA_2_X0 = 216;
const int ESCALERA_2_X1 = 224;
const int ESCALERA_2_Y0 = 200;
const int ESCALERA_2_Y1 = 160;

const int ESCALERA_3_X0 = 0;
const int ESCALERA_3_X1 = 8;
const int ESCALERA_3_Y0 = 160;
const int ESCALERA_3_Y1 = 120;

const int ESCALERA_4_X0 = 216;
const int ESCALERA_4_X1 = 224;
const int ESCALERA_4_Y0 = 120;
const int ESCALERA_4_Y1 = 88;

const int ESCALERA_5_X0 = 128;
const int ESCALERA_5_X1 = 136;
const int ESCALERA_5_Y0 = 88;
const int ESCALERA_5_Y1 = 56;

Nivel1::Nivel1() : Nivel() {
    this->initPlataformas();
    this->inicializarEscaleras();
}

void Nivel1::initPlataformas() {
    plataformasMoviles.push_back(new PlataformaMovil(0, 0));
    plataformasMoviles.push_back(new PlataformaMovil(1, 0));
    plataformasMoviles.push_back(new PlataformaMovil(2, 0));

    plataformasMoviles.push_back(new PlataformaMovil(0, 1));
    plataformasMoviles.push_back(new PlataformaMovil(1, 1));
    plataformasMoviles.push_back(new PlataformaMovil(2, 1));

    plataformasMoviles.push_back(new PlataformaMovil(0, 2));
    plataformasMoviles.push_back(new PlataformaMovil(1, 2));
    plataformasMoviles.push_back(new PlataformaMovil(2, 2));

    plataformasMoviles.push_back(new PlataformaMovil(0, 3));
    plataformasMoviles.push_back(new PlataformaMovil(1, 3));
    plataformasMoviles.push_back(new PlataformaMovil(2, 3));

    this->plataformas.push_back(new Plataforma(0, 248, 48, 248));
    this->plataformas.push_back(new Plataforma(64, 248, 88, 248));
    this->plataformas.push_back(new Plataforma(104, 248, 128, 248));
    this->plataformas.push_back(new Plataforma(144, 248, 168, 248));
    this->plataformas.push_back(new Plataforma(184, 248, 224, 248));
    this->plataformas.push_back(new Plataforma(0, 200, 48, 200));
    this->plataformas.push_back(new Plataforma(64, 200, 88, 200));
    this->plataformas.push_back(new Plataforma(104, 200, 128, 200));
    this->plataformas.push_back(new Plataforma(144, 200, 168, 200));
    this->plataformas.push_back(new Plataforma(184, 200, 224, 200));
    this->plataformas.push_back(new Plataforma(0, 160, 16, 160));
    this->plataformas.push_back(new Plataforma(208, 160, 224, 160));
    this->plataformas.push_back(new Plataforma(0, 120, 16, 120));
    this->plataformas.push_back(new Plataforma(208, 120, 224, 120));
    this->plataformas.push_back(new Plataforma(208, 88, 224, 88));
    this->plataformas.push_back(new Plataforma(160, 87, 192, 87));
    this->plataformas.push_back(new Plataforma(24, 84, 144, 84));
    this->plataformas.push_back(new Plataforma(88, 56, 136, 56));
}

void Nivel1::update() {
    updatePlatforms();
    updateEnemies();
    updatePlayers();
}

void Nivel1::updatePlatforms() {
    std::list<PlataformaMovil*>::iterator it;
    for (it = plataformasMoviles.begin(); it != plataformasMoviles.end(); ++it) {
        (*it)->mover();
    }
}

estadoNivel_t* Nivel1::getEstado() {

    estadoNivel->enemies.clear();
    for (EnemigoFuego *enemy : this->enemies) {
        estadoNivel->enemies.push_back(enemy->getPos());
    }
    estadoNivel->platforms.clear();
    for (PlataformaMovil *plataforma : this->plataformasMoviles) {
        estadoNivel->platforms.push_back(plataforma->getPos());
    }
    estadoNivel->players.clear();
    for (Mario *player : this->jugadores) {
        estadoNivel->players.push_back(player->getEstado());
    }
    return estadoNivel;
}

Nivel1::~Nivel1() {
    std::list<PlataformaMovil*>::iterator it;
    for (it = plataformasMoviles.begin(); it != plataformasMoviles.end(); ++it) delete (*it);

    plataformasMoviles.clear();
}

void Nivel1::inicializarEscaleras() {
    Escalera * e1 = new Escalera("E1",
                                ESCALERA_1_X1 -12,
                                ESCALERA_1_X0,
                                ESCALERA_1_X1,
                                ESCALERA_1_Y0,
                                ESCALERA_1_Y1);
    Escalera * e2 = new Escalera("E2",
                                ESCALERA_2_X1 -12,
                                ESCALERA_2_X0,
                                ESCALERA_2_X1,
                                ESCALERA_2_Y0,
                                ESCALERA_2_Y1);
    Escalera * e3 = new Escalera("E3",
                                ESCALERA_3_X1 -12,
                                ESCALERA_3_X0,
                                ESCALERA_3_X1,
                                ESCALERA_3_Y0,
                                ESCALERA_3_Y1);
    Escalera * e4 = new Escalera("E4",
                                ESCALERA_4_X1 -12,
                                ESCALERA_4_X0,
                                ESCALERA_4_X1,
                                ESCALERA_4_Y0,
                                ESCALERA_4_Y1);
    Escalera * e5 = new Escalera("E5",
                                ESCALERA_5_X1 -12,
                                ESCALERA_5_X0,
                                ESCALERA_5_X1,
                                ESCALERA_5_Y0,
                                ESCALERA_5_Y1);

    this->escaleras_2.push_back(e1);
    this->escaleras_2.push_back(e2);
    this->escaleras_2.push_back(e3);
    this->escaleras_2.push_back(e4);
    this->escaleras_2.push_back(e5);
}

// Escalera* Nivel1::getEscalera(punto_t p) {
//     std::list<Escalera*>::iterator it;
//     for (it = this->escaleras_2.begin(); it != this->escaleras_2.end(); ++it) {
//         Escalera* e = (*it);
        
//         // Considero que si Mario tiene la mitad del cuerpo
//         // en el rango de la escalera
//         // ya puedo subirla
//         if (p.x >= (e->getX0() - ANCHO_MARIO / 2)
//         && p.x <= (e->getX1() - ANCHO_MARIO / 2) 
//         && p.y <= (e->getY0() - ALTO_MARIO)
//         && p.y >= (e->getY1() - ALTO_MARIO)) {
//             return e;
//         }
//     }
//     return NULL;
// }

Plataforma* Nivel1::getPlataformaInicial() {
    return this->plataformas[0];
}

Plataforma* Nivel1::getPlataforma(punto_t p) {
    // std::vector<Plataforma*>::iterator it;
    // for (it = this->plataformas.begin(); it != this->plataformas.end(); ++it) {
    //     punto_t inicio = (*it)->getInicio();
    //     punto_t final = (*it)->getFinal();

    //     if (p.x >= inicio.x 
    //     && p.x <= final.x
    //     && p.y <= ESCALERA_1_Y0
    //     && p.y >= ESCALERA_1_Y1) {
    //         return this->escaleras[0];
    // }
    return NULL;
}