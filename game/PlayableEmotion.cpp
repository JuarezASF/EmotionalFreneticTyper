//
// Created by jasf on 5/22/16.
//

#include "PlayableEmotion.h"
#include "Camera.h"
#include "TyperInput.h"
#include "CollidableBox.h"
#include "jasf_math.h"

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

    for(auto line : restrictingLineSegments){
        Vec2 a = line.first;
        Vec2 b = line.second;


        Vec2 directionOfLine = (b-a).getNormalizedVector().rotated(M_PI_2);

        float mag = speed.dot(directionOfLine);

        Vec2 newSpeed = speed + Vec2::getVec2FromPolar(mag, directionOfLine.ang_rad());

        if (newSpeed.magnitude() > speed.magnitude())
            speed = speed - Vec2::getVec2FromPolar(mag, directionOfLine.ang_rad());
        else
            speed = newSpeed;
    }

}

bool PlayableEmotion::isDead() {
    return false;
}

void PlayableEmotion::notifyCollision(GameObject &other) {

    vector<pair<Vec2, Vec2>> collidingSegments;

    if (other.is("SupportRectangle")){

        //colide every line segment with every line segment composing both boxes to find the support line segement that
        // is effectively being colided with
        auto corners = ((CollidableBox*)other.getCollisionVolume())->box.getCorners();
        auto selfCorners = ((CollidableBox*)this->getCollisionVolume())->box.getCorners();
        Vec2 a, b, c, d;
        for(uint k = 0; k < corners.size(); k++){
            a = corners[k];
            b = corners[(k + 1) % corners.size()];
            for(uint j = 0; j < selfCorners.size(); j++) {
                c = selfCorners[j];
                d = selfCorners[(j+1)%selfCorners.size()];

                if(lineIntersect(a, b, c, d)){
                    collidingSegments.push_back(std::make_pair(a, b));
                    break;
                }
            }
        }
    }

    restrictingLineSegments.insert(restrictingLineSegments.end(), collidingSegments.begin(), collidingSegments.end());

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

void PlayableEmotion::render() {
    GameObject::render();

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(1);

    sp.render(pos.x, pos.y, rotation, (currentlyFacing == PlayableFacing::LEFT)? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void PlayableEmotion::clearCollisionState() {
    restrictingLineSegments.clear();

}
