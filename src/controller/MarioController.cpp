#include <SDL2/SDL.h>
#include "MarioController.h"

controls_t getControls() {
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    return controls_t{keyboard[SDL_SCANCODE_SPACE],
                      keyboard[SDL_SCANCODE_UP],
                      keyboard[SDL_SCANCODE_DOWN],
                      keyboard[SDL_SCANCODE_LEFT],
                      keyboard[SDL_SCANCODE_RIGHT]};
}