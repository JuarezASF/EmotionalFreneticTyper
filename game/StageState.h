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
#include "Sound.h"
#include "Panel.h"
#include "Text.h"
#include "Timer.h"
#include "PlataformasManager.h"

class StageState : public State {

public:
    typedef enum GameState {
        INITIAL_PAUSED_STATE, PLAYING, PAUSED
    } GameState;

    typedef enum MusicState {
		MUSIC_INITIAL_WAIT, MUSIC_START_EFFECT, MUSIC_GAMEPLAY_INTRO, MUSIC_GAMEPLAY_LOOP, MUSIC_PAUSED
	} MusicState;

protected:
    TileSet tileSet;
    TileMap tileMap;
    Panel stagePanel;
    Text startText;
    GameObject *mainPlayerPtr;
    GameObject *trackThis;
    PlataformasManager *plataformasManager;

    Sprite lightEffetct;

    //used to show blinking msg
    Sound startEffect;
    Timer startEffectTimer;
    Music gameplayIntroMusic;
    Music gameplayLoopMusic;
    bool showText;
    Timer startTextTimer;

    GameState currentState;

    MusicState currentMusicState;
    MusicState musicStateBeforePause;
    bool musicWasPaused;

public:
    StageState();

    virtual ~StageState();

    void update(float dt);

    void render();

    void pause();

    void resume();

    Panel &getPanel();

    string usedEmotion;

    vector<pair<unsigned, unsigned>> checkForCollision() const;

    inline bool isPaused() { return currentState == PAUSED || currentState == INITIAL_PAUSED_STATE; }
};


#endif // IDJ201601T1_STAGE_STATE_H
