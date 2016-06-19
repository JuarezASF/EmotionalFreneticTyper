//
// Created by jasf on 5/24/16.
//

#include "AxisAlignedBoundingBox.h"
#include "Camera.h"
#include "Game.h"

bool AxisAlignedBoundingBox::is(string className) {
    return className == "AxisAlignedBoundingBox";
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(Vec2 topLeft, int width, int heigth) : axisAlignedRectangle(topLeft, heigth, width) {
    setColor(255, 0, 0, 255);
    shouldFill = false;


}

void AxisAlignedBoundingBox::render() {
    auto corners = axisAlignedRectangle.getCorners();
    auto center = axisAlignedRectangle.getCenter();
    const Vec2 &cameraDisplacement = Camera::getPos(Camera::PLAYER_GROUND_VIEW);
    Vec2 a, b;
    for (unsigned i = 0; i < corners.size(); i++) {
        a = corners[i] - cameraDisplacement;
        b = corners[(i + 1) % corners.size()] - cameraDisplacement;
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), colorR, colorG, colorB, colorA);
        SDL_RenderDrawLine(Game::getInstance().getRenderer(), (int)a.x, (int)a.y, (int)b.x, (int)b.y);



    }
    auto normals = getNormals();
    unsigned k = 0;
    for (auto v : normals) {
        a = center - cameraDisplacement;
        float q = 0.5f * ((((k++) % 2) == 0) ? axisAlignedRectangle.w : axisAlignedRectangle.h);
        b = center + v * q - cameraDisplacement;
        SDL_RenderDrawLine(Game::getInstance().getRenderer(), (int)a.x, (int)a.y, (int)b.x, (int)b.y);
    }

    if(shouldFill){
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), colorR, colorG, colorB, colorA);
        SDL_Rect sdl_rect;
        sdl_rect.x = (int) (axisAlignedRectangle.leftTopX - cameraDisplacement.x);
        sdl_rect.y = (int) (axisAlignedRectangle.leftTopY - cameraDisplacement.y);
        sdl_rect.w = (int)axisAlignedRectangle.w;
        sdl_rect.h = (int)axisAlignedRectangle.h;

        SDL_RenderFillRect(Game::getInstance().getRenderer(), &sdl_rect);

    }

}

void AxisAlignedBoundingBox::setLeftTopCorner(Vec2 v) {
    axisAlignedRectangle.setLT(v);
}

void AxisAlignedBoundingBox::setCenter(Vec2 v) {
    axisAlignedRectangle.setCenter(v);

}

Vec2 AxisAlignedBoundingBox::getCenter() {
    return axisAlignedRectangle.getCenter();
}


vector<Vec2> AxisAlignedBoundingBox::getNormals() {
    vector<Vec2> normals;
    auto corners = axisAlignedRectangle.getCorners();

    //compute all normals

    for (unsigned i = 0; i < corners.size(); i++) {
        normals.push_back((corners[(i + 1) % corners.size()] - corners[i]).getNormalizedVector().rotated(M_PI_2));
    }
    return normals;
}

AxisAlignedBoundingBox::CollisionAvoidanceInfo AxisAlignedBoundingBox::getInfoToAvoidCollision(Vec2 circleCenter, float circleRadius) {

    auto closestPoint = axisAlignedRectangle.getClosestPointTo(circleCenter);
    Vec2 direction = circleCenter - closestPoint;

    float distance = (float) Vec2::distanceBetweenPoints(closestPoint, circleCenter);

    float overlap = -1;

    if(distance < circleRadius){
        overlap = circleRadius - distance;
    }

    return AxisAlignedBoundingBox::CollisionAvoidanceInfo(direction, overlap);

}

void AxisAlignedBoundingBox::setColor(uchar r, uchar g, uchar b, uchar a) {
    colorR = r;
    colorG = g;
    colorB = b;
    colorA = a;

}

AxisAlignedBoundingBox AxisAlignedBoundingBox::getAABBCenteredAt(Vec2 c, int w, int h) {
    return AxisAlignedBoundingBox(c + (-0.5)*Vec2(w, h), w, h);
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::getAABBTopLeftAt(Vec2 c, int w, int h) {
    return AxisAlignedBoundingBox(c, 0, 0);
}

