#include <stdlib.h>
#include "Plataforma.h"

Plataforma::Plataforma(float x1, float y1, float x2, float y2) {
    this->extremo1.x = x1;
    this->extremo1.y = y1;

    this->extremo2.x = x2;
    this->extremo2.y = y2;
}

punto_t Plataforma::getPosicionAleatoria(int anchoEntidad) {
    punto_t pos;

    float t = rand() / (float)RAND_MAX;
    pos.x = extremo1.x + t * ((extremo2.x - anchoEntidad) - extremo1.x);
    pos.y = extremo1.y + t * (extremo2.y - extremo1.y);

    return pos;
}

void Plataforma::getLimites(float *min, float *max) {
    *min = extremo1.x;
    *max = extremo2.x;
}

punto_t Plataforma::getInicio() {
    return this->extremo1;
}

punto_t Plataforma::getFinal() {
    return this->extremo2;
}
