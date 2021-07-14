#pragma once
#include <unordered_set>
#include <vector>
#include "Platform.h"
#include "Ladder.h"

// Representa una celda de dimension ANCHO_TILE y ALTO_TILE
// que contiene los elementos con los que Mario puede interactuar
class Tile
{
public:
    void addPlatform(const Platform *platform);
    void getPlatforms(std::unordered_set<const Platform *> &platforms) const;
    void setLadderBottom(const Ladder *ladder);
    void setLadderTop(const Ladder *ladder);
    const Ladder *getLadderBottom() const { return ladderBottom; }
    const Ladder *getLadderTop() const { return ladderTop; }
    ~Tile();

private:
    const Ladder *ladderBottom{nullptr};
    const Ladder *ladderTop{nullptr};
    std::vector<const Platform *> platforms;
};