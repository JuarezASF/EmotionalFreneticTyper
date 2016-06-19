//
// Created by jasf on 5/24/16.
//

#include "CollidableAABBGameObject.h"
#include "AxisAlignedBoundingBox.h"

void CollidableAABBGameObject::construct(Vec2 leftTop, Vec2 bottomRight) {
    this->centerPos = (leftTop + bottomRight) * 0.5;

    Vec2 size = bottomRight - leftTop;
    size.x = abs(size.x);
    size.y = abs(size.y);

    collisionVolume = new AxisAlignedBoundingBox(leftTop, (int) size.x, (int) size.y);

    rotation = 0;

    center_LT_displacement = (-0.5)*Vec2(size.x, size.y);

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

VictoryRectangle::VictoryRectangle(Vec2 leftTop, Vec2 bottomRight):CollidableAABBGameObject(leftTop, bottomRight)
{

}
