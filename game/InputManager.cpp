//
// Created by jasf on 3/25/16.
//

#include "InputManager.h"
#include "defines.h"
#include "Logger.h"
#include "Camera.h"

void InputManager::update() {
    updateCounter++;

    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);

    quitRequested = false;


    // SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
    while (SDL_PollEvent(&event) != 0) {

        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;

                break;

            case SDL_MOUSEBUTTONDOWN:

                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button] = updateCounter;

                break;

            case SDL_MOUSEBUTTONUP:
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button] = updateCounter;

                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == ESCAPE_KEY) {
                    quitRequested = true;
                }
                if (event.key.repeat)
                    continue;
                keyState[event.key.keysym.sym] = true;
                keyUpdate[event.key.keysym.sym] = updateCounter;

                break;

            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;
                keyUpdate[event.key.keysym.sym] = updateCounter;

                break;
            case SDL_MOUSEMOTION:
            case SDL_WINDOWEVENT:
            case SDL_AUDIODEVICEADDED:
            case SDL_TEXTINPUT:
                break;

            default:
                std::cerr << "Unexpecte envet of code " << event.type << std::endl;
                break;

        }
    }

}

bool InputManager::keyPress(int key) {
    return (keyUpdate[key] == updateCounter) && keyState[key];
}

bool InputManager::keyRelease(int key) {
    return (keyUpdate[key] == updateCounter) && !keyState[key];
}

bool InputManager::isKeyDown(int key) {
    return keyState[key];
}

bool InputManager::mousePress(int button) {
    return (mouseUpdate[button] == updateCounter) && mouseState[button];
}

bool InputManager::mouseRelease(int button) {
    return (mouseUpdate[button] == updateCounter) && !mouseState[button];
}

bool InputManager::isMouseDown(int button) {
    return mouseState[button];
}

int InputManager::getMouseX() {
    return mouseX;
}

int InputManager::getMouseY() {
    return mouseY;
}

bool InputManager::getQuitRequested() {
    return quitRequested;
}

InputManager &InputManager::getInstance() {
    static InputManager im;

    return im;
}

InputManager::InputManager() : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0) {
    for (unsigned int i = 0; i < INPUT_MANAGER_MOUSE_STATE_SIZE; i++) {
        mouseState[i] = false;
        mouseUpdate[i] = 0;
    }

    for (unsigned int code = 0x0; code <= 0x7F; code++) {
        keyState[code] = 0;
        keyUpdate[code] = false;
    }

    for (unsigned int code = 0x40000000; code <= 0x4000011A; code++) {
        keyState[code] = 0;
        keyUpdate[code] = false;
    }


}

InputManager::~InputManager() {

}

Vec2 InputManager::getMouse() {
    return Vec2(mouseX, mouseY) + Camera::getPos(Camera::PLAYER_GROUND_VIEW);
}
