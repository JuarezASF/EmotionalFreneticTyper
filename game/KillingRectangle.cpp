//
// Created by jasf on 5/25/16.
//

#include "KillingRectangle.h"
#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "Game.h"
#include "StageState.h"


void KillingRectangle::update(float dt) {
    CollidableAABBGameObject::update(dt);

    pos += dt*constSpeed;

    ((AxisAlignedBoundingBox *) collisionVolume)->setLeftTopCorner(pos + center_LT_displacement);


}

bool KillingRectangle::isDead() {
    return CollidableAABBGameObject::isDead();
}

void KillingRectangle::notifyCollision(GameObject &other) {
    CollidableAABBGameObject::notifyCollision(other);
}

bool KillingRectangle::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "KillingRectangle";
}

void KillingRectangle::render() {
    CollidableAABBGameObject::render();
    static SDL_Rect sdl_rect;
    Rect &box = ((AxisAlignedBoundingBox *)getCollisionVolume())->axisAlignedRectangle;
    auto corners = box.getCorners();
    for (unsigned i = 0; i < corners.size(); i++) {
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 0,0,255,100);
        sdl_rect.x = box.x + ((StageState&) Game::getInstance().getCurrentState()).getPanel().GetLeftWidth();
        sdl_rect.y = box.y;
        sdl_rect.w = box.w;
        sdl_rect.h = box.h;

        SDL_RenderFillRect(Game::getInstance().getRenderer(), &sdl_rect);

    }
}

