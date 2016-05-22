//
// Created by jasf on 4/21/16.
//

#include "Alien.h"
#include "InputManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Game.h"
#include "Animation.h"
#include "Logger.h"

float Alien::VELOCITY_CONST = 150;
int Alien::alienCount = 0;
float Alien::alienCoolDownTime = 1.5;

Alien::Alien(float x, float y, int nMinions) {
    alienCount++;
    hp = 100;
    angular_velocity = -1 * M_PI_4;
    sp.open("img/alien.png");

    int width = sp.getWidth();
    int height = sp.getHeight();

    pos = Vec2(x, y);
    speed = Vec2::getVec2FromPolar(0, 0);

    center_LT_displacement = -1 * Vec2(64, 79);

    box = Rect(Vec2(x, y) + center_LT_displacement, height, width);

    float offset = 0;
    for (int i = 0; i < nMinions; i++) {
        minionArray.push_back(new Minion(this, offset));
        offset += 2 * M_PI / nMinions;
    }

    state = AlientState::RESTING;

    restTimer.restart();

    dyingSound.open("audio/boom.wav");


}

Alien::~Alien() {

    for (unsigned int i = 0; i < minionArray.size(); i++)
        delete minionArray[i];

    minionArray.clear();

    alienCount--;


}

void Alien::update(float dt) {
    auto im = InputManager::getInstance();


    switch (state) {
        case (AlientState::RESTING):
            restTimer.update(dt);
            if (restTimer.get() > alienCoolDownTime) {
                GameObject *playerPtr = Camera::getPlayer();

                if (playerPtr != nullptr) {
                    destination = playerPtr->getCenterPos();

                    speed = (destination - pos).getNormalizedVector() * VELOCITY_CONST;
                    state = AlientState::MOVING;

                }
            }

            break;
        case (AlientState::MOVING):
            if ((destination - pos).magnitude() < 5) {
                pos = destination;
                speed = Vec2(0, 0);

                GameObject *player = Camera::getPlayer();

                if (player) {
                    Vec2 shotTarget = player->getCenterPos();
                    float target_angle = (shotTarget - this->getCenterPos()).ang_rad();
                    //find minion closest to target point
                    int idx = 0;
                    float min = 2 * M_PI;
                    float x;
                    for (unsigned int i = 0; i < minionArray.size(); i++) {
                        x = abs((minionArray[i]->getCenterPos() - this->getCenterPos()).ang_rad() - target_angle);
                        if (x < min) {
                            idx = i;
                            min = x;
                        }

                    }

                    minionArray[idx]->shoot(shotTarget);
                    restTimer.restart();
                    state = AlientState::RESTING;

                }
            }
            else {
                pos += speed * dt;
            }

            box.setLT(pos + center_LT_displacement);

            break;
        default:
            Logger::log("Invalid alien state!");
            break;
    }
    rotation += angular_velocity * dt;

    for (uint i = 0; i < minionArray.size(); i++)
        minionArray[i]->update(dt);

}

void Alien::render() {
    GameObject::render();
    Vec2 c = pos - Camera::getPos(Camera::PLAYER_GROUND_VIEW) + center_LT_displacement;
    sp.render(c.x, c.y, rotation);

    for (uint i = 0; i < minionArray.size(); i++)
        minionArray[i]->render();


}

bool Alien::isDead() {
    return hp <= 0;
}


void Alien::notifyCollision(GameObject &other) {
    if (other.is("Bullet") && !((Bullet &) other).getTargetsPlayer()) {
        damage(10);
    }
}

bool Alien::is(std::string type) {
    return type == "Alien";

}

void Alien::damage(int dm) {
    hp -= dm;

    if (hp <= 0) {
        static float step = 0.1;
        static int frameCount = 4;
        static int repeat = 3;

        dyingSound.play(1);
        Game::getInstance().getCurrentState().addObject(
                new Animation(pos.x, pos.y, 0, "img/aliendeath.png", repeat*step * frameCount, true, frameCount, step));

    }

}
