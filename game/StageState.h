//
// Created by jasf on 3/16/16.
//

#ifndef IDJ201601T1_STAGE_STATE_H
#define IDJ201601T1_STAGE_STATE_H


#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>

#include "Sprite.h"
#include "GameObject.h"
#include "TileSet.h"
#include "TileMap.h"
#include "State.h"
#include "Music.h"

class StageState : public State {
protected:
    Sprite bg;
    TileSet tileSet;
    TileMap tileMap;

    Music gameBackroundMusic;

public:
    StageState();

    virtual ~StageState();

    void update(float dt);
    void render();
    void pause();
    void resume();

    Sprite getBg();


};


#endif // IDJ201601T1_STAGE_STATE_H
