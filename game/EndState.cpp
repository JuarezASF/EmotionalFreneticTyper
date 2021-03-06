//
// Created by jasf on 5/22/16.
//

#include "Game.h"
#include "StageState.h"
#include "EndState.h"
#include "TyperInput.h"

EndState::EndState(StateData stateData)
        : winMusic("audio/game_over.ogg"),loseMusic("audio/game_over.ogg"),timer(),
          instruction("font/goodfoot.ttf", 40, Text::TextStyle::BLENDED, "Type again to restart and quit to leave",
                      WHITE, 0, 0) {

	Music::halt();
    if (stateData.playerVictory) {
        bg.open("img/win.png");
        winMusic.play(0);
    }else{
        bg.open("img/loose.png");
        loseMusic.play(0);
    }

    instruction.setPos(300, 500, false, false);

    show = true;
    instruction.setPos((int) Game::getInstance().getScreenDimensions().x / 2, (int) Game::getInstance().getScreenDimensions().y / 2 + 200,
                       true, true);


}

void EndState::update(float dt) {
    static TyperInput &im = TyperInput::getInstance();

    quitRequested = im.getQuitRequested();

    if (im.hasTypintEvent()) {
        switch (im.getTypingEvent()) {
            case TyperInput::TypingEvent::CONTINUE:
                popRequested = true;
                Game::getInstance().push(new StageState());
                break;
            case TyperInput::TypingEvent::QUIT:
                quitRequested = true;
                break;

            default:
                break;

        }

    }

    timer.update(dt);

    if (timer.get() > 0.8) {
        show = !show;
        timer.restart();

    }


}

void EndState::render() {

    bg.render(0, 0, 0, (SDL_FLIP_NONE));

    if (show) {
        instruction.render();

    }
}

void EndState::pause() {

}

void EndState::resume() {

}
