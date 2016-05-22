//
// Created by jasf on 5/21/16.
//

#include "TitleState.h"
#include "InputManager.h"
#include "defines.h"
#include "StageState.h"
#include "Game.h"

TitleState::TitleState() : bg("img/title.jpg"),
                           infoText("font/Call me maybe.ttf", 40, Text::TextStyle::BLENDED, "Press space to start...",
                                    SDL_Color{255, 255, 0, 255}, 0, 0) {

    infoText.setPos(300, 500, false, false);
    shouldDisplayInfoText = false;
    timer.restart();

}

void TitleState::update(float d) {

    InputManager &im = InputManager::getInstance();

    if (im.keyPress(ESCAPE_KEY)) {
        quitRequested = true;

    }

    if (im.keyPress(SPACE_BAR)) {
        Game::getInstance().push(new StageState());

    }

    timer.update(d);

    if (timer.get() > 0.5){
        shouldDisplayInfoText = !shouldDisplayInfoText;
        timer.restart();

    }


}

void TitleState::render() {

    bg.render(0, 0, 0);

    if(shouldDisplayInfoText)
        infoText.render();

}

void TitleState::pause() {

}

void TitleState::resume() {

}
