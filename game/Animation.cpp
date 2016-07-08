//
// Created by jasf on 5/8/16.
//

#include "Animation.h"
#include "Camera.h"

Animation::Animation(float centerX, float centerY, float rotation, string sprite, float timeLimit, bool ends,
                     int frameCount,
                     float frameTime)
        : timeLimit(timeLimit), oneTimeOnly(ends), sp(sprite, frameCount, frameTime) {

    this->rotation = rotation;
    endTimer.restart();

    centerPos = Vec2(centerX, centerY);

    int width = sp.getSpriteFullWidth() / frameCount;
    int height = sp.getSpriteFullHeight();

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

MatrixAnimation::MatrixAnimation(float centerX, float centerY, float rotation, string sprite, float timeLimit,
                                 bool ends, int qtdRows, int qtdCols, float frameTime)
        : Animation(centerX, centerY, rotation, timeLimit, ends), matrixSprite(sprite, qtdRows, qtdCols, frameTime) {

    this->rotation = rotation;
    endTimer.restart();

    centerPos = Vec2(centerX, centerY);

    int width = matrixSprite.getFrameWidth();
    int height = matrixSprite.getFrameHeight();

    center_LT_displacement = -1 * Vec2(width * 0.5f, height * 0.5f);

    matrixSprite.setFrame(0);

}

void Animation::restart() {
    sp.setFrame(0);
    endTimer.restart();
}

void MatrixAnimation::restart() {
    matrixSprite.setFrame(0);
    endTimer.restart();
}

Animation::Animation(float centerX, float centerY, float rotation, float timeLimit, bool ends)
        : timeLimit(timeLimit), oneTimeOnly(ends) {
    this->rotation = rotation;

    endTimer.restart();

    centerPos = Vec2(centerX, centerY);

}

void MatrixAnimation::render() {
    GameObject::render();

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(Camera::PLAYER_GROUND_VIEW);

    matrixSprite.render((int) pos.x, (int) pos.y, rotation, (SDL_FLIP_NONE));
}

bool MatrixAnimation::isDead() {
    return matrixSprite.isThistLastFrame();
}

void MatrixAnimation::update(float dt) {
    matrixSprite.update(dt);
}
