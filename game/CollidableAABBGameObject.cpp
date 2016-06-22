//
// Created by jasf on 5/24/16.
//

#include "CollidableAABBGameObject.h"
#include "AxisAlignedBoundingBox.h"
#include "Game.h"
#include "Animation.h"
#include "Camera.h"

void CollidableAABBGameObject::construct(Vec2 leftTop, Vec2 bottomRight) {
    this->centerPos = (leftTop + bottomRight) * 0.5;

    Vec2 size = bottomRight - leftTop;
    size.x = abs(size.x);
    size.y = abs(size.y);

    collisionVolume = new AxisAlignedBoundingBox(leftTop, (int) size.x, (int) size.y);

    rotation = 0;

    center_LT_displacement = (-0.5) * Vec2(size.x, size.y);

}

CollidableAABBGameObject::CollidableAABBGameObject() {
    construct(Vec2(0, 0), Vec2(1, 1));
}

CollidableAABBGameObject::CollidableAABBGameObject(Vec2 leftTop, Vec2 bottomRight) {
    construct(leftTop, bottomRight);
}


void CollidableAABBGameObject::update(float dt) {

}

bool CollidableAABBGameObject::isDead() {
    return false;
}

void CollidableAABBGameObject::notifyCollision(GameObject &other) {

}

bool CollidableAABBGameObject::is(std::string type) {
    return type == "CollidableAABBGameObject";
}

void CollidableAABBGameObject::render() {
    GameObject::render();
}


bool VictoryRectangle::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "VictoryRectangle";
}

void VictoryRectangle::render() {
    CollidableAABBGameObject::render();
}

CollidableAABBGameObject *CollidableAABBGameObject::getCenteredAt(Vec2 center, int w, int h) {
    return new CollidableAABBGameObject(center + Vec2(w, h) * (-0.5), center + Vec2(w, h) * (0.5));
}

CollidableAABBGameObject *CollidableAABBGameObject::getTopLeftAt(Vec2 topLeft, int w, int h) {
    return new CollidableAABBGameObject(topLeft, topLeft + Vec2(w, h));
}

VictoryRectangle::VictoryRectangle(Vec2 leftTop, Vec2 bottomRight) : CollidableAABBGameObject(leftTop, bottomRight) {
    ((AxisAlignedBoundingBox *) collisionVolume)->setColor(255, 255, 255, 255);

}

void KillingRectangle::update(float dt) {
    CollidableAABBGameObject::update(dt);

    centerPos += dt * constSpeed;

    ((AxisAlignedBoundingBox *) collisionVolume)->setLeftTopCorner(centerPos + center_LT_displacement);


}


bool KillingRectangle::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "KillingRectangle";
}

KillingRectangle::KillingRectangle(Vec2 topLeft, std::string smokeBorderFileName, std::string smokeBodyFileName,
                                   Vec2 speed)
        : borderSp(smokeBorderFileName),
          bodySp(smokeBodyFileName) {

    const int qtdFramesOnBorderSp = 1;
    const int qtdFramesOnBodySp = 1;

    int borderWidth = borderSp.getWidth()/qtdFramesOnBorderSp;
    int borderHeight = borderSp.getHeight();

    borderDimensions = Vec2(borderWidth, borderHeight);

    int bodyWidth = bodySp.getWidth()/qtdFramesOnBodySp;
    int bodyHeight = bodySp.getHeight();

    if(bodyWidth != borderWidth){
        cerr << "Smoke body and border differ in width!" << endl;
    }

    int width = max(bodyWidth, borderWidth);
    int height = bodyHeight + borderHeight;

    constSpeed = speed;

    construct(topLeft, topLeft + Vec2(width, height));
    ((AxisAlignedBoundingBox *) collisionVolume)->setShouldFill(false);
    ((AxisAlignedBoundingBox *) collisionVolume)->setColor(0, 0, 255, 255);

}


KillingRectangle *KillingRectangle::getTopLeftAt(Vec2 topLeft, std::string smokeBorderFN,
                                                std::string smokeBodyFN, Vec2 speed) {
    return new KillingRectangle(topLeft, smokeBorderFN, smokeBodyFN, speed);

}



void KillingRectangle::render() {
    CollidableAABBGameObject::render();

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(1);
    borderSp.render(pos.x, pos.y, 0);
    bodySp.render(pos.x, pos.y + borderDimensions.y, 0);
}

bool DestroyableRectangle::isDead() {
    return !this->alive;
}

DestroyableRectangle *DestroyableRectangle::getTopLeftAt(Vec2 topLeft, int width, int heigth) {
    return new DestroyableRectangle(topLeft, topLeft + Vec2(width, heigth));
}

DestroyableRectangle *DestroyableRectangle::getCenteredAt(Vec2 center, int width, int height) {
    return new DestroyableRectangle(center + (-0.5) * Vec2(width, height), center + (0.5) * Vec2(width, height));
}

void DestroyableRectangle::update(float dt) {
    CollidableAABBGameObject::update(dt);
}

bool DestroyableRectangle::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "DestroyableRectangle";
}

void DestroyableRectangle::render() {
    CollidableAABBGameObject::render();
}

DestroyableRectangle::DestroyableRectangle(Vec2 topLeft, Vec2 bottomRight) :
        CollidableAABBGameObject(topLeft, bottomRight) {

    alive = true;

    ((AxisAlignedBoundingBox *) collisionVolume)->setColor(0, 255, 0, 255);

}

void DestroyableRectangle::smashThis() {
    alive = false;

    //add explosion when destroying block
    static float step = 0.33;
    static int frameCount = 4;
    static int repeat = 3;

    Game::getInstance().getCurrentState().addObject(
            new Animation(centerPos.x, centerPos.y, 0, "img/sprite_block_being_destroyed.png",
                          repeat * step * frameCount, true, frameCount, step)
    );

}

VictoryRectangle *VictoryRectangle::getTopLeftAt(Vec2 topLeft, int w, int h) {
    return new VictoryRectangle(topLeft, topLeft + Vec2(w, h));
}

TrackerObject::TrackerObject(GameObject *target, Vec2 offset) : GameObject(), target(target), offset(offset) {
    skipCollision = true;
    centerPos = target->getCenterPos() + offset;
}

void TrackerObject::update(float dt) {

    centerPos = target->getCenterPos() + offset;


}

bool TrackerObject::isDead() {
    return false;
}

void TrackerObject::notifyCollision(GameObject &other) {

}

bool TrackerObject::is(std::string type) {
    return type == "TrackerObject";
}

GrabableSquare *GrabableSquare::getTopLeftAt(Vec2 topLeft, int width) {
    return new GrabableSquare(topLeft, width);
}

void GrabableSquare::update(float dt) {
    CollidableAABBGameObject::update(dt);
}

bool GrabableSquare::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "GrabableSquare";
}

bool GrabableSquare::isDead() {
    return false;
}

void GrabableSquare::render() {
    CollidableAABBGameObject::render();
}

GrabableSquare::GrabableSquare(Vec2 topLeft, int length) : CollidableAABBGameObject(topLeft, topLeft + Vec2(length, length)) {
    //paint it yellow
    ((AxisAlignedBoundingBox *) collisionVolume)->setColor(255, 255, 0, 255);



}
