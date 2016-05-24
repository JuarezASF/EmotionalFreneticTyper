//
// Created by jasf on 5/24/16.
//

#include "SupportLineSegment.h"

SupportLineSegment::SupportLineSegment(Vec2 A, Vec2 B) : pointA(A), pointB(B){

}

Vec2 SupportLineSegment::getCenter() {
    return (pointA + pointB)*0.5;
}

void SupportLineSegment::rotate(float theta) {
    Vec2 c = getCenter();

    float lentgh = (pointB - c).magnitude();
    float currentAngle = (pointB - c).ang_rad();

    currentAngle += theta;

    pointB = c + Vec2::getVec2FromPolar(lentgh, currentAngle);
    pointA = c + Vec2::getVec2FromPolar(lentgh, currentAngle + M_PI);



}

bool SupportLineSegment::is(string className) {
    return className == "SupportLineSegment";
}
