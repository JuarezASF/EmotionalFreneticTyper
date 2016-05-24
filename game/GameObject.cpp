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
    pos = Vec2(0,0);
    rotation = 0.0;
    center_LT_displacement = Vec2(0,0);
    collisionVolume = nullptr;

}

void GameObject::render() {
    if (GameConfig::printHitBox && collisionVolume != nullptr) {
        collisionVolume->render();
    }
}
