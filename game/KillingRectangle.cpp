//
// Created by jasf on 5/25/16.
//

#include "KillingRectangle.h"
#include "AxisAlignedBoundingBox.h"
#include "Game.h"
#include "StageState.h"


void KillingRectangle::update(float dt) {
    CollidableAABBGameObject::update(dt);

    centerPos += dt * constSpeed;

    ((AxisAlignedBoundingBox *) collisionVolume)->setLeftTopCorner(centerPos + center_LT_displacement);


}



bool KillingRectangle::is(std::string type) {
    return CollidableAABBGameObject::is(type) || type == "KillingRectangle";
}


KillingRectangle *KillingRectangle::getTopLeftAt(Vec2 topLeft, int width, int heigth, Vec2 speed) {
    return new KillingRectangle(topLeft, topLeft + Vec2(width, heigth), speed);
}

KillingRectangle::KillingRectangle(Vec2 topLeft, Vec2 bottomRight, Vec2 speed)
        : CollidableAABBGameObject(topLeft, bottomRight) {
    constSpeed = speed;
    ((AxisAlignedBoundingBox *)collisionVolume)->setShouldFill(true);
    ((AxisAlignedBoundingBox *)collisionVolume)->setColor(0, 0, 255, 255);


}

KillingRectangle *KillingRectangle::getCenteredAt(Vec2 center, int width, int height, Vec2 speed) {
    return new KillingRectangle(center + (-0.5) * Vec2(width, height), center + (0.5) * Vec2(width, height), speed);
}

void KillingRectangle::render() {
    CollidableAABBGameObject::render();
}
