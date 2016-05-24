//
// Created by jasf on 5/24/16.
//

#include "SupportRectangle.h"
#include "CollidableBox.h"

void SupportRectangle::construct(Vec2 leftTop, Vec2 bottomRight) {
    this->pos = (leftTop + bottomRight) * 0.5;

    Vec2 size = bottomRight - leftTop;
    size.x = abs(size.x);
    size.y = abs(size.y);

    collisionVolume = new CollidableBox(leftTop, size.y, size.x);
}

SupportRectangle::SupportRectangle(Vec2 leftTop, Vec2 bottomRight) {
    construct(leftTop, bottomRight);
}

SupportRectangle::SupportRectangle(Vec2 leftTop, int width, int height) {
    construct(leftTop, leftTop + Vec2(width, height));

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
    GameObject::render(true);
}

