#pragma once

#include "../Mario.hpp"
#include <string>
#include "../nivel/Escalera.h"
#include "../../utils/Constants.hpp"

class Mario;

using namespace std;

class MarioState {
    public:
        MarioState();
        virtual MarioState* handleInput(char controls, Mario* mario) = 0;
        virtual void update() = 0;
        virtual void perform() = 0;
        virtual char getEstado() = 0;
        virtual ~MarioState() = default;

        std::string getName();
        float getVelY();
        float getVelX();


    protected:
        std::string name;
        float velX;
        float velY;
};