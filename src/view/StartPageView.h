#include <string>
#include "TextRenderer.h"
#include "../utils/user.h"

class StartPage
{
    public:
        StartPage(SDL_Renderer *);
        ~StartPage();
        void show();
        bool handle(SDL_Event event);
        user_t startLogin ();
        void connectedPage();
        void connectionErrorPage();
        void showWaitingView();

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