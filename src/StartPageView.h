#pragma once

#include <string>
#include "TextRenderer.h"
#include "utils/user.h"

class StartPage
{
public:
    StartPage(SDL_Renderer *);
    void show() const;
    bool handle(SDL_Event event);
    user_t getCurrentUser() const { return currentUser; };
    user_t getLoginUser(bool &quitRequested);
    void setResponse(char response);
    ~StartPage() = default;

private:
    SDL_Renderer *renderer;
    TextRenderer *textRenderer;
    std::string username = "";
    std::string password = "";
    const char *resultMsg;
    user_t currentUser;

    char focus = 0;

    int setFocusColor(int) const;
    bool mouseOnUsernameButton(int, int) const;
    bool mouseOnPasswordButton(int, int) const;
    bool mouseOnDoneButton(int, int) const;
    void showError() const;
};