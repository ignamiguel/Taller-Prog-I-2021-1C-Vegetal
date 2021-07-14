#include "Tile.h"

void Tile::addPlatform(const Platform *platform) {
    platforms.push_back(platform);
}

void Tile::getPlatforms(std::unordered_set<const Platform *> &platforms) const {
    platforms.insert(this->platforms.begin(), this->platforms.end());
}

void Tile::setLadderBottom(const Ladder *ladder) {
    ladderBottom = ladder;
}

void Tile::setLadderTop(const Ladder *ladder) {
    ladderTop = ladder;
}

Tile::~Tile() {
    platforms.clear();
    delete ladderBottom;
}