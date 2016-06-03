//
// Created by jasf on 3/16/16.
//

#include "Game.h"
#include "Util.h"
#include "InputManager.h"
#include "Camera.h"
#include "Collision.h"
#include "StageState.h"
#include "PlayableEmotion.h"
#include "TyperInput.h"
#include "SupportRectangle.h"
#include "KillingRectangle.h"

#define TILE_HEIGHT 170
#define TILE_WIDTH 170

StageState::StageState() : bg("img/ParedesPreto.png"), tileSet(TILE_WIDTH, TILE_HEIGHT, "img/TileBrick.png"),
                           tileMap("map/tileMap.txt", &tileSet), stagePanel(250, 250), usedEmotion() {

    GameObject *mainPlayer = new PlayableEmotion(300, 200);

    addObject(mainPlayer);
    addObject(new SupportRectangle(Vec2(300, 350), 200, 80 , 0));
    addObject(new SupportRectangle(Vec2(210, 280), 40, 40 , 0));

    addObject(new SupportRectangle(Vec2(570, 500), 200, 80 , 0));
    addObject(new SupportRectangle(Vec2(680, 420), 40, 40 , 0));

    addObject(new KillingRectangle(Vec2(0, 500), 80, 80, Vec2(0, -20)));



}

StageState::~StageState() {
    objectArray.clear();

}

Sprite StageState::getBg() {
    return bg;
}


void StageState::update(float dt) {
    TyperInput &im = TyperInput::getInstance();

    if (im.getQuitRequested())
    	quitRequested = true;
        //popRequested = true;

    Camera::update(dt);

    // unless told the opposite, no one is supported
    for(uint k = 0; k < objectArray.size(); k++)
        objectArray[k]->clearSupported();

    auto collidingPairs = checkForCollision();

    updateArray(dt);

    for(auto it : collidingPairs){
        objectArray[it.first]->notifyCollision(*objectArray[it.second]);
        objectArray[it.second]->notifyCollision(*objectArray[it.first]);
    }



    std::vector<int> toBeDeleted;
    //collect index to all objects that need to be deleted
    for (unsigned int i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->isDead())
            toBeDeleted.push_back(i);
    //delete them
    auto objArrayBegin = objectArray.begin();
    for (auto it = toBeDeleted.rbegin(); it != toBeDeleted.rend(); ++it)
        objectArray.erase(objArrayBegin + *it);

    stagePanel.update(dt);
}

typedef unsigned long ulong;

vector<pair<uint, uint>> StageState::checkForCollision() const {//test for collision
    vector<pair<uint, uint>> collidingPairs;

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
}

void StageState::pause() {

}

void StageState::resume() {

}

//StageState::StateInfo StageState::getStateInfo() {
//    return StageState::StateInfo();
//}

Panel& StageState::getPanel() { return stagePanel; }
