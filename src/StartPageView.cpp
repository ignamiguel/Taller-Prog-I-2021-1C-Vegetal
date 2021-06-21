#include <SDL2/SDL_image.h>
#include "StartPageView.h"
#include "configuration.hpp"
#include "utils/Constants.hpp"
#include <map>
#include <string_view>

#include <iostream>

#define ANCHO_PANTALLA 448
#define ALTO_PANTALLA 512

#define ANCHO_NIVEL 224
#define ALTO_NIVEL 256

#define TEXT_BUTTON_X 28
#define USER_BUTTON_Y 78
#define PASS_BUTTON_Y 118
#define DONE_BUTTON_X 112
#define DONE_BUTTON_WIDTH (((7 + 2) * 4 + 1) * RESIZE)
#define DONE_BUTTON_Y 168

#define RESIZE 1.5f
#define TEXT_BUTTON_WIDTH (((7 + 2) * 10 + 2) * RESIZE)
#define BUTTON_HEIGHT ((7 + 4) * RESIZE)

const char* FONT_IMG = "res/font.png";

const char* USERNAME = "USERNAME";
const char* PASSWORD = "PASSWORD";
const char* DONE = "DONE";
const char* INVALID_USER = "INVALID USER";
const char* INVALID_PASS = "INVALID PASSWORD";
const char* LOGIN_OK = "LOGIN OK";

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

const SDL_Rect errorRect = {(int)(TEXT_BUTTON_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(USER_BUTTON_Y * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f),
                               (int)(TEXT_BUTTON_WIDTH * ANCHO_PANTALLA / (float)ANCHO_NIVEL + 0.5f),
                               (int)(BUTTON_HEIGHT * ALTO_PANTALLA / (float)ALTO_NIVEL + 0.5f)};

StartPage::StartPage(SDL_Renderer *renderer) {
    this->renderer = renderer;
    this->textRenderer = new TextRenderer(renderer, FONT_IMG);
    
    // Load users
    std::cout << "Loading users..." << std::endl;
    auto config = configuration::GameConfiguration(CONFIG_FILE);
    for (auto u: config.getUsers())
    {
        this->users[u.username] = u;
    }
}

void StartPage::setFocusColor(int focus) {
    SDL_SetRenderDrawColor(renderer, 128 + focus * 127, (1 - focus) * 128, (1 - focus) * 128, 255);
}

void StartPage::show() {

    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = (TEXT_BUTTON_X + 2 * RESIZE) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (58 + 2 * RESIZE)* ALTO_PANTALLA / (float)ALTO_NIVEL;
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

    this->showError();
}

void StartPage::showError() {
    punto_t pos;
    pos.x = .0f;
    pos.y = .0f;
    textRenderer->renderText(pos, this->resultMsg.c_str(), RESIZE);
    SDL_RenderDrawRect(renderer, &errorRect);
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
            return this->login(username, password);
        }
    } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (focus == 0 && !username.empty()){
                    username.pop_back();
                } else if (focus == 1 && !password.empty()) {
                    password.pop_back();
                }
                break;
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
                return this->login(username, password);
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

bool StartPage::login(std::string name, std::string pass) {
    std::cout << "login process..." << std::endl;
    if (this->users.count(name) == 0) {
        this->resultMsg = INVALID_USER;
        return false;
    } 
    
    auto user = this->users.at(name);

    if (pass.compare("") == 0 || pass.compare(user.password) != 0) {
        this->resultMsg = INVALID_PASS;
        return false;
    }

    this->resultMsg = LOGIN_OK;
    return true;
}

StartPage::~StartPage()  {
    delete textRenderer;
}