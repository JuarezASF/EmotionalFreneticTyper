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
    float leftTopX, leftTopY,h,w;

    Rect();

    Rect(float leftTopX, float leftTopY, float h, float w);

    Rect(Vec2 LT, float h, float w);

    Vec2 getTopLeft();

    /**
     * Order of corners is: TL, BL, BR, TR
     * Note that this order is based on rotation 0. If they are rotated, the first will no be the actual TL.
     */
    std::vector<Vec2> getCorners();

    Vec2 getCenter() const;

    static double distanceFromRectCenters(const Rect & A, const Rect & B);

    bool isPointInside(const Vec2 & p) ;
    bool isPointInside(const float& x, const float& y) ;

    void setLT(Vec2 lt);

    void setCenter(Vec2 c);

    void setRotation(float rot);

    Vec2 getClosestPointTo(const Vec2 &testPoint);

    void applyScale(float factor);

};


#endif //IDJ201601T1_RECT_H
