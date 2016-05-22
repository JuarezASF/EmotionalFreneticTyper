//
// Created by jasf on 3/15/16.
//

#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "GameException.h"
#include "Resources.h"
#include "InputManager.h"
#include "Camera.h"
#include "Logger.h"


Game *Game::instance = nullptr;

Game::Game(string name, int width, int height) {

    gameQuitRequested = false;

    screenWidth = width;
    screenHeight = height;
    srand(time(NULL));
    dt = 0.0;
    frameStart = SDL_GetTicks();

    if (!instance) {
        instance = this;
    }

    //initialize SDL
    bool success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "");

    if (!success) {
        string error_msg(error_msg = SDL_GetError());
        error_msg = "Could not initialize SDL:\n" + error_msg;
        throw GameException(error_msg);
    }

    //initialize image module and check if process went OK
    map<int, string> code_name_map = {{IMG_INIT_TIF, "tif"},
                                      {IMG_INIT_JPG, "jpg"},
                                      {IMG_INIT_PNG, "png"}};
    vector<int> image_formats{IMG_INIT_TIF, IMG_INIT_JPG, IMG_INIT_PNG};
    //or between all desired formats
    int image_settings = accumulate(image_formats.begin(),
                                    image_formats.end(),
                                    0,
                                    [](const int &a, const int &b) {
                                        return a | b;
                                    }
    );
    int obtained_image_settings = IMG_Init(image_settings);
    if (image_settings != obtained_image_settings) {
        string error_msg_main = SDL_GetError();
        string error_msg = "Could not initiazlie image libary for type:";
        for (auto format : image_formats)
            if ((format & obtained_image_settings) == 0) {
                error_msg += code_name_map[format];
            }
        error_msg += "\n";
        error_msg = error_msg_main + error_msg;
        throw GameException(error_msg);
    }

    //initia SDL sound module

    int audio_modules = MIX_INIT_OGG;
    int res;
    if ((res = Mix_Init(audio_modules)) != audio_modules) {
        if ((MIX_INIT_OGG & res ) == 0 ){
            cerr << "OGG flag not in res!" << endl;
        }
        if ((MIX_INIT_MP3 & res ) == 0 ){
            cerr << "MP3 flag not in res!" << endl;
        }
        throw GameException("Problem when initiating SDL audio!");
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
        throw GameException("Problem when initiating SDL audio!");

    }

    //initialize SDL text module

    if(TTF_Init() != 0)
    {
        std::cerr << "Could not initialize TTF module!" << std::endl;
        requestQuit();
    }




    //create window for game

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (!window)
        throw GameException("Could not load window!");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
        throw GameException("Could not load renderer!");

    Camera::setLvsOfParalax(3, std::vector<float>{0.9, 1.0, 1.2});

}


SDL_Renderer *Game::getRenderer() {
    return this->renderer;
}

SDL_Window *Game::GetWindow() {
    return this->window;
}

Game &Game::getInstance() {
    return *instance;
}

State &Game::getCurrentState() {
    return *stateStack.top();
}

Game::~Game() {
    while (!stateStack.empty())
        stateStack.pop();

    //shut img library down
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();
    //shut sdl down
    SDL_Quit();

}

void Game::run() {
    if (storedState) {
        stateStack.emplace(&(*storedState));
        storedState = nullptr;
    }
    else {
        Logger::err("Invalid initial state!");
        return;
    }

    InputManager &im = InputManager::getInstance();

    calculateDeltaTime();

    while (!stateStack.empty() && !stateStack.top()->getQuitRequested() && !gameQuitRequested) {


        calculateDeltaTime();

        im.update();
        stateStack.top()->update(getDeltaTime());
        stateStack.top()->render();
        SDL_RenderPresent(renderer);

        if (stateStack.top()->getPopRequested()) {
            stateStack.pop();
            stateStack.top()->resume();
        }

        if (storedState) {
            stateStack.top()->pause();
            stateStack.emplace(&(*storedState));
            storedState = nullptr;

        }
        SDL_Delay(33);
    }

    Resources::clearImages();


}

void Game::calculateDeltaTime() {

    unsigned int tmp = frameStart;
    frameStart = SDL_GetTicks();
    dt = max((frameStart - tmp) / 1000.0, 0.001);

}

float Game::getDeltaTime() {
    return dt;
}

Vec2 Game::getScreenDimensions() {
    return Vec2(screenWidth, screenHeight);
}

void Game::push(State *state) {

    storedState = state;

}

void Game::requestQuit() {
    gameQuitRequested = true;
}
