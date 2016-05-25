//
// Created by jasf on 5/22/16.
//

#include "PlayableEmotion.h"
#include "Camera.h"
#include "TyperInput.h"
#include "CollidableBox.h"
#include "ForceField.h"
#include "Collision.h"

PlayableEmotion::PlayableEmotion(int x, int y) : GameObject(),
                                                 runnigSp("img/Sprite_Run.png", 15, 0.15),
                                                 gettingToRunSp("img/sprite_StartRun.png", 8, 0.15),
                                                 stopingToRunSp("img/sprite_EndRun.png", 6, 0.15),
                                                 turningSp("img/Sprite_Turn.png", 11, 0.15),
                                                 idleSp("img/Sprite_Idle.png", 5, 0.15),
                                                 idleStartJump("img/idleJump_start.png", 4, 0.15),
                                                 idleJump("img/idleJump_Jump.png", 3, 0.15),
                                                 runningJumpSp("img/moveJump_Jump.png", 3, 0.6),
                                                 runningStartJumpSp("img/moveJump_Start.png", 4, 0.15),
                                                 jumpEndSp("img/Jump_End.png", 7, 0.15) {

    defeated = false;

    currentState = PlayableState::IDLE;
    currentlyFacing = PlayableFacing::RIGHT;
    rotation = 0;

    speedAngle = 0;
    speedMagnitude = 0;

    accelerationAngle = 0;
    accelerationMagnitude = 0;

    speed = Vec2::getVec2FromPolar(speedMagnitude, speedAngle);
    acceleration = Vec2::getVec2FromPolar(accelerationMagnitude, accelerationAngle);

    int width = runnigSp.getWidth() / 15;
    int height = runnigSp.getHeight();

    center_LT_displacement = -0.5 * Vec2(width, height);

    pos = Vec2(x, y);
    speed = Vec2(0, 0);
    acceleration = Vec2(0, 0);
    previousPos = pos;


    collisionVolume = new CollidableBox(pos + center_LT_displacement, width, height);

    activeActionTimer.restart();

}

void PlayableEmotion::update(float dt) {

    activeActionTimer.update(dt);

    TyperInput &im = TyperInput::getInstance();

    //get typing input and decide state transition
    if (im.hasTypintEvent()) {
        TyperInput::TypingEvent e = im.getTypingEvent();

        switch (currentState) {
            case PlayableState::IDLE:
                switch (e) {
                    case TyperInput::TypingEvent::TURN :
                        currentState = PlayableState::TURNING;
                        idleSp.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::RUN :
                        currentState = PlayableState::GETTING_TO_RUN;
                        gettingToRunSp.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::JUMP :
                        currentState = PlayableState::IDLE_JUMP_START;
                        idleStartJump.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    default:
                        break;
                }
                break;
            case PlayableState::GETTING_TO_RUN:
                break;
            case PlayableState::RUNNING:
                switch (e) {
                    case TyperInput::TypingEvent::STOP :
                        currentState = PlayableState::STOPING_RUN;
                        stopingToRunSp.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::JUMP :
                        currentState = PlayableState::RUNNING_JUMP_START;
                        runningStartJumpSp.setFrame(0);
                        break;
                    default:
                        break;
                }
                break;
            case PlayableState::STOPING_RUN:
                break;
            case PlayableState::TURNING:
                break;
            default:
                break;

        }

    }
    //update forces accorging to state
    switch (currentState) {
        case PlayableState::IDLE:
            idleSp.update(dt);
            break;
        case PlayableState::GETTING_TO_RUN:
            acceleration = Vec2(5, 0);
            gettingToRunSp.update(dt);
            if (gettingToRunSp.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                runnigSp.setFrame(0);
            }
            break;
        case PlayableState::RUNNING:
            runnigSp.update(dt);
            acceleration = Vec2(0, 0);
            speed.x = 20;
            break;
        case PlayableState::IDLE_JUMP_START:
            idleStartJump.update(dt);
            if (idleStartJump.isThistLastFrame()) {
                currentState = PlayableState::IDLE_JUMP_JUMPING;
                idleStartJump.setFrame(0);
                speed += Vec2(0, -120);
            }
            break;
        case PlayableState::IDLE_JUMP_JUMPING:
            idleJump.update(dt);
            if (idleJump.isThistLastFrame()) {
                currentState = PlayableState::JUMP_END;
                jumpEndSp.setFrame(0);
            }
            break;
        case PlayableState::JUMP_END:
            jumpEndSp.update(dt);
            if (jumpEndSp.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                idleSp.setFrame(0);
            }
            break;
        case PlayableState::STOPING_RUN:
            stopingToRunSp.update(dt);
            acceleration = Vec2(-5, 0);
            if (stopingToRunSp.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                idleSp.setFrame(0);

            }
            break;
        case PlayableState::RUNNING_JUMP_START:
            runningStartJumpSp.update(dt);
            if(runningStartJumpSp.isThistLastFrame()){
                currentState = PlayableState::RUNNING_JUMP_JUMPING;
                runningJumpSp.setFrame(0);
                speed += Vec2(0, -150);
            }
            break;
        case PlayableState::RUNNING_JUMP_JUMPING:
            runningJumpSp.update(dt);
            if(runningJumpSp.isThistLastFrame()){
                currentState = PlayableState::RUNNING_JUMP_END;
                jumpEndSp.setFrame(0);
            }
            break;
        case PlayableState::RUNNING_JUMP_END:
            jumpEndSp.update(dt);
            if(jumpEndSp.isThistLastFrame()){
                currentState = PlayableState::RUNNING;
                runnigSp.setFrame(0);
            }
            break;
        case PlayableState::TURNING:
            acceleration = Vec2(0, 0);
            turningSp.update(dt);
            if (turningSp.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                toogleDirection();
                idleSp.setFrame(0);

            }
            break;
        default:
            break;

    }

    acceleration += ForceField::getInstance()->getForceAt(pos);

    //integrate time equation
    speed += dt * acceleration;
    previousPos = pos;
    pos += dt * speed;

    ((CollidableBox *) collisionVolume)->setLT(pos + center_LT_displacement);

}

