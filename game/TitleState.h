//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_TITLESTATE_H
#define IDJ201601T1_TITLESTATE_H


#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"

class TitleState : public State {

private:
    Sprite bg;
    Text infoText;

    bool shouldDisplayInfoText;
    Timer timer;

public:

    TitleState();

    void update(float);

    void render();

    void pause();

    void resume();


};


#endif //IDJ201601T1_TITLESTATE_H
