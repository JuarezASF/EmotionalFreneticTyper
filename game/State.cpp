//
// Created by jasf on 5/21/16.
//

#include "State.h"
#include "Camera.h"
#include "defines.h"
#include "Collision.h"

State::State() {
    quitRequested = false;
    popRequested = false;

}

State::~State() {

}

bool State::getPopRequested() {
    return popRequested;
}

bool State::getQuitRequested() {
    return quitRequested;
}

void State::updateArray(float dt) {
    for (unsigned obj_idx = 0; obj_idx < objectArray.size(); obj_idx++)
        objectArray[obj_idx]->update(dt);
}

void State::renderArray() {
    for (auto it = objectArray.begin(); it != objectArray.end(); ++it)
        if((*it)->isBackground())
            (*it)->renderBackground();

    for (auto it = objectArray.begin(); it != objectArray.end(); ++it)
        if((*it)->isPlayerGround())
            (*it)->render();

    for (auto it = objectArray.begin(); it != objectArray.end(); ++it)
        if((*it)->isForeground())
            (*it)->renderForeground();

}

void State::addObject(GameObject *obj) {
    objectArray.emplace_back(obj);
}
