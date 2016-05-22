//
// Created by jasf on 3/15/16.
//
#ifndef IDJ201601T1_GAME_H
#define IDJ201601T1_GAME_H

#include <string>
#include <stack>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "State.h"

using namespace std;

class Game {

protected:
    static Game *instance;
    SDL_Window *window;
    SDL_Renderer *renderer;
    unsigned int frameStart;
    float dt;

    //hold new state to be added at the end of the current frame
    State* storedState;

    std::stack<std::shared_ptr<State>> stateStack;

    int screenHeight;
    int screenWidth;

    bool gameQuitRequested;

    void calculateDeltaTime();

public:
    Game(string name, int width, int height);

    virtual ~Game();

    void run();

    SDL_Renderer *getRenderer();

    SDL_Window *GetWindow();

    State &getCurrentState();

    static Game &getInstance();

    float getDeltaTime();

    Vec2 getScreenDimensions();

    void push(State* state);

    void requestQuit();

};


#endif //IDJ201601T1_GAME_H
