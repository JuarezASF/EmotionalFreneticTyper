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

    setColor(255, 0, 0, 255);


}

//MARCOS, add this somewhere
typedef unsigned int uint;

void CollidableBox::render() {
    auto corners = box.getCorners();
    static SDL_Rect sdl_rect;
    auto center = box.getCenter();
    const Vec2 &cameraDisplacement = Camera::getPos(Camera::PLAYER_GROUND_VIEW);
    for (uint i = 0; i < corners.size(); i++) {
        Vec2 a = corners[i] - cameraDisplacement;
        Vec2 b = corners[(i + 1) % corners.size()] - cameraDisplacement;
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), colorR, colorG, colorB, colorA);
        SDL_RenderDrawLine(Game::getInstance().getRenderer(), a.x, a.y, b.x, b.y);

        auto normals = getNormals();

        uint k = 0;
        for (auto v : normals) {
            a = center - cameraDisplacement;
            float q = 0.5f * ((((k++) % 2) == 0) ? box.w : box.h);
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
    auto corners = box.getCorners();

    Vec2 vx = (corners[3] - corners[0]).getNormalizedVector();
    Vec2 vy = (corners[1] - corners[0]).getNormalizedVector();

    p = p - box.getTopLeft();

    float x = p.dot(vx);
    float y = p.dot(vy);

    if (x <= box.w && x >= 0 && y <= 0) {
        return {normals[3], box.h / 2, 3};
    } else if (x <= 0 && y >= 0 && y <= box.h) {
        return {normals[0], box.w / 2, 0};
    }
    else if (x >= 0 && x <= box.w && y >= box.h) {
        return {normals[1], box.h / 2, 1};
    }
    else if (x >= box.w && y >= 0 && y <= box.h) {
        return {normals[2], box.w / 2, 2};
    }
    else{
        //TODO more 4 cases
        return CollidableBox::CollisionAvoidanceInfo(Vec2(1,0), 20,2 );

    }

}

void CollidableBox::setColor(int r, int g, int b, int a) {
    colorR = r;
    colorG = g;
    colorB = b;
    colorA = a;

}
