//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_PANEL_H
#define IDJ201601T1_PANEL_H


#include "StageState.h"

class Panel {

    StageState* stage;

public:

    Panel(StageState *stateState);

    void update(float dt);

    void render();

};


#endif //IDJ201601T1_PANEL_H
