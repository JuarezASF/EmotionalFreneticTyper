//
// Created by jasf on 4/6/16.
//

#include "defines.h"
#include "Camera.h"
#include "Game.h"
#include "InputManager.h"
#include "Logger.h"

GameObject *Camera::focus = nullptr;

Vec2 Camera::speed(0, 0);
std::vector<Vec2> Camera::paralax_pos;
bool Camera::hasBenInitialized;
std::vector<float> Camera::paralax_sinsitivity;

int Camera::BACKGROUND_VIEW = 0;
int Camera::PLAYER_GROUND_VIEW = 1;
int Camera::TOPGROUND_VIEW = 2;

void Camera::follow(GameObject *newFocus) {
    focus = newFocus;

    paralax_pos[Camera::PLAYER_GROUND_VIEW] = -1*Game::getInstance().getScreenDimensions() * 0.5 + focus->getCenterPos();
}

void Camera::unfollow() {
    focus = nullptr;

}

void Camera::update(float dt) {
    if (focus != nullptr) {

        Vec2 newPos = -1*Game::getInstance().getScreenDimensions() * 0.5  + focus->getCenterPos();

        Vec2 ds = newPos - paralax_pos[Camera::PLAYER_GROUND_VIEW];


        for (unsigned int k = 0; k < paralax_pos.size(); k++)
            paralax_pos[k] = paralax_pos[k] + paralax_sinsitivity[k] * ds;
    }
    else {

        const float camera_sensitivity = 400.0;

        speed = Vec2(0, 0);

        if (InputManager::getInstance().isKeyDown(LEFT_ARROW_KEY)) {
            speed -= Vec2::getVec2FromPolar(camera_sensitivity, 0);
        }
        if (InputManager::getInstance().isKeyDown(RIGHT_ARROW_KEY)) {
            speed -= Vec2::getVec2FromPolar(camera_sensitivity, -1 * M_PI);
        }
        if (InputManager::getInstance().isKeyDown(UP_ARROW_KEY)) {
            speed -= Vec2::getVec2FromPolar(camera_sensitivity, M_PI_2);
        }
        if (InputManager::getInstance().isKeyDown(DOWN_ARROW_KEY)) {
            speed -= Vec2::getVec2FromPolar(camera_sensitivity, -1 * M_PI_2);
        }

        for (unsigned int k = 0; k < paralax_pos.size(); k++)
            paralax_pos[k] = paralax_pos[k] + paralax_sinsitivity[k] * dt * speed;


    }


}

void Camera::setLvsOfParalax(unsigned int k, std::vector<float> sensitivities) {

    if (hasBenInitialized)
        Logger::err("lvs of paralax have alrady been set!");

    if (sensitivities.size() != k)
        Logger::err("Vector with sensitivity must be of length k!");

    paralax_pos.clear();

    for (unsigned int i = 0; i < k; i++)
        paralax_pos.emplace_back(0, 0);

    paralax_sinsitivity.clear();
    for (unsigned int i = 0; i < k; i++)
        paralax_sinsitivity.push_back(sensitivities[i]);

    hasBenInitialized = true;

}

Vec2 Camera::getPos(unsigned int k) {
    if (k > paralax_pos.size())
        Logger::err("Invalid paralax lv!");

    return paralax_pos[k];
}

GameObject *Camera::getPlayer() {
    return focus;
}
