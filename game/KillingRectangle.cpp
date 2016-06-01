//
// Created by jasf on 5/25/16.
//

#include "KillingRectangle.h"
#include "CollidableBox.h"
#include "Camera.h"
#include "Game.h"


void KillingRectangle::update(float dt) {
    SupportRectangle::update(dt);

    pos += dt*constSpeed;

    ((CollidableBox *) collisionVolume)->setLT(pos + center_LT_displacement);


}

bool KillingRectangle::isDead() {
    return SupportRectangle::isDead();
}

void KillingRectangle::notifyCollision(GameObject &other) {
    SupportRectangle::notifyCollision(other);
}

bool KillingRectangle::is(std::string type) {
    return SupportRectangle::is(type) || type == "KillingRectangle";
}

void KillingRectangle::render() {
    SupportRectangle::render();
    static SDL_Rect sdl_rect;
    Rect &box = ((CollidableBox *)getCollisionVolume())->box;
    auto corners = box.getCorners();
    for (unsigned i = 0; i < corners.size(); i++) {
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 0,0,255,100);
        sdl_rect.x = box.x;
        sdl_rect.y = box.y;
        sdl_rect.w = box.w;
        sdl_rect.h = box.h;

        SDL_RenderFillRect(Game::getInstance().getRenderer(), &sdl_rect);

    }
}

KillingRectangle::KillingRectangle(Vec2 cPos, int w, int h, Vec2 speed) : SupportRectangle(cPos - Vec2(w*0.5, h*0.5), w, h),
constSpeed(speed){


}
