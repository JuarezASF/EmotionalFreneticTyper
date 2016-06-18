//
// Created by jasf on 5/25/16.
//

#include "ForceField.h"
#include "SettingsLoad.h"

int ForceField::GRAVITY_ACCELERATION;
Vec2 ForceField::getForceAt(Vec2 pos) {
    return Vec2(0, GRAVITY_ACCELERATION);
}

ForceField *ForceField::getInstance() {
    static ForceField *instance = new ForceField();

    return instance;
}

ForceField::ForceField() {

    SettingsLoad *settings = SettingsLoad::getInstance();
    GRAVITY_ACCELERATION = (int) settings->get("GRAVITY", 20);

}

void ForceField::setGravity(int gravityConstant) {

    GRAVITY_ACCELERATION = gravityConstant;

}
