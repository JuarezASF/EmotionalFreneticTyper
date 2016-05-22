//
// Created by jasf on 3/23/16.
//

#ifndef IDJ201601T1_RECT_H
#define IDJ201601T1_RECT_H

#include "Vec2.h"
#include <vector>
#include <cmath>
#include <algorithm>

class Rect {
private:
    bool hasBeenUpdated;

    std::vector<Vec2> corners;

    float rotation;

    void construct();

public:
    float x,y,h,w;

    Rect();

    Rect(float x, float y, float h, float w);

    Rect(Vec2 LT, float h, float w);

    Vec2 getTopLeft();

    std::vector<Vec2> getCorners();

    Vec2 getCenter() const;

    static double distanceFromRectCenters(const Rect & A, const Rect & B);

    bool isPointInside(const Vec2 & p) ;
    bool isPointInside(const float& x, const float& y) ;

    void setLT(Vec2 lt);

    void setRotation(float rot);

};


#endif //IDJ201601T1_RECT_H
