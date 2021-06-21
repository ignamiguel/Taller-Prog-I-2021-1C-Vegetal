#include <string>
#include "TextRenderer.h"
#include "utils/user.h"
#include <map>

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
        std::string resultMsg = "";
        std::map<std::string, user_t> users;
        
        char focus = 0;

        void setFocusColor(int);
        bool mouseOnUsernameButton(int, int);
        bool mouseOnPasswordButton(int, int);
        bool mouseOnDoneButton(int, int);
        void showError();
        bool login(std::string username, std::string password);
};