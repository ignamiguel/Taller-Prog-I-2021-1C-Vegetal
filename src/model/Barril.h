#pragma once
#include <array>
#include "Entidad.h"
#include "../utils/Constants.hpp"

class Barril : public Entidad
{
public:
    Barril();
    void mover();
    void updateDirection(float, float);
    bool estaEnNivel() const { return pos.x < ANCHO_NIVEL; }
    dimensiones_t dimensions() const;

private:
    static constexpr std::array<std::pair<punto_t,punto_t>, 12> direcciones {
        std::make_pair(punto_t{128, 84 - ALTO_BARRIL}, punto_t{1, 0.0625}),
        std::make_pair(punto_t{208, 89 - ALTO_BARRIL}, punto_t{0, 1}),
        std::make_pair(punto_t{208, 110 - ALTO_BARRIL}, punto_t{-1, 0.0625}),
        std::make_pair(punto_t{16 - ANCHO_BARRIL, 122.75 - ALTO_BARRIL}, punto_t{0, 1}),
        std::make_pair(punto_t{16 - ANCHO_BARRIL, 142.75 - ALTO_BARRIL}, punto_t{1, 0.0625}),
        std::make_pair(punto_t{208, 155.5 - ALTO_BARRIL}, punto_t{0, 1}),
        std::make_pair(punto_t{208, 175.5 - ALTO_BARRIL}, punto_t{-1, 0.0625}),
        std::make_pair(punto_t{16 - ANCHO_BARRIL, 188.25 - ALTO_BARRIL}, punto_t{0, 1}),
        std::make_pair(punto_t{16 - ANCHO_BARRIL, 208.25 - ALTO_BARRIL}, punto_t{1, 0.0625}),
        std::make_pair(punto_t{208, 221 - ALTO_BARRIL}, punto_t{0, 1}),
        std::make_pair(punto_t{208, 241 - ALTO_BARRIL}, punto_t{1, 0}),
        {}
    };
    punto_t vel;
    size_t direcciones_index{};
};