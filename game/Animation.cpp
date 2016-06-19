//
// Created by jasf on 5/8/16.
//

#include "Animation.h"
#include "Camera.h"

Animation::Animation(float centerX, float centerY, float rotation, string sprite, float timeLimit, bool ends, int frameCount,
                     float frameTime)
        : timeLimit(timeLimit), oneTimeOnly(ends), sp(sprite, frameCount, frameTime) {

    this->rotation = rotation;
    endTimer.restart();

    centerPos = Vec2(centerX, centerY);

    int width = sp.getWidth() / frameCount;
    int height = sp.getHeight();

    center_LT_displacement = -1 * Vec2(width * 0.5f, height * 0.5f);

}

void Animation::update(float dt) {
    endTimer.update(dt);

    sp.update(dt);
}

void Animation::render() {
    GameObject::render();

    Vec2 pos = pos + center_LT_displacement - Camera::getPos(Camera::PLAYER_GROUND_VIEW);

    sp.render((int) pos.x, (int) pos.y, rotation, (SDL_FLIP_NONE));
}

bool Animation::isDead() {

    return oneTimeOnly && (endTimer.get() > timeLimit);
}

void Animation::notifyCollision(GameObject *other) {
    //do not react to collision

}

bool Animation::is(std::string type) {
    return type == "Animation";
}

void Animation::notifyCollision(GameObject &other) {
    //do not react to collision

}

