#include <SDL2/SDL.h>
#include "utils/punto.h"

class TextRenderer
{
    public: 
        TextRenderer(SDL_Renderer *, const char *);
        void setColor(Uint8);
        void renderText(punto_t, const char *, float);
        bool includesCharacter(char);
        ~TextRenderer();

    private:
        static SDL_Renderer *renderer;
        static SDL_Texture *texture;
        SDL_Rect srcRect;
        SDL_Rect dstRect;
};