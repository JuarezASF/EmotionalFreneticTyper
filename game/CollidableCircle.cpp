//
// Created by jasf on 6/2/16.
//

#include <SDL2/SDL2_gfxPrimitives.h>
#include "CollidableCircle.h"
#include "Game.h"
#include "Camera.h"

bool CollidableCircle::is(string className) {
    return className == "CollidableCircle";
}

void CollidableCircle::render() {

    Vec2 c = center - Camera::getPos(Camera::PLAYER_GROUND_VIEW);

    circleRGBA(Game::getInstance().getRenderer(), (int)c.x, (int)c.y, (int) radius, 0x00, 0xFF, 0x00, 0xFF);

}

void CollidableCircle::setCenter(Vec2 p) {
    center = p;

}

void CollidableCircle::applyScale(float factor) {
    radius *= factor;
}
