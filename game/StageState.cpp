//
// Created by jasf on 3/16/16.
//

#include <Game.h>
#include "Util.h"
#include "InputManager.h"
#include "defines.h"
#include "Camera.h"
#include "Collision.h"
#include "StageState.h"
#include "EndState.h"
#include "PlaybleEmotion.h"
#include "TyperInput.h"

#define TILE_HEIGHT 64
#define TILE_WIDTH 64

StageState::StageState() : bg("img/farBackground.jpg"){

    addObject(new PlaybleEmotion());


}

StageState::~StageState() {
    objectArray.clear();

}

Sprite StageState::getBg() {
    return bg;
}


void StageState::update(float dt) {
    TyperInput &im = TyperInput::getInstance();

    if ( im.getQuitRequested())
        popRequested = true;

    Camera::update(dt);

    updateArray(dt);

    //test for collision
    for (int i = objectArray.size() - 1; i >= 0; i--) {
        if (objectArray[i]->is("Animation"))
            continue;
        for (int j = 0; j < i; j++) {
            if (objectArray[j]->is("Animation"))
                continue;
            if (Collision::IsColliding(objectArray[i]->getBox(), objectArray[j]->getBox(),
                                       objectArray[i]->getRotation(), objectArray[j]->getRotation())) {
                objectArray[i]->notifyCollision(*objectArray[j]);
                objectArray[j]->notifyCollision(*objectArray[i]);

            }

        }

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
}

void StageState::render() {
    bg.render(0, 0, 0);

    renderArray();
}

void StageState::pause() {

}

void StageState::resume() {

}
