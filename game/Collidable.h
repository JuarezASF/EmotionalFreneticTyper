//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLE_H
#define IDJ201601T1_COLLIDABLE_H

#include <string>

using namespace std;

class Collidable {

public:
    virtual bool is(string className) = 0;

    virtual void render() = 0;

    virtual void applyScale(float factor) = 0;

};


#endif //IDJ201601T1_COLLIDABLE_H
