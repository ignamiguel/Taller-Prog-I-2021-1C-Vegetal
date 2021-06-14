#include <stdlib.h>
#include "Nivel.h"
#include "../utils/Constants.hpp"
#include "../logger.h"

Nivel::Nivel() {
    estadoNivel = new estadoNivel_t();
}

void Nivel::addPlayer(Mario *jugador) {
    jugadores.push_back(jugador);
}

void Nivel::addEnemies(unsigned int amount) {
    for (unsigned int i = 0; i < amount; ++i) {
        unsigned int j = 1 + (rand() % (plataformas.size() - 2));           // Omite plataformas inicial y final
        Plataforma *plataforma = plataformas[j];
        punto_t pos = plataforma->getPosicionAleatoria(ANCHO_ENEMIGO_FUEGO);
        pos.y -= ALTO_ENEMIGO_FUEGO;
        int direccion = (rand() % 2) ? -1 : 1;
        EnemigoFuego *enemy = new EnemigoFuego(pos, direccion);
        float min, max;
        plataforma->getLimites(&min, &max);
        enemy->setLimites(min, max);
        this->enemies.push_back(enemy);
    }
}

void Nivel::updatePlayers() {
    std::list<Mario*>::iterator it;
    for (it = jugadores.begin(); it != jugadores.end(); ++it) {
        (*it)->mover();
    }
}

void Nivel::updateEnemies() {
    std::list<EnemigoFuego*>::iterator it;
    for (it = enemies.begin(); it != enemies.end(); ++it) {
        (*it)->mover();
    }
}

Nivel::~Nivel() {
    estadoNivel->barrels.clear();
    estadoNivel->enemies.clear();
    estadoNivel->platforms.clear();
    estadoNivel->players.clear();
    delete estadoNivel;
    jugadores.clear();

    std::vector<Plataforma*>::iterator it;
    for (it = plataformas.begin(); it != plataformas.end(); ++it) delete (*it);

    plataformas.clear();

    std::list<EnemigoFuego*>::iterator it2;
    for (it2 = enemies.begin(); it2 != enemies.end(); ++it2) delete (*it2);

    enemies.clear();
}

Escalera* Nivel::getEscalera(punto_t p) {
    std::list<Escalera*>::iterator it;
    for (it = this->escaleras_2.begin(); it != this->escaleras_2.end(); ++it) {
        Escalera* e = (*it);
        
        // Considero que si Mario tiene la mitad del cuerpo
        // en el rango de la escalera
        // ya puedo subirla
        if (p.x >= (e->getX0() - ANCHO_MARIO / 2)
        && p.x <= (e->getX1() - ANCHO_MARIO / 2) 
        && p.y <= (e->getY0() - ALTO_MARIO)
        && p.y >= (e->getY1() - ALTO_MARIO)) {
            return e;
        }
    }
    return NULL;
}

Plataforma* Nivel::getPlataforma(punto_t p) {
    std::vector<Plataforma*>::iterator it;
    for (it = this->plataformas.begin(); it != this->plataformas.end(); ++it) {
        Plataforma* plataforma = (*it);
        punto_t inicio = plataforma->getInicio();
        punto_t final = plataforma->getFinal();

        float x = p.x < 0 ? 0 : p.x;

        if (x >= inicio.x
        && x <= final.x
        && p.y == (inicio.y - ALTO_MARIO) ) {
            return plataforma;
        }
    }
    return NULL;
}