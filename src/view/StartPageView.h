#pragma once

#include <string>
#include "TextRenderer.h"
#include "../utils/user.h"
#include <map>

class StartPage
{
    public:
        StartPage(SDL_Renderer *);
        user_t startLogin ();
        void show();
        bool handle(SDL_Event event);
        void connectionErrorResponse(int);
        ~StartPage();

    private:
        SDL_Renderer *renderer;
        TextRenderer *textRenderer;
        std::string username = "";
        std::string password = "";
        std::string resultMsg = "";
        std::map<std::string, user_t> users;
        user_t currentUser;
        
        char focus = 0;

        int setFocusColor(int);
        bool mouseOnUsernameButton(int, int);
        bool mouseOnPasswordButton(int, int);
        bool mouseOnDoneButton(int, int);
        void showError();
};