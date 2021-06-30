#include <iostream>
#include <SDL2/SDL_image.h>
#include "StartPageView.h"
#include "utils/Constants.hpp"
#include "utils/window.hpp"
#include "model/Client.h"
#include "utils/GameAbortedException.h"

#define TEXT_BUTTON_X 28
#define USER_BUTTON_Y 78
#define PASS_BUTTON_Y 118
#define DONE_BUTTON_X 112
#define DONE_BUTTON_WIDTH (((7 + 2) * 4 + 1) * RESIZE)
#define DONE_BUTTON_Y 168

#define RESIZE 1.5f
#define TEXT_BUTTON_WIDTH (((7 + 2) * 10 + 2) * RESIZE)
#define BUTTON_HEIGHT ((7 + 4) * RESIZE)

#define ERROR_MSG_X 74
#define ERROR_MSG_Y 138

const char* USERNAME = "USERNAME";
const char* PASSWORD = "PASSWORD";
const char* DONE = "DONE";

const char* MSG_OK = "OK";
const char* MSG_ABORTED = "SERVER DISCONNECTED";
const char* MSG_INVALID_USER = "INVALID USER";
const char* MSG_INVALID_PASS = "INVALID PASSWORD";
const char* MSG_USER_ALREADY_CONNECTED = "USER ALREADY CONNECTED";
const char* MSG_MAX_USERS_CONNECTED = "MAX USERS CONNECTED";

const SDL_Rect usernameRect = {(int)(TEXT_BUTTON_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(USER_BUTTON_Y * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f),
                               (int)(TEXT_BUTTON_WIDTH * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(BUTTON_HEIGHT * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f)};

const SDL_Rect passwordRect = {(int)(TEXT_BUTTON_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(PASS_BUTTON_Y * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f),
                               (int)(TEXT_BUTTON_WIDTH * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(BUTTON_HEIGHT * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f)};

const SDL_Rect doneRect = {(int)(DONE_BUTTON_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                           (int)(DONE_BUTTON_Y * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f),
                           (int)(DONE_BUTTON_WIDTH * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                           (int)(BUTTON_HEIGHT * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f)};

StartPage::StartPage(SDL_Renderer *renderer) {
    this->renderer = renderer;
    this->textRenderer = TextRenderer::getInstance(renderer);
}

user_t StartPage::getLoginUser() {
    SDL_StartTextInput();

    bool loginDone = false;
    SDL_Event event;

    int inicio, fin;
    
    while (!loginDone) {
        inicio = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "aborted" << std::endl;
                throw GameAborted;
            }
            else if (event.type != SDL_MOUSEMOTION)
                loginDone = handle(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        show();
        SDL_RenderPresent(renderer);

        fin = SDL_GetTicks();
        SDL_Delay(std::max(MS_PER_UPDATE - (fin - inicio), 0));
    }

    SDL_StopTextInput();
    user_t user;
    strcpy (user.username, username.c_str());
    strcpy (user.password, password.c_str());

    std::cout << "capturing user [" << user.username << " " << user.password << "]" << std::endl;
    return user;
}

int StartPage::setFocusColor(int focus) {
    return SDL_SetRenderDrawColor(renderer, 128 + focus * 127, (1 - focus) * 128, (1 - focus) * 128, 255);
}

void StartPage::show() {
    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = (TEXT_BUTTON_X + 2 * RESIZE) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (58 + 2 * RESIZE) * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, USERNAME, RESIZE);

    pos.y += 20 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, username.c_str(), RESIZE);
    setFocusColor(focus == 0);
    SDL_RenderDrawRect(renderer, &usernameRect);

    pos.y += 20 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, PASSWORD, RESIZE);

    pos.y += 20 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, password.c_str(), RESIZE);
    setFocusColor(focus == 1);
    SDL_RenderDrawRect(renderer, &passwordRect);

    pos.x = (DONE_BUTTON_X + 2 * RESIZE) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (DONE_BUTTON_Y + 2 * RESIZE) * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, DONE, RESIZE);
    setFocusColor(focus == 2);
    SDL_RenderDrawRect(renderer, &doneRect);

    showError();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void StartPage::showError() {
    punto_t pos;
    pos.x = (TEXT_BUTTON_X + 2 * RESIZE) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = ERROR_MSG_Y * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, this->resultMsg.c_str(), 1, RED);
}

bool StartPage::mouseOnUsernameButton(int x, int y) {
    return usernameRect.y <= y && y <= usernameRect.y + usernameRect.h
        && usernameRect.x <= x && x <= usernameRect.x + usernameRect.w;
}

bool StartPage::mouseOnPasswordButton(int x, int y) {
    return passwordRect.y <= y && y <= passwordRect.y + passwordRect.h
        && passwordRect.x <= x && x <= passwordRect.x + passwordRect.w;
}

bool StartPage::mouseOnDoneButton(int x, int y) {
    return doneRect.y <= y && y <= doneRect.y + doneRect.h
        && doneRect.x <= x && x <= doneRect.x + doneRect.w;
}

bool StartPage::handle(SDL_Event event) {
    
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        
        int x = event.button.x;
        int y = event.button.y;

        if (mouseOnUsernameButton(x, y)) {
            focus = 0;
        } else if (mouseOnPasswordButton(x, y)) {
            focus = 1;
        } else if (mouseOnDoneButton(x, y)) {
            return true;
        }
    } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (focus == 0 && !username.empty()) {
                    username.pop_back();
                } else if (focus == 1 && !password.empty()) {
                    password.pop_back();
                }
                break;
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
                if (focus) return true;
                [[fallthrough]];
            case SDLK_TAB:
                focus = (focus + 1) % 3;
                break;
            default:
                break;
        }
    } else if (event.type == SDL_TEXTINPUT) {
        char* value = event.text.text;
        if (textRenderer->includesCharacter(*value)) {
            if (focus == 0 && username.size() < 10) {
                username += value;
            }
            else if (focus == 1 && password.size() < 10) {
                password += value;
            }
        }
    }
    return false;
}

void StartPage::renderResponse(int response) {
    switch (response)
    {
        case LOGIN_OK:
            this->resultMsg = MSG_OK;
            break;
        case LOGIN_ABORTED:
            this->resultMsg = MSG_ABORTED;
            break;
        case LOGIN_INVALID_USER_PASS:
            this->resultMsg = MSG_INVALID_PASS;
            break;
        case LOGIN_USER_ALREADY_CONNECTED:
            this->resultMsg = MSG_USER_ALREADY_CONNECTED;
            break;
        case LOGIN_MAX_USERS_CONNECTED:
            this->resultMsg = MSG_MAX_USERS_CONNECTED;
            break;
        case LOGIN_INVALID_USER:
            this->resultMsg = MSG_INVALID_USER;
            break;
        default:
            break;
    }
}