void PlayableEmotion::toogleDirection() {
    currentlyFacing = (currentlyFacing == RIGHT) ? LEFT : RIGHT;
}

bool PlayableEmotion::isDead() {
    return defeated;
}

void PlayableEmotion::notifyCollision(GameObject &other) {

    vector<pair<Vec2, Vec2>> collidingSegments;

    if (other.is("KillingRectangle")) {
        defeated = true;



    }
    else if (other.is("SupportRectangle")) {

        Vec2 testPoint = pos;

        Vec2 directionToEscape = (previousPos - pos).getNormalizedVector();
        float step = 10;

        while (true) {
            testPoint = testPoint + Vec2(0, -1 * step);
            ((CollidableBox *) collisionVolume)->setLT(testPoint + center_LT_displacement);
            if (!Collision::IsColliding(getCollisionVolume(), other.getCollisionVolume()))
                break;
        }

        speed.y = 0;
        pos = testPoint;
        ((CollidableBox *) collisionVolume)->setLT(pos + center_LT_displacement);


    }else{

    }

}

bool PlayableEmotion::is(std::string type) {
    return type == "PlayableEmotion";
}


void PlayableEmotion::render() {
    GameObject::render();

    Vec2 pos = getCenterPos() + center_LT_displacement - Camera::getPos(1);

    switch (currentState) {
        case PlayableState::RUNNING:
            runnigSp.render(pos.x, pos.y, rotation,
                            (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::GETTING_TO_RUN:
            gettingToRunSp.render(pos.x, pos.y, rotation,
                                  (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE:
            idleSp.render(pos.x, pos.y, rotation,
                          (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE_JUMP_START:
            idleStartJump.render(pos.x, pos.y, rotation,
                            (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE_JUMP_JUMPING:
            idleJump.render(pos.x, pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::JUMP_END:
            jumpEndSp.render(pos.x, pos.y, rotation,
                            (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_START:
            runningStartJumpSp.render(pos.x, pos.y, rotation,
                             (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_JUMPING:
            runningJumpSp.render(pos.x, pos.y, rotation,
                                      (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_END:
            jumpEndSp.render(pos.x, pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::STOPING_RUN:
            stopingToRunSp.render(pos.x, pos.y, rotation,
                                  (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::TURNING:
            turningSp.render(pos.x, pos.y, rotation,
                             (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;

    }
}

void PlayableEmotion::clearCollisionState() {
    restrictingLineSegments.clear();

}
