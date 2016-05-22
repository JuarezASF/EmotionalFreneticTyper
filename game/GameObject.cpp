//
// Created by jasf on 3/23/16.
//

#include <SDL_render.h>
#include "GameObject.h"
#include "Game.h"
#include "GameConfig.h"
#include "Camera.h"

GameObject::GameObject() {
    constuct();

}

GameObject::~GameObject() {

}

void GameObject::constuct() {

    rotation = 0.0;

}

void GameObject::render() {
    if (GameConfig::printHitBox) {
        auto corners = box.getCorners();
        for (uint i = 0; i < corners.size(); i++) {
            Vec2 a = corners[i] - Camera::getPos(Camera::PLAYER_GROUND_VIEW);
            Vec2 b = corners[(i + 1) % corners.size()] - Camera::getPos(Camera::PLAYER_GROUND_VIEW);
            SDL_RenderDrawLine(Game::getInstance().getRenderer(), a.x, a.y, b.x, b.y);

        }


    }
}
