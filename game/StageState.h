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
#include "Panel.h"
#include "Text.h"
#include "Timer.h"

class StageState : public State {
protected:
    Sprite bg;
    TileSet tileSet;
    TileMap tileMap;
    Panel stagePanel;
    Text startText;
    bool startWait, showStartText;
    Timer startTextTimer;
    GameObject *mainPlayerStart;

public:
    StageState();

    virtual ~StageState();

    void update(float dt);
    void render();
    void pause();
    void resume();

    Sprite getBg();

    //StateInfo getStateInfo();

    Panel& getPanel();

    vector<string> recentlyUsedWords;
    string usedEmotion;

    vector<pair<unsigned, unsigned>> checkForCollision() const;
};


#endif // IDJ201601T1_STAGE_STATE_H
