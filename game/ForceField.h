//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_FORCEFIELD_H
#define IDJ201601T1_FORCEFIELD_H

#include "Vec2.h"

class ForceField {
private:

    ForceField();
    static int GRAVITY_ACCELERATION;

public:
    Vec2 getForceAt(Vec2 pos);

    static ForceField *getInstance();

};


#endif //IDJ201601T1_FORCEFIELD_H
