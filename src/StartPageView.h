#include <string>
#include "TextRenderer.h"

class StartPage
{
    public:
        StartPage(SDL_Renderer *);
        void show();
        bool handle(SDL_Event event);
        ~StartPage();

    private:
        SDL_Renderer *renderer;
        TextRenderer *textRenderer;
        std::string username = "";
        std::string password = "";
        char focus = 0;

        void setFocusColor(int);
        bool mouseOnUsernameButton(int, int);
        bool mouseOnPasswordButton(int, int);
        bool mouseOnDoneButton(int, int);
        };