//
// Created by jasf on 5/24/16.
//

#include "CollidableBox.h"
#include "Camera.h"
#include "Game.h"

bool CollidableBox::is(string className) {
    return className == "CollidableBox";
}

CollidableBox::CollidableBox(Vec2 topLeft, int width, int heigth, float r) : box(topLeft, heigth, width) {
    setRotation(r);


}

void CollidableBox::render() {
    auto corners = box.getCorners();
    static SDL_Rect sdl_rect;
    auto center = box.getCenter();
    const Vec2 &cameraDisplacement = Camera::getPos(Camera::PLAYER_GROUND_VIEW);
    for (uint i = 0; i < corners.size(); i++) {
        Vec2 a = corners[i] - cameraDisplacement;
        Vec2 b = corners[(i + 1) % corners.size()] - cameraDisplacement;
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 255, 0, 0, 255);
        SDL_RenderDrawLine(Game::getInstance().getRenderer(), a.x, a.y, b.x, b.y);
//        sdl_rect.x = box.x;
//        sdl_rect.y = box.y;
//        sdl_rect.w = box.w;
//        sdl_rect.h = box.h;
//
//        SDL_RenderFillRect(Game::getInstance().getRenderer(), &sdl_rect);

        auto normals = getNormals();

        uint k = 0;
        for (auto v : normals) {
            a = center - cameraDisplacement;
            float q = 0.5f * ((((k++) %2) == 0) ? box.w : box.h);
            b = center + v * q - cameraDisplacement;
            SDL_RenderDrawLine(Game::getInstance().getRenderer(), a.x, a.y, b.x, b.y);
        }

    }

}

void CollidableBox::setLT(Vec2 v) {
    box.setLT(v);
}

Vec2 CollidableBox::getCenter() {
    return box.getCenter();
}

Vec2 CollidableBox::getClosestNormalToPoint(Vec2 p) {

    auto normals = getNormals();

    vector<float> normalDirections;

    for (auto v : normals)
        normalDirections.push_back(v.ang_rad());

    float connectingLineAngle = (p - box.getCenter()).ang_rad();

    //find min dif
    int min_k = -1;
    int min = numeric_limits<float>::infinity();

    for (uint k = 0; k < normalDirections.size(); k++) {
        float distance = abs(normalDirections[k] - connectingLineAngle);
        if (distance < min) {
            min = distance;
            min_k = k;
        }
    }

    if (min_k < 0) {
        cerr << "Cannot find min! FIX ME!" << endl;
    }

    return normals[min_k];
}

vector<Vec2> CollidableBox::getNormals() {
    vector<Vec2> normals;
    auto corners = box.getCorners();

    //compute all normals

    for (uint i = 0; i < corners.size(); i++) {
        normals.push_back((corners[(i + 1) % corners.size()] - corners[i]).getNormalizedVector().rotated(M_PI_2));
    }
    return normals;
}

CollidableBox::CollisionAvoidanceInfo CollidableBox::getInfoToAvoidCollision(Vec2 p) {
    auto normals = getNormals();

    vector<float> normalDirections;

    for (auto v : normals)
        normalDirections.push_back(v.ang_rad());

    float connectingLineAngle = (p - box.getCenter()).ang_rad();

    //find min dif
    int min_k = -1;
    float min = numeric_limits<float>::infinity();

    for (uint k = 0; k < normalDirections.size(); k++) {
        float distance = abs(normalDirections[k] - connectingLineAngle);
        if (distance < min) {
            min = distance;
            min_k = k;
        }
    }

    if (min_k < 0) {
        cerr << "Cannot find min! FIX ME!" << endl;
    }

    float q = 0.5f * (((min_k %2) == 0) ? box.w : box.h);


    return CollidableBox::CollisionAvoidanceInfo(normals[min_k], q);
}
