//
// Created by jasf on 5/22/16.
//

#include "PlayableEmotion.h"
#include "Camera.h"
#include "TyperInput.h"
#include "CollidableBox.h"

void PlayableEmotion::update(float dt) {

    sp.update(dt);

    TyperInput &im = TyperInput::getInstance();

    if (im.hasTypintEvent()) {
        TyperInput::TypingEvent e = im.getTypingEvent();

        switch (e) {
            case TyperInput::TypingEvent::TURN :
                if (currentState == PlayableState::DASHING)
                    break;

                this->currentlyFacing = (currentlyFacing == PlayableFacing::RIGHT) ? PlayableFacing::LEFT
                                                                                   : PlayableFacing::RIGHT;
                speedAngle = (currentlyFacing == PlayableFacing::RIGHT) ? 0: M_PI;

                break;
            case TyperInput::TypingEvent::MOVE :
                currentState = PlayableState::WALKING;
                speedMagnitude = 50;
                speedAngle = (currentlyFacing == PlayableFacing::RIGHT) ? 0: M_PI;
                break;
            case TyperInput::TypingEvent::UP :
                currentState = PlayableState::JUMPING;
                speedMagnitude = 50;
                speedAngle = - 1*M_PI_2;
                break;
            case TyperInput::TypingEvent::DOWN :
                currentState = PlayableState::FALLING;
                speedMagnitude = 50;
                speedAngle = 1*M_PI_2;
                break;
            case TyperInput::TypingEvent::RUN :
                currentState = PlayableState::RUNNING;
                speedMagnitude = 80;

                break;
            case TyperInput::TypingEvent::STOP :
                currentState = PlayableState::IDLE;
                speedMagnitude = 0;
                accelerationMagnitude = 0;

                break;
            case TyperInput::TypingEvent::DASH :
                currentState = PlayableState::DASHING;
                speedMagnitude = 100;
                accelerationMagnitude = 0;
                dashTimer.restart();

                break;
            case TyperInput::TypingEvent::JUMP :
                //TODO

                break;
            default:

                break;
        }
    }


    switch (currentState) {
        case PlayableState::IDLE:
            break;
        case PlayableState::WALKING:
            break;
        case PlayableState::RUNNING:
            break;
        case PlayableState::DASHING:
            dashTimer.update(dt);
            if (dashTimer.get() > 2) {
                currentState = PlayableState::WALKING;
                speedMagnitude = 50;
            }
            break;
        case PlayableState::JUMPING:
            break;
        case PlayableState::FALLING:
            break;
        case PlayableState::GRABBING:
            break;
        default:
            break;
    }

    speed = Vec2::getVec2FromPolar(speedMagnitude, speedAngle);

    if (speed.magnitude() < 0.1)
        speed = Vec2(0,0);

    pos += dt * speed;

    ((CollidableBox *)collisionVolume)->setLT(pos + center_LT_displacement);

}

bool PlayableEmotion::isDead() {
    return false;
}

void PlayableEmotion::notifyCollision(GameObject &other) {

    if (other.is("SupportRectangle")){
        speedMagnitude = 0;
    }

}

bool PlayableEmotion::is(std::string type) {
    return type == "PlayableEmotion";
}

PlayableEmotion::PlayableEmotion() : GameObject(), sp("img/dummyCharSprite.png", 8, 0.25) {

    currentState = PlayableState::IDLE;
    currentlyFacing = PlayableFacing::RIGHT;
    rotation = 0;

    speedAngle = 0;
    speedMagnitude = 0;

    accelerationAngle = 0;
    accelerationMagnitude = 0;

    speed = Vec2::getVec2FromPolar(speedMagnitude, speedAngle);
    acceleration = Vec2::getVec2FromPolar(accelerationMagnitude, accelerationAngle);

    int width = sp.getWidth()/8;
    int height = sp.getHeight();

    center_LT_displacement = -0.5 * Vec2(width, height);

    pos = 0.5 * Vec2(width, height);

    collisionVolume = new CollidableBox(pos + center_LT_displacement, width, height);

}

void PlayableEmotion::render(bool forceDrawn) {
    GameObject::render(forceDrawn);

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(1);

    sp.render(pos.x, pos.y, rotation, (currentlyFacing == PlayableFacing::LEFT)? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
