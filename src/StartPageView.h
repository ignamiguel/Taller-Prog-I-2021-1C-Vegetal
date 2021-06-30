#pragma once

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
    user_t getCurrentUser() const { return currentUser; };
    user_t getLoginUser();
    void renderResponse(int response);
    ~StartPage() = default;

private:
    SDL_Renderer *renderer;
    TextRenderer *textRenderer;
    std::string username = "";
    std::string password = "";
    std::string resultMsg = "";
    user_t currentUser;

    char focus = 0;

    int setFocusColor(int);
    bool mouseOnUsernameButton(int, int);
    bool mouseOnPasswordButton(int, int);
    bool mouseOnDoneButton(int, int);
    void showError();
};