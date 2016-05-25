//
// Created by jasf on 5/24/16.
//

#include "CollidableBox.h"
#include "Camera.h"
#include "Game.h"

bool CollidableBox::is(string className) {
    return className == "CollidableBox" ;
}

CollidableBox::CollidableBox(Vec2 topLeft, int width, int heigth) : box(topLeft, heigth, width) {


}

void CollidableBox::render() {
    auto corners = box.getCorners();
    static SDL_Rect sdl_rect;
    for (uint i = 0; i < corners.size(); i++) {
        Vec2 a = corners[i] - Camera::getPos(Camera::PLAYER_GROUND_VIEW);
        Vec2 b = corners[(i + 1) % corners.size()] - Camera::getPos(Camera::PLAYER_GROUND_VIEW);
        SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 255,0,0,255);
        SDL_RenderDrawLine(Game::getInstance().getRenderer(), a.x, a.y, b.x, b.y);
//        sdl_rect.x = box.x;
//        sdl_rect.y = box.y;
//        sdl_rect.w = box.w;
//        sdl_rect.h = box.h;
//
//        SDL_RenderFillRect(Game::getInstance().getRenderer(), &sdl_rect);

    }

}

void CollidableBox::setLT(Vec2 v) {
    box.setLT(v);
}
