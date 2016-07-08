//
// Created by jasf on 3/16/16.
//

#include <fstream>
#include "Game.h"
#include "Util.h"
#include "Camera.h"
#include "Collision.h"
#include "StageState.h"
#include "PlayableEmotion.h"
#include "TyperInput.h"
#include "defines.h"

#define TILE_HEIGHT 356
#define TILE_WIDTH 680

StageState::StageState() : tileSet(TILE_WIDTH, TILE_HEIGHT, "img/tileSet.png"),
                           tileMap("map/tileMap.txt", &tileSet),
                           stagePanel(340, 340),
                           startText("font/goodfoot.ttf", 70, Text::TextStyle::BLENDED, "TYPE START", WHITE),
                           startTextTimer(),
                           lightEffetct("img/vignetting.png") {

    SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 0, 0, 0, 255);

    plataformasManager = PlataformasManager::getInstance();

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
    cout << "main player at tl:" << Vec2(x, y) << endl;
    mainPlayerPtr = new PlayableEmotion(x, y);
    int playerY0 = y;

    is >> buffer >> x >> y >> w >> h >> vx >> vy;

    cout << "killing rectangle tl:" << Vec2(x, y) << " w,h" << Vec2(w, h) << " speed:" << Vec2(vx, vy) << endl;
    KillingRectangle *smokeObject = KillingRectangle::getTopLeftAt(Vec2(x, y), "img/darknessFinal.png",
                                                                   Vec2(vx, vy), 10, 3);
    addObject(smokeObject);

    trackThis = new TrackerObject(smokeObject, Vec2(0, -1 * Game::getInstance().getScreenDimensions().y / 2 + 75));
    addObject(trackThis);
    //read left and right limits
    is >> buffer >> x;
    int leftLimit = x;
    is >> buffer >> x;
    int rightLimit = x;

    //add block to enforce limits
    addObject(CollidableAABBGameObject::getCenteredAt(Vec2(leftLimit - stagePanel.GetLeftWidth() - 18, playerY0), 20, 6000));
    addObject(CollidableAABBGameObject::getCenteredAt(Vec2(rightLimit - stagePanel.GetLeftWidth() + 10, playerY0), 20, 6000));


    //read qtd of blocks on stage
    is >> buffer >> q;

    cout << "reading: " << q << " simple rectangles" << endl;
    string filenameForeground, filenameBackground, platformName;

    for (int k = 0; k < q; k++) {
        is >> buffer;

        if (buffer.compare("plataforma") == 0) {
            is >> x >> y >> platformName;
            cout << "adding platform of type:" << platformName << " at " << Vec2(x, y) << endl;

            auto plataforma = plataformasManager->getPlatformCenteredAt(Vec2(x, y), platformName);
            if (plataforma != nullptr)
                addObject(plataforma);


        }
        else if (buffer.compare("block") == 0) {
            is >> x >> y >> w >> h;
            cout << "adding rectanble of type:" << buffer << " at " << Vec2(x, y) << " w,h:" << Vec2(w, h) << endl;
            addObject(CollidableAABBGameObject::getTopLeftAt(Vec2(x, y), w, h));
        } else if (buffer.compare("smashable") == 0) {
            is >> x >> y >> w >> h;
            cout << "adding rectanble of type:" << buffer << " at " << Vec2(x, y) << " w,h:" << Vec2(w, h) << endl;
            addObject(DestroyableRectangle::getTopLeftAt(Vec2(x, y), w, h));
        } else if (buffer.compare("victory") == 0) {
            is >> x >> y >> w >> h;
            cout << "adding rectanble of type:" << buffer << " at " << Vec2(x, y) << " w,h:" << Vec2(w, h) << endl;
            addObject(VictoryRectangle::getTopLeftAt(Vec2(x, y), w, h));
        } else {
            cerr << "Unreconizable type of rectangle:" << buffer << endl;
        }


        cout.flush();

    }


    startText.setPos(Game::getInstance().getScreenDimensions().x / 2, Game::getInstance().getScreenDimensions().y / 2,
                     true, true);

    showText = false;


}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::update(float dt) {
    stagePanel.update(dt);
    static TyperInput &im = TyperInput::getInstance();
    static vector<pair<unsigned, unsigned>> collidingPairs = vector<pair<unsigned, unsigned>>();
    static std::vector<int> toBeDeleted;

    quitRequested = im.getQuitRequested();

    Camera::update(dt);

    switch (currentState) {
        case GameState::INITIAL_PAUSED_STATE: {
            //check if we need to switch state of game
            if (im.hasTypintEvent() && im.getTypingEvent() == TyperInput::TypingEvent::START) {
                startTextTimer.restart();

                addObject(mainPlayerPtr);
                Camera::follow(trackThis);
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
            for (auto it = toBeDeleted.rbegin(); it != toBeDeleted.rend(); ++it) {
                objectArray.erase(objArrayBegin + *it);

            }


        }
            break;

        case GameState::PAUSED: {
            if (im.hasTypintEvent() && im.peakTypingEvent() == TyperInput::TypingEvent::START) {
                im.getTypingEvent();//effectively consume event
                startTextTimer.restart();
                showText = false;

                Camera::follow(trackThis);

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
    SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 0, 0, 0, 255);
    SDL_RenderClear(Game::getInstance().getRenderer());

    tileMap.renderLayer(Camera::BACKGROUND_VIEW, Camera::getPos(Camera::PLAYER_GROUND_VIEW));

    renderArray();

//    lightEffetct.render(342, 0, 0);


    stagePanel.render();

    if (showText)
        startText.render();
}

void StageState::pause() {

}

void StageState::resume() {

}

Panel &StageState::getPanel() { return stagePanel; }
