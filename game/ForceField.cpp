//
// Created by jasf on 5/25/16.
//

#include "ForceField.h"

Vec2 ForceField::getForceAt(Vec2 pos) {
    return Vec2(0, 8);
}

ForceField *ForceField::getInstance() {
    static ForceField *instance = new ForceField();

    return instance;
}

ForceField::ForceField() {

}
