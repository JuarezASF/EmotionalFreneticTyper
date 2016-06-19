//
// Created by jasf on 3/16/16.
//

#include <fstream>
#include "Game.h"
#include "Util.h"
#include "InputManager.h"
#include "Camera.h"
#include "Collision.h"
#include "StageState.h"
#include "PlayableEmotion.h"
#include "TyperInput.h"
#include "CollidableAABBGameObject.h"
#include "KillingRectangle.h"
#include "defines.h"

#define TILE_HEIGHT 170
#define TILE_WIDTH 170

StageState::StageState() : bg("img/ParedesPreto.png"),
                           tileSet(TILE_WIDTH, TILE_HEIGHT, "img/TileBrick.png"),
                           tileMap("map/tileMap.txt", &tileSet),
                           stagePanel(250, 250),
                           startText("font/Call me maybe.ttf", 70, Text::TextStyle::BLENDED, "TYPE START", DARK_YELLOW),
                           startTextTimer() {

    usedEmotion = "ALL_EMOTIONS_ARE_THE_SAME";

    // load stage configuration
    string filename = "txt/stage1config.txt";
    ifstream is(filename);

    currentState = INITIAL_PAUSED_STATE;

    if (!is.is_open()) {
        cerr << "could not load file:" << filename << endl;
    }
    else {
        cout << "loading stage 1 from" << filename << endl;
    }

    //used to read the file, parameter by parameter
    string buffer;
    int x, y, w, h, q, vx, vy;

    //skip description
    is >> buffer >> x >> y;
    cout << "main player at tl:" << Vec2(x, y) << " w,h" << Vec2(w, h) << " speed:" << Vec2(vx, vy) << endl;
    mainPlayerPtr = new PlayableEmotion(x, y);

    is >> buffer >> x >> y >> w >> h >> vx >> vy;

    cout << "killing rectangle tl:" << Vec2(x, y) << " w,h" << Vec2(w, h) << " speed:" << Vec2(vx, vy) << endl;
    addObject(KillingRectangle::getTopLeftAt(Vec2(x, y), w, h, Vec2(vx, vy)));

    //skip description
    is >> buffer >> q;

    cout << "reading: " << q << " simple rectangles" << endl;

    for (int k = 0; k < q; k++) {
        is >> x >> y >> w >> h;
        cout << "adding rectanble at " << Vec2(x, y) << " w,h:" << Vec2(w, h) << endl;

        addObject(CollidableAABBGameObject::getTopLeftAt(Vec2(x, y), w, h));

        cout.flush();

    }


    startText.setPos(Game::getInstance().getScreenDimensions().x / 2, Game::getInstance().getScreenDimensions().y / 2,
                     true, true);

    showText = false;


}

StageState::~StageState() {
    objectArray.clear();
    if (mainPlayerPtr)
        delete mainPlayerPtr;
}

Sprite StageState::getBg() {
    return bg;
}


void StageState::update(float dt) {
    static TyperInput &im = TyperInput::getInstance();
    static vector<pair<unsigned, unsigned>> collidingPairs = vector<pair<unsigned, unsigned>>();
    static std::vector<int> toBeDeleted;

    if (im.getQuitRequested()) {
        quitRequested = true;
    }

    Camera::update(dt);

    switch (currentState) {
        case GameState::INITIAL_PAUSED_STATE: {
            //check if we need to switch state of game
            if (im.hasTypintEvent() && im.peakTypingEvent() == TyperInput::TypingEvent::START) {
                im.getTypingEvent();//effectively consume event
                startTextTimer.restart();

                addObject(mainPlayerPtr);
                Camera::follow(mainPlayerPtr);
                showText = false;
                currentState = PLAYING;
            }

            startTextTimer.update(dt);
            if (startTextTimer.get() > 0.7) {
                showText = !showText;
                startTextTimer.restart();
            }

        }
            break;
        case GameState::PLAYING: {
            //check if we need to switch state of game
            if (im.hasTypintEvent() && im.peakTypingEvent() == TyperInput::TypingEvent::PAUSE) {
                im.getTypingEvent();//effectively consume event
                startTextTimer.restart();
                showText = true;

                currentState = PAUSED;
            }

            collidingPairs.clear();
            collidingPairs = checkForCollision();

            updateArray(dt);

            for (pair<unsigned, unsigned> it : collidingPairs) {
                objectArray[it.first]->notifyCollision(*objectArray[it.second]);
                objectArray[it.second]->notifyCollision(*objectArray[it.first]);
            }

            toBeDeleted.clear();
            //collect index to all objects that need to be deleted
            for (unsigned int i = 0; i < objectArray.size(); i++)
                if (objectArray[i]->isDead())
                    toBeDeleted.push_back(i);
            //delete them
            std::vector<std::unique_ptr<GameObject>>::iterator objArrayBegin = objectArray.begin();
            for (auto it = toBeDeleted.rbegin(); it != toBeDeleted.rend(); ++it)
                objectArray.erase(objArrayBegin + *it);


        }
            break;

        case GameState::PAUSED: {
            if (im.hasTypintEvent() && im.peakTypingEvent() == TyperInput::TypingEvent::START) {
                im.getTypingEvent();//effectively consume event
                startTextTimer.restart();
                showText = false;

                Camera::follow(mainPlayerPtr);

                currentState = PLAYING;
            }

            startTextTimer.update(dt);
            if (startTextTimer.get() > 0.7) {
                showText = !showText;
                startTextTimer.restart();
            }

        }
            break;

        default:
            break;

    }


    stagePanel.update(dt);
}

vector<pair<unsigned, unsigned>> StageState::checkForCollision() const {//test for collision
    vector<pair<unsigned, unsigned>> collidingPairs;

    for (long i = objectArray.size() - 1; i >= 0; i--) {
        if (objectArray[i]->is("Animation"))
            continue;
        for (int j = 0; j < i; j++) {
            if (objectArray[j]->is("Animation"))
                continue;
            if (Collision::IsColliding(objectArray[i]->getCollisionVolume(), objectArray[j]->getCollisionVolume()
            )) {
                collidingPairs.push_back(make_pair(i, j));

            }

        }

    }

    return collidingPairs;
}

void StageState::render() {
    bg.render(0, 0, 0, (SDL_FLIP_NONE));

    tileMap.renderLayer(0, Camera::getPos(Camera::PLAYER_GROUND_VIEW));

    renderArray();

    stagePanel.render();

    if (showText)
        startText.render();
}

void StageState::pause() {

}

void StageState::resume() {

}

//StageState::StateInfo StageState::getStateInfo() {
//    return StageState::StateInfo();
//}

Panel &StageState::getPanel() { return stagePanel; }
