//
// Created by jasf on 5/22/16.
//

#include "PlayableEmotion.h"
#include "Camera.h"
#include "TyperInput.h"
#include "AxisAlignedBoundingBox.h"
#include "ForceField.h"
#include "Game.h"
#include "EndState.h"
#include "SettingsLoad.h"

int PlayableEmotion::RUNNING_VELOCITY = 80;
int PlayableEmotion::IDLE_JUMP_INITIAL_UPWARD_VELOCITY = 150;
int PlayableEmotion::RUNNING_JUMP_UPWARD_INITIAL_VELOCITY = 180;

PlayableEmotion::PlayableEmotion(int x, int y) : GameObject(),
                                                 runnigSp("img/Sprite_Run.png", 15, SECONDS_PER_FRAME),
                                                 gettingToRunSp("img/sprite_StartRun.png", 8, SECONDS_PER_FRAME),
                                                 stopingToRunSp("img/sprite_EndRun.png", 6, SECONDS_PER_FRAME),
                                                 turningSp("img/Sprite_Turn.png", 11, SECONDS_PER_FRAME),
                                                 idleSp("img/Sprite_Idle.png", 5, SECONDS_PER_FRAME),
                                                 idleStartJump("img/idleJump_start.png", 4, SECONDS_PER_FRAME),
                                                 idleJump("img/idleJump_Jump.png", 3, SECONDS_PER_FRAME),
                                                 runningJumpSp("img/moveJump_Jump.png", 3, SECONDS_PER_FRAME),
                                                 runningStartJumpSp("img/moveJump_Start.png", 4, SECONDS_PER_FRAME),
                                                 jumpEndSp("img/Jump_End.png", 7, SECONDS_PER_FRAME),
                                                 turnRunSp("img/sprite_MoveTurn.png", 11, SECONDS_PER_FRAME),
                                                 dashSp("img/sprite_Dash.png", 4, SECONDS_PER_FRAME),
                                                 fallingSp("img/sprites_falling.png", 2, SECONDS_PER_FRAME) {

    loadSettings();

    defeated = false;

    currentState = PlayableState::FALLING;
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
    speed = Vec2(0, 1);
    acceleration = Vec2(0, 0);

    auxCollisionVolume[0].setCenter(pos + Vec2::getVec2FromPolar(height / 4, M_PI_2));
    auxCollisionVolume[1].setCenter(pos + Vec2::getVec2FromPolar(0, -1 * M_PI_2));
    auxCollisionVolume[2].setCenter(pos + Vec2::getVec2FromPolar(height / 4, -1 * M_PI_2));

    auxCollisionVolume[0].setRadius(4.0 / 5.0 * min(width / 2, height / 2));
    auxCollisionVolume[1].setRadius(4.0 / 5.0 * min(width / 2, height / 2));
    auxCollisionVolume[2].setRadius(4.0 / 5.0 * min(width / 2, height / 2));

    collisionVolume = new AxisAlignedBoundingBox(pos + center_LT_displacement, width, height);

    activeActionTimer.restart();
    wasRunning = false;

    supported = false;
    iterationsSinceContactWithSupport = 0;

    iterationsSinceLastHorizontalSupportNodification = 0;
    iterationsSinceLastVerticalSupportNodification = 0;


}

