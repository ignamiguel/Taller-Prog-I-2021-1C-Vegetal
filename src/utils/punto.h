#pragma once

typedef struct punto
{
    float x;
    float y;
    constexpr bool operator==(const punto &rhs) const { return x == rhs.x && y == rhs.y; }
} punto_t;