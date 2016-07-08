//
// Created by jasf on 3/23/16.
//

#include <SDL_render.h>
#include "GameObject.h"
#include "Game.h"
#include "GameConfig.h"
#include "Camera.h"

GameObject::GameObject() {
    construct();

}

GameObject::~GameObject() {
    if(collisionVolume)
        delete collisionVolume;
}

void GameObject::construct() {
    centerPos = Vec2(0, 0);
    rotation = 0.0;
    center_LT_displacement = Vec2(0,0);
    collisionVolume = nullptr;
    supported = false;
    currentScaleFactor = 1.0;
    skipCollision = false;

}

void GameObject::render() {
    if (GameConfig::printHitBox && collisionVolume != nullptr) {
        collisionVolume->render();
    }
}

void GameObject::notifySupported() {
    supported = true;

}

void GameObject::clearSupported() {
    supported = false;
}

void GameObject::applyScaleFactor(float f) {
    currentScaleFactor *= f;
    collisionVolume->applyScale(f);
}

void GameObject::renderForeground() {

}

void GameObject::renderBackground() {

}

bool GameObject::isForeground() {
    return false;

}

bool GameObject::isBackground() {
    return false;

}

bool GameObject::isPlayerGround() {
    return true;
}
