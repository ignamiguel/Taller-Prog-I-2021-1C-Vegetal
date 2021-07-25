#pragma once
#include "Scene.h"

#define INTERLUDE_DURATION 300

class Interlude : public Scene
{
public:
    Interlude(bool gameOver, int sceneNumber);
    void update() override { ++ticks; };
    const estadoNivel_t &getEstado() override { return estadoNivel; };
    bool isComplete() const override { return ticks > INTERLUDE_DURATION; };
private:
    size_t ticks{};
};
