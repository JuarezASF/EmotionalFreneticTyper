//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_SUPPORTLINESEGMENT_H
#define IDJ201601T1_SUPPORTLINESEGMENT_H


#include "Vec2.h"
#include "Collidable.h"
#include <string>

using namespace std;

class SupportLineSegment : public Collidable {

private:
    Vec2 pointA, pointB;

public:
    SupportLineSegment(Vec2 A, Vec2 B);

    Vec2 getCenter();

    void rotate(float theta);

    bool is(string className);




};


#endif //IDJ201601T1_SUPPORTLINESEGMENT_H
