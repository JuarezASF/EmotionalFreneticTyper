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
    Music music;
    Text instruction = Text("font/goodfoot.ttf", 40, Text::TextStyle::BLENDED, "Press space to restart and ESC to title screen", WHITE, 0, 0);

public:

    EndState(StateData stateData);

    void update(float dt);
    void render();

    void pause();
    void resume();

};


#endif //IDJ201601T1_ENDSTATE_H
