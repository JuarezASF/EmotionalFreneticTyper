//
// Created by jasf on 5/25/16.
//

#include "ForceField.h"

int ForceField::GRAVITY_ACCELERATION = 20;
Vec2 ForceField::getForceAt(Vec2 pos) {
    return Vec2(0, GRAVITY_ACCELERATION);
}

ForceField *ForceField::getInstance() {
    static ForceField *instance = new ForceField();

    return instance;
}

ForceField::ForceField() {

}
