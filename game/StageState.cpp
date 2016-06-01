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
                           tileMap("map/tileMap.txt", &tileSet) {

    GameObject *mainPlayer = new PlayableEmotion(200, 300);

    addObject(mainPlayer);
    addObject(new SupportRectangle(Vec2(0, 500), 800, 80 , 0));
    addObject(new KillingRectangle(Vec2(0, 500), 80, 80, Vec2(0, -20)));
    addObject(new VictoryRectangle(Vec2(500, 400), 80, 80, 0));


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
            if (Collision::IsColliding(objectArray[i]->getCollisionVolume(), objectArray[j]->getCollisionVolume()
            )) {
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
    bg.render(0, 0, 0, (SDL_FLIP_NONE));

    tileMap.renderLayer(0, Camera::getPos(Camera::PLAYER_GROUND_VIEW));

    renderArray();


}

void StageState::pause() {

}

void StageState::resume() {

}

StageState::StateInfo StageState::getStateInfo() {
    return StageState::StateInfo();
}