void PlayableEmotion::update(float dt) {

    activeActionTimer.update(dt);

    TyperInput &im = TyperInput::getInstance();

    iterationsSinceContactWithSupport--;
    iterationsSinceLastHorizontalSupportNodification++;
    iterationsSinceLastVerticalSupportNodification++;

    //update due to lack of contact with support
    switch (currentState) {
        case PlayableState::RUNNING:
            if (iterationsSinceLastHorizontalSupportNodification > 15) {
                cout << "was running, now falling!" << endl;
                currentState = PlayableState::FALLING;
                fallingSp.setFrame(0);
                wasRunning = true;
                break;
            }
            break;
        default:
            break;
    }


    acceleration = Vec2(0, 0);

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
                        wasRunning = true;
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
                    case TyperInput::TypingEvent::VERTICAL_SUPPORT_FOUND:
                        currentState = PlayableState::STOPING_RUN;
                        speed.x = 0.0;
                        stopingToRunSp.setFrame(0);
                        break;
                    case TyperInput::TypingEvent::TURN :
                        currentState = PlayableState::TURN_RUN;
                        turnRunSp.setFrame(0);
                        activeActionTimer.restart();
                        speed.x = 0.0;
                        break;
                    case TyperInput::TypingEvent::STOP :
                        currentState = PlayableState::STOPING_RUN;
                        stopingToRunSp.setFrame(0);
                        activeActionTimer.restart();
                        wasRunning = false;
                        break;
                    case TyperInput::TypingEvent::JUMP :
                        currentState = PlayableState::RUNNING_JUMP_START;

                        //untach from ground
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
            case PlayableState::FALLING:
                switch (e) {
                    case TyperInput::TypingEvent::HORIZONTAL_SUPPORT_FOUND:
                        speed.y = 0;
                        if (wasRunning) {
                            currentState = PlayableState::RUNNING_JUMP_END;
                            jumpEndSp.setFrame(0);
                            activeActionTimer.restart();

                        }
                        else {
                            currentState = PlayableState::JUMP_END;
                            jumpEndSp.setFrame(0);
                            activeActionTimer.restart();
                        }

                        break;
                    case TyperInput::TypingEvent::DASH: {
                        currentState = PlayableState::DASHING;
                        dashSp.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    }
                    default:
                        break;
                }
                break;
            case PlayableState::DASHING:
                switch (e) {
                    case TyperInput::TypingEvent::COLIDED:
                        if (!wasRunning) {
                            currentState = PlayableState::RUNNING_JUMP_END;
                            jumpEndSp.setFrame(0);
                            activeActionTimer.restart();

                        }
                        else {
                            currentState = PlayableState::JUMP_END;
                            jumpEndSp.setFrame(0);
                            activeActionTimer.restart();
                        }

                        break;
                    default:
                        break;
                }
                break;
            default:
                break;

        }

    }
    //update forces(acceleration and possibly speed directly)  accorging to state
    switch (currentState) {
        case PlayableState::IDLE:
            acceleration = Vec2(0, 0);
            speed.x = 0;
            idleSp.update(dt);
            break;
        case PlayableState::GETTING_TO_RUN:
            acceleration += Vec2(5 * getDirectionHorizontalMultiplier(), 0);
            gettingToRunSp.update(dt);
            if (gettingToRunSp.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                runnigSp.setFrame(0);
            }
            break;
        case PlayableState::RUNNING:
            runnigSp.update(dt);
            speed.x = RUNNING_VELOCITY * getDirectionHorizontalMultiplier();
            break;
        case PlayableState::IDLE_JUMP_START:
            idleStartJump.update(dt);
            if (idleStartJump.isThistLastFrame()) {
                currentState = PlayableState::IDLE_JUMP_JUMPING;
                idleStartJump.setFrame(0);
                speed += Vec2(0, -IDLE_JUMP_INITIAL_UPWARD_VELOCITY);
            }
            break;
        case PlayableState::IDLE_JUMP_JUMPING:
            idleJump.update(dt);
            if (idleJump.isThistLastFrame()) {
                currentState = PlayableState::FALLING;
                fallingSp.setFrame(0);
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
            acceleration = Vec2(-5 * getDirectionHorizontalMultiplier(), 0);
            if (stopingToRunSp.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                idleSp.setFrame(0);

            }
            break;
        case PlayableState::RUNNING_JUMP_START:
            runningStartJumpSp.update(dt);
            if (runningStartJumpSp.isThistLastFrame()) {
                currentState = PlayableState::RUNNING_JUMP_JUMPING;
                runningJumpSp.setFrame(0);
                pos += Vec2(0, -5);
                speed += Vec2(0, -RUNNING_JUMP_UPWARD_INITIAL_VELOCITY);
            }
            break;
        case PlayableState::FALLING:
            fallingSp.update(dt);
            break;
        case PlayableState::RUNNING_JUMP_JUMPING:
            runningJumpSp.update(dt);
            if (runningJumpSp.isThistLastFrame()) {
                currentState = PlayableState::FALLING;
                fallingSp.setFrame(0);
            }
            break;
        case PlayableState::RUNNING_JUMP_END:
            jumpEndSp.update(dt);
            if (jumpEndSp.isThistLastFrame()) {
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
        case PlayableState::TURN_RUN:
            turnRunSp.update(dt);
            if (turnRunSp.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                toogleDirection();
                runnigSp.setFrame(0);
            }
            break;
        case PlayableState::DASHING :
            dashSp.update(dt);
            acceleration -= ForceField::getInstance()->getForceAt(pos);
            speed.x = 20;
            if (activeActionTimer.get() > 2.0) {
                currentState = PlayableState::FALLING;
                fallingSp.setFrame(0);

            }
        default:
            break;

    }

    acceleration += ForceField::getInstance()->getForceAt(pos);

    //integrate time equation
    speed += dt * acceleration;

    updatePos(pos + dt * speed);


}

int PlayableEmotion::getDirectionHorizontalMultiplier() const { return ((currentlyFacing == RIGHT) ? 1 : -1); }

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
    if (other.is("VictoryRectangle")) {
//        Game::getInstance().getCurrentState().requestPop();
        Game::getInstance().push(new EndState({true}));

    }
    else if (other.is("CollidableAABBGameObject")) {
        Rect collidingRect = ((AxisAlignedBoundingBox *) other.getCollisionVolume())->axisAlignedRectangle;

        //among intermal circles, find the one that is closes to the axis aligned rectangle
        int min_k = -1;
        double distanceBetweenCircleAndRectangle = numeric_limits<float>::infinity();

        for (int k = 0; k < 3; k++) {
            const Vec2 &currentCenter = auxCollisionVolume[k].getCenter();
            double dd = Vec2::distanceBetweenPoints(currentCenter, collidingRect.getClosestPointTo(currentCenter));
            if (dd < distanceBetweenCircleAndRectangle) {
                min_k = k;
                distanceBetweenCircleAndRectangle = dd;
            }
        }

        const Vec2 closestInnerCircle = auxCollisionVolume[min_k].getCenter();
        const Vec2 closestPointOnRectagle = collidingRect.getClosestPointTo(closestInnerCircle);

        double overlap = auxCollisionVolume[min_k].getRadius() - distanceBetweenCircleAndRectangle;

        static double TOLERANCE = M_PI / 20.0;

        if (overlap >= 0.0) {
            Vec2 directionToEscape = (closestInnerCircle - closestPointOnRectagle).getNormalizedVector();
            updatePos(pos + overlap * directionToEscape);

            double angleToEscape = directionToEscape.ang_rad();

            if (abs(angleToEscape) < TOLERANCE || abs(angleToEscape - M_PI) < TOLERANCE) {
                if (iterationsSinceLastVerticalSupportNodification > 10) {
                    TyperInput::getInstance().addEventOnFront(
                            TyperInput::TypingEvent::VERTICAL_SUPPORT_FOUND);
                    iterationsSinceLastVerticalSupportNodification = 0;

                }
                speed.x = 0;
            } else if (abs(angleToEscape - M_PI_2) < TOLERANCE || abs(angleToEscape + M_PI_2) < TOLERANCE) {
                if (iterationsSinceLastHorizontalSupportNodification > 10) {
                    TyperInput::getInstance().addEventOnFront(
                            TyperInput::TypingEvent::HORIZONTAL_SUPPORT_FOUND);
                    iterationsSinceLastHorizontalSupportNodification = 0;

                }
                speed.y = 0;
            }
            else {

            }


        }


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
                                      (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                : SDL_FLIP_NONE);
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
        case PlayableState::TURN_RUN:
            turnRunSp.render(pos.x, pos.y, rotation,
                             (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::FALLING:
            fallingSp.render(pos.x, pos.y, rotation,
                             (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::DASHING:
            dashSp.render(pos.x, pos.y, rotation,
                          (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;

    }

    for (uint k = 0; k < 3; k++)
        auxCollisionVolume[k].render();
}

void PlayableEmotion::updatePos(Vec2 center) {
    for (uint k = 0; k < 3; k++)
        auxCollisionVolume[k].moveCenter(center - pos);

    pos = center;
    ((AxisAlignedBoundingBox *) collisionVolume)->setLeftTopCorner(pos + center_LT_displacement);

}

void PlayableEmotion::loadSettings() {

    SettingsLoad *settings = SettingsLoad::getInstance();
    RUNNING_VELOCITY = (int) settings->get("RUNNING_VELOCITY", 80);
    IDLE_JUMP_INITIAL_UPWARD_VELOCITY = (int) settings->get("IDLE_JUMP_INITIAL_UPWARD_VELOCITY", 150);
    RUNNING_JUMP_UPWARD_INITIAL_VELOCITY = (int) settings->get("RUNNING_JUMP_UPWARD_INITIAL_VELOCITY", 180);

}
