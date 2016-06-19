//
// Created by jasf on 5/22/16.
//

#include "Game.h"
#include "StageState.h"
#include "EndState.h"
#include "defines.h"

EndState::EndState(StateData stateData)  {

    if (stateData.playerVictory){
        bg.open("img/win.png");
    }

    instruction.setPos(300, 500, false, false);


}

void EndState::update(float dt) {


}

void EndState::render() {

    bg.render(0, 0, 0, (SDL_FLIP_NONE));

//    instruction.render();
}

void EndState::pause() {

}

void EndState::resume() {

}
