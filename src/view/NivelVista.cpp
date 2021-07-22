#include <string>
#include <SDL2/SDL_image.h>
#include "NivelVista.h"
#include "PaulineVista.h"
#include "DonkeyKongVista.h"
#include "DefaultConfigVista.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/Constants.hpp"

NivelVista::NivelVista(SDL_Renderer *renderer, const char *clientUsername) : statsVista(renderer), hammerVista(renderer) {
    this->renderer = renderer;
    strcpy(this->clientUsername, clientUsername);

    entidadesVista.push_back(new PaulineVista(renderer));
    entidadesVista.push_back(new DonkeyKongVista(renderer));

    const configuration::GameConfiguration *const config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    if (config->getDefaultConfigFlag())
        entidadesVista.push_back(new DefaultConfigVista(renderer));

    auto maxPlayers = config->getMaxPlayers();
    if (maxPlayers < MIN_PLAYERS || MAX_PLAYERS < maxPlayers) maxPlayers = DEFAULT_MAX_PLAYERS;
    addPlayers(maxPlayers);
}

void NivelVista::addPlayers(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        jugadoresVista.emplace_back(renderer);
    }
}

void NivelVista::setBackground(const std::string &rutaImagen) {
    SDL_Surface* surface = IMG_Load(rutaImagen.c_str());
    if (surface == NULL) {
        logger::Logger::getInstance().logError("Image not found: " + rutaImagen);
        logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
        surface = IMG_Load(IMG_DEFAULT.c_str());
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

NivelVista::~NivelVista() {
    SDL_DestroyTexture(texture);

    jugadoresVista.clear();
    entidadesVista.clear();
}