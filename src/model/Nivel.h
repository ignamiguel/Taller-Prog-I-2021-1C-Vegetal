#pragma once

#include <list>
#include <vector>
#include "Mario.hpp"
#include "EnemigoFuego.h"
#include "Plataforma.h"
#include "../utils/estadoNivel.h"
#include "nivel/Escalera.h"
#include <map>

class Mario;

class Nivel
{
    public:
        Nivel();
        void addPlayer(Mario *jugador);
        void addEnemies(unsigned int amount);
        virtual void update() = 0;
        virtual estadoNivel_t* getEstado() = 0;
        virtual ~Nivel();
        Escalera* getEscalera(punto_t p);
        virtual Plataforma* getPlataformaInicial() = 0;
        Plataforma* getPlataforma(punto_t p);

    protected:
        std::vector<Plataforma*> plataformas;
        std::list<EnemigoFuego*> enemies;
        std::list<Mario*> jugadores;
        std::map<int, Escalera*> escaleras;
        std::list<Escalera*> escaleras_2;
        estadoNivel_t *estadoNivel;

        void updatePlayers();
        void updateEnemies();
};