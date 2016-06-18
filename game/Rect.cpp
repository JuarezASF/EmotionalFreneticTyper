//
// Created by jasf on 3/23/16.
//

#include "Rect.h"
#include <algorithm>
#include <cmath>



Rect::Rect(float x, float y, float h, float w) : x(x), y(y), h(h), w(w), rotation(0) {
    construct();

}

Rect::Rect() : x(-1), y(-1), h(-1), w(-1), rotation(0) {
    construct();

}

Vec2 Rect::getTopLeft() {
    Vec2 c = getCenter();
    return Vec2(x, y).rotatedAroundCenter(getCenter(), rotation);
}

std::vector<Vec2> Rect::getCorners() {
    if (hasBeenUpdated) {
        corners.clear();
        hasBeenUpdated = false;
    }
    if (corners.empty()) {
        auto c = getCenter();
        //TL
        corners.push_back((c + Vec2(-0.5 * w, -0.5 * h)).rotatedAroundCenter(c, rotation));
        //BL
        corners.push_back((c + Vec2(-0.5 * w, +0.5 * h)).rotatedAroundCenter(c, rotation));
        //BR
        corners.push_back((c + Vec2(+0.5 * w, +0.5 * h)).rotatedAroundCenter(c, rotation));
        //TR
        corners.push_back((c + Vec2(+0.5 * w, -0.5 * h)).rotatedAroundCenter(c, rotation));

    }
    return corners;
}

Vec2 Rect::getCenter() const {
    return Vec2(x + w / 2.0, y + h / 2.0);
}

double Rect::distanceFromRectCenters(const Rect &A, const Rect &B) {
    return Vec2::distanceBetweenPoints(A.getCenter(), B.getCenter());
}

bool Rect::isPointInside(const Vec2 &p) {
    auto vertices = getCorners();
    auto LT = vertices[0];
    auto RB = vertices[2];

    float xmin = std::min(LT.x, RB.x);
    float xmax = std::max(LT.x, RB.x);

    float ymin = std::min(LT.y, RB.y);
    float ymax = std::max(LT.y, RB.y);

    // works for axis aligned rectangle only
    return !(p.x < xmin || p.x > xmax || p.y < ymin || p.y > ymax);

}

Rect::Rect(Vec2 LT, float h, float w) : x(LT.x), y(LT.y), h(h), w(w), rotation(0) {
    construct();

}

bool Rect::isPointInside(const float &x, const float &y) {
    return isPointInside(Vec2(x, y));
}

void Rect::construct() {

    hasBeenUpdated = true;

}

void Rect::setLT(Vec2 lt) {

    x = lt.x;
    y = lt.y;
    hasBeenUpdated = true;

}

void Rect::setRotation(float rot) {
    rotation = rot;
    hasBeenUpdated = true;
}

void Rect::setCenter(Vec2 c) {
    setLT(c + (-0.5) * Vec2(w, h));

}

Vec2 Rect::getClosestPointTo(const Vec2 &testPoint) {
    float x, y;

    auto corners = getCorners();

    auto topLeft = corners[0];

    auto bottomRight = corners[2];

    float minX = std::min(topLeft.x, bottomRight.x);
    float maxX = std::max(topLeft.x, bottomRight.x);

    x = std::max(testPoint.x, minX);
    x = std::min(x, maxX);

    float minY = std::min(topLeft.y, bottomRight.y);
    float maxY = std::max(topLeft.y, bottomRight.y);

    y = std::max(testPoint.y, minY);
    y = std::min(y, maxY);

    return Vec2(x, y);
}
