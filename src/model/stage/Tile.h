#pragma once
#include <unordered_set>
#include <list>
#include "Platform.h"
#include "Ladder.h"

// Representa una celda de dimension ANCHO_TILE y ALTO_TILE
// que contiene los elementos con los que Mario puede interactuar
class Tile {
    public:
        void addPlatform(Platform *platform);
        void getPlatforms(std::unordered_set<Platform *> *);
        void setLadderBottom(Ladder *ladder);
        void setLadderTop(Ladder *ladder);
        Ladder *getLadderBottom();
        Ladder *getLadderTop();
        ~Tile();

    private:
        Ladder *ladderBottom = NULL;
        Ladder *ladderTop = NULL;
        std::list<Platform *> platforms;
};