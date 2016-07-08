//
// Created by jasf on 5/22/16.
//

#ifndef IDJ201601T1_ENDSTATE_H
#define IDJ201601T1_ENDSTATE_H


#include "Sprite.h"
#include "Music.h"
#include "Text.h"
#include "State.h"
#include "StateData.h"
#include "defines.h"

class EndState : public State{
private:
    Sprite bg;
    Music winMusic;
    Music loseMusic;
    Text instruction = Text("font/goodfoot.ttf", 40, Text::TextStyle::BLENDED, "Type again to restart and quit to leave", WHITE, 0, 0);
    Timer timer;
    bool show;

public:

    EndState(StateData stateData);

    void update(float dt);
    void render();

    void pause();
    void resume();

};


#endif //IDJ201601T1_ENDSTATE_H
