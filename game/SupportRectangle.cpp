//
// Created by jasf on 5/24/16.
//

#include "SupportRectangle.h"
#include "CollidableBox.h"

void SupportRectangle::construct(Vec2 leftTop, Vec2 bottomRight, float r) {
    this->pos = (leftTop + bottomRight) * 0.5;

    Vec2 size = bottomRight - leftTop;
    size.x = abs(size.x);
    size.y = abs(size.y);

    collisionVolume = new CollidableBox(leftTop, size.x, size.y, r);

    rotation = r;
}

SupportRectangle::SupportRectangle(Vec2 leftTop, Vec2 bottomRight) {
    construct(leftTop, bottomRight);
}

SupportRectangle::SupportRectangle(Vec2 leftTop, int width, int height, float r) {
    construct(leftTop, leftTop + Vec2(width, height), r);

}

void SupportRectangle::update(float dt) {

}

bool SupportRectangle::isDead() {
    return false;
}

void SupportRectangle::notifyCollision(GameObject &other) {

}

bool SupportRectangle::is(std::string type) {
    return type == "SupportRectangle";
}

void SupportRectangle::render() {
    GameObject::render();
}

void SupportRectangle::clearCollisionState() {

}

SupportRectangle::SupportRectangle() {
    construct(Vec2(0,0), Vec2(1,1));

}
