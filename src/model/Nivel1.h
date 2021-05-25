#pragma once

#include <list>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "../view/ComponenteVistaNivel.h"
#include "Entidad.h"
#include "NivelBase.h"
#include "PlataformaEstatica.h"

struct punto_t {
  float x;
  float y;
};

class Nivel1 : public NivelBase
{
    public:
        Nivel1(SDL_Renderer* renderer, bool useDefaultConfig);
        void actualizarNivel();
        void actualizarVista();
        void agregarObjeto(Entidad* objeto);
        std::list<Entidad*>* getObjetos();
        void setFondo(std::string rutaImagen);
        void inicializarObjetos(SDL_Renderer* renderer);
        punto_t getPosicionAleatoria();

    private:
        ComponenteVistaNivel* compVista;
        std::list<Entidad*> objetos;
        std::vector<PlataformaEstatica*> plataformasEstaticas;
        static std::string rutaImagen;
        void inicializarPlataformasEstaticas();
};