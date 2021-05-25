#include <stdlib.h>
#include "Nivel2.h"
#include "FireBarrel.h"
#include "Fuego.h"
#include "Peach.h"
#include "DonkeyKong.h"
#include "DefaultConfig.h"
#include "../utils/Constants.hpp"
#include "../logger.h"

using namespace std;

std::string Nivel2::rutaImagen = IMG_DEFAULT;

Nivel2::Nivel2 (SDL_Renderer* renderer, bool useDefaultConfig) {
    this->renderer = renderer;
    this->tick = 0;
    this->compVista = new ComponenteVistaNivel(renderer, rutaImagen);
    this->useDefaultConfig = useDefaultConfig;
}

void Nivel2::actualizarNivel() {
    if (!(++tick % 128)) agregarBarril();
    updateBarriles();

    std::list<Entidad*>::iterator it;
    for (it = objetos.begin(); it != objetos.end(); ++it) (*it)->mover();
}

void Nivel2::actualizarVista() {
    compVista->mostrar(&objetos);
    this->mostrarBarriles();
}

void Nivel2::agregarObjeto(Entidad* element) {
    this->objetos.push_front(element);
}

list<Entidad*>* Nivel2::getObjetos() {
    return &objetos;
}

void Nivel2::setFondo(std::string rutaImagen) {
    compVista->setFondo(rutaImagen);
}

void Nivel2::inicializarObjetos(SDL_Renderer *renderer) {
    agregarObjeto(new FireBarrel(renderer));
    agregarObjeto(new Fuego(N2_POSX_FUEGO, N2_POSY_FUEGO, renderer));

    agregarObjeto(new Peach(renderer));

    agregarObjeto(new DonkeyKong(renderer));

    if(this->getDefaultConfigFlag()) {
        this->objetos.push_front(new DefaultConfig(renderer));
    }
}

void Nivel2::agregarBarril() {
    int x = rand() % MAX_X_BARRILES;
    this->barriles.push_back(new Barril(x, 30, this->renderer));
}

void Nivel2::updateBarriles() {
    Barril *barril;
    for (int i = this->barriles.size() - 1; i >= 0; --i) {
        barril = this->barriles.at(i);
        barril->mover();
        if (!barril->estaEnNivel()) {
        this->barriles.erase(this->barriles.begin() + i);
        delete barril;
        }
    }
}

void Nivel2::mostrarBarriles() {
    for (int i = this->barriles.size() - 1; i >= 0; --i)
        this->barriles.at(i)->mostrar();
}