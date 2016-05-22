//
// Created by jasf on 5/22/16.
//

#include "PlaybleEmotion.h"
#include "Camera.h"

void PlaybleEmotion::update(float dt) {

    sp.update(dt);

}

bool PlaybleEmotion::isDead() {
    return false;
}

void PlaybleEmotion::notifyCollision(GameObject &other) {

}

bool PlaybleEmotion::is(std::string type) {
    return type == "PlayableEmotion";
}

PlaybleEmotion::PlaybleEmotion() : sp("img/dummyCharSprite.png", 4, 0.25) {

}

void PlaybleEmotion::render() {
    GameObject::render();

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(1);

    sp.render(pos.x, pos.y, 0);
}
