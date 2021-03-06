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
#include "CollidableAABBGameObject.h"

int PlayableEmotion::RUNNING_VELOCITY = 80;
int PlayableEmotion::IDLE_JUMP_INITIAL_UPWARD_VELOCITY = 150;
int PlayableEmotion::RUNNING_JUMP_UPWARD_INITIAL_VELOCITY = 180;
float PlayableEmotion::SCALE_FACTOR_ON_MAIN_CHAR = 0.7;
int PlayableEmotion::CIRCLE_VERTICAL_SPACING = 30;
int PlayableEmotion::CIRCLE_RADIUS = 10;

PlayableEmotion::PlayableEmotion(int x, int y) : GameObject(),
                                                 spriteRunning("img/personagem/run_running.png", 12, SECONDS_PER_FRAME),
                                                 spriteGettingToRun("img/personagem/run_start.png", 6,
                                                                    SECONDS_PER_FRAME),
                                                 spriteStopingRun("img/personagem/run_end.png", 6, SECONDS_PER_FRAME),
                                                 spriteTurning("img/personagem/idle_turn.png", 12, SECONDS_PER_FRAME),
                                                 spriteIdle("img/personagem/idle.png", 7, 7, SECONDS_PER_FRAME),
                                                 spriteIdleJumpStart("img/personagem/idle_jump_start.png", 11,
                                                                     SECONDS_PER_FRAME * 0.6),
                                                 spriteIdleJumpJumping("img/personagem/idle_jump_going_up.png", 7,
                                                                       SECONDS_PER_FRAME),
                                                 spriteRunningJumpJumping("img/personagem/run_jump_going_up.png", 7,
                                                                          SECONDS_PER_FRAME),
                                                 spriteRunningJumpStartJump("img/personagem/run_jump_start.png", 7,
                                                                            SECONDS_PER_FRAME),
                                                 spriteJumpEnd("img/personagem/idle_jump_end.png", 18,
                                                               SECONDS_PER_FRAME),
                                                 spriteTurnRunning("img/personagem/run_turn.png", 12,
                                                                   SECONDS_PER_FRAME),
                                                 spriteDashing("img/personagem/dash.png", 5, SECONDS_PER_FRAME),
                                                 spriteFalling("img/sprites_falling.png", 2, SECONDS_PER_FRAME),
                                                 spriteSmashingForward("img/sprite_smash_forward.png", 4,
                                                                       SECONDS_PER_FRAME),
                                                 spriteSmashingUpward("img/sprite_smash_upward.png", 4,
                                                                      SECONDS_PER_FRAME),
                                                 spriteWinning("img/animation/winAnim.png", 6, 9, SECONDS_PER_FRAME),
                                                 spriteLosing("img/animation/death.png", 1, 21, SECONDS_PER_FRAME),
                                                 soundJump("audio/dash2.wav"),
                                                 soundDash("audio/dash2.wav") {

    allSprites.push_back(&spriteRunning);
    allSprites.push_back(&spriteGettingToRun);
    allSprites.push_back(&spriteStopingRun);
    allSprites.push_back(&spriteTurning);
    allSprites.push_back(&spriteIdle);
    allSprites.push_back(&spriteIdleJumpStart);
    allSprites.push_back(&spriteIdleJumpJumping);
    allSprites.push_back(&spriteRunningJumpJumping);
    allSprites.push_back(&spriteRunningJumpStartJump);
    allSprites.push_back(&spriteJumpEnd);
    allSprites.push_back(&spriteTurnRunning);
    allSprites.push_back(&spriteDashing);
    allSprites.push_back(&spriteFalling);
    allSprites.push_back(&spriteSmashingForward);
    allSprites.push_back(&spriteSmashingUpward);
    allSprites.push_back(&spriteWinning);
    allSprites.push_back(&spriteLosing);

    loadSettings();

    spriteIdle.setScaleX(0.5);
    spriteIdle.setScaleY(0.5);

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

    int width = spriteRunning.getSpriteFullWidth() / 15;
    int height = spriteRunning.getSpriteFullHeight();

    center_LT_displacement = -0.5 * Vec2(width, height);

    centerPos = Vec2(x, y);
    speed = Vec2(0, 1);
    acceleration = Vec2(0, 0);

    cout << "configured circle spacing is:" << CIRCLE_VERTICAL_SPACING << endl;
    cout << "configured circle radius is:" << CIRCLE_RADIUS << endl;

    auxCollisionVolume[0].setCenter(centerPos + Vec2::getVec2FromPolar(CIRCLE_VERTICAL_SPACING, (float) M_PI_2));
    auxCollisionVolume[1].setCenter(centerPos + Vec2::getVec2FromPolar(0, -1.0f * (float) M_PI_2));
    auxCollisionVolume[2].setCenter(centerPos + Vec2::getVec2FromPolar(CIRCLE_VERTICAL_SPACING, (float) (-1 * M_PI_2)));

    auxCollisionVolume[0].setRadius(CIRCLE_RADIUS);
    auxCollisionVolume[1].setRadius(CIRCLE_RADIUS);
    auxCollisionVolume[2].setRadius(CIRCLE_RADIUS);

    collisionVolume = new AxisAlignedBoundingBox(centerPos + center_LT_displacement, width, height);

    activeActionTimer.restart();
    wasRunning = false;

    supported = false;
    iterationsSinceContactWithSupport = 0;

    iterationsSinceLastHorizontalSupportNodification = 0;
    iterationsSinceLastVerticalSupportNodification = 0;
    isSmashing = false;

    applyScaleFactor(0.8);


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
        case PlayableState::IDLE:
            if (iterationsSinceLastHorizontalSupportNodification > 15) {
                cout << "was running, now falling!" << endl;
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);
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
                        spriteTurning.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::RUN :
                        currentState = PlayableState::GETTING_TO_RUN;
                        spriteGettingToRun.setFrame(0);
                        activeActionTimer.restart();
                        wasRunning = true;
                        break;
                    case TyperInput::TypingEvent::JUMP :
                        soundJump.play(0);
                        currentState = PlayableState::IDLE_JUMP_START;
                        spriteIdleJumpStart.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::SMASH :
                        soundDash.play(1);
                        currentState = PlayableState::SMASHING_UPWARD;
                        isSmashing = true;
                        spriteSmashingForward.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::GRAB:
                        if (canGrabLeft) {
                            if (grabableToTheLeft) {
                                Vec2 c = grabableToTheLeft->getCenterPos();
                                centerPos = c + Vec2(0, -20);
                            }

                        }
                        else if (canGrabRight) {
                            if (grabableToTheRight) {
                                Vec2 c = grabableToTheRight->getCenterPos();
                                centerPos = c + Vec2(0, -20);
                            }

                        } else {

                        }
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
                        spriteStopingRun.setFrame(0);
                        break;
                    case TyperInput::TypingEvent::TURN :
                        currentState = PlayableState::TURN_RUN;
                        spriteTurnRunning.setFrame(0);
                        activeActionTimer.restart();
                        speed.x = 0.0;
                        break;
                    case TyperInput::TypingEvent::STOP :
                        currentState = PlayableState::STOPING_RUN;
                        spriteStopingRun.setFrame(0);
                        activeActionTimer.restart();
                        wasRunning = false;
                        break;
                    case TyperInput::TypingEvent::JUMP :
                        soundJump.play(1);
                        currentState = PlayableState::RUNNING_JUMP_START;
                        spriteRunningJumpStartJump.setFrame(0);
                        break;
                    case TyperInput::TypingEvent::SMASH :
                        soundDash.play(1);
                        currentState = PlayableState::SMASHING_FORWARD;
                        activeActionTimer.restart();
                        isSmashing = true;
                        spriteSmashingForward.setFrame(0);
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
                        speed.x = 0;
                        if (wasRunning) {
                            currentState = PlayableState::RUNNING_JUMP_END;
                            spriteJumpEnd.setFrame(0);
                            activeActionTimer.restart();

                        }
                        else {
                            currentState = PlayableState::JUMP_END;
                            spriteJumpEnd.setFrame(0);
                            activeActionTimer.restart();
                        }

                        break;
                    case TyperInput::TypingEvent::DASH: {
                        currentState = PlayableState::DASHING;
                        soundDash.play(1);
                        spriteDashing.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    }
                    case TyperInput::TypingEvent::TURN :
                        toogleDirection();
                        break;
                    default:
                        break;
                }
                break;
            case PlayableState::DASHING:
                switch (e) {
                    case TyperInput::TypingEvent::VERTICAL_SUPPORT_FOUND:
                        currentState = PlayableState::FALLING;
                        spriteFalling.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    case TyperInput::TypingEvent::STOP:
                        currentState = FALLING;
                        spriteFalling.setFrame(0);
                        activeActionTimer.restart();
                    default:
                        break;
                }
                break;
            case PlayableState::SMASHING_FORWARD:
                switch (e) {
                    case TyperInput::TypingEvent::VERTICAL_SUPPORT_FOUND:
                        currentState = PlayableState::FALLING;
                        spriteFalling.setFrame(0);
                        activeActionTimer.restart();
                        break;
                    default:
                        break;
                }
                break;
            case PlayableState::SMASHING_UPWARD:
                switch (e) {
                    case TyperInput::TypingEvent::HORIZONTAL_SUPPORT_FOUND:
                        currentState = PlayableState::FALLING;
                        spriteFalling.setFrame(0);
                        activeActionTimer.restart();
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
            spriteIdle.update(dt);
            break;
        case PlayableState::GETTING_TO_RUN:
            acceleration += Vec2(5 * getDirectionHorizontalMultiplier(), 0);
            spriteGettingToRun.update(dt);
            if (spriteGettingToRun.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                spriteRunning.setFrame(0);
            }
            break;
        case PlayableState::RUNNING:
            spriteRunning.update(dt);
            speed.x = RUNNING_VELOCITY * getDirectionHorizontalMultiplier();
            break;
        case PlayableState::IDLE_JUMP_START:
            spriteIdleJumpStart.update(dt);
            if (spriteIdleJumpStart.isThistLastFrame()) {
                currentState = PlayableState::IDLE_JUMP_JUMPING;
                spriteIdleJumpStart.setFrame(0);
                speed += Vec2(0, -IDLE_JUMP_INITIAL_UPWARD_VELOCITY);
            }
            break;
        case PlayableState::IDLE_JUMP_JUMPING:
            spriteIdleJumpJumping.update(dt);
            if (spriteIdleJumpJumping.isThistLastFrame()) {
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);
            }
            break;
        case PlayableState::JUMP_END:
            spriteJumpEnd.update(dt);
            if (spriteJumpEnd.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                spriteIdle.setFrame(0);
            }
            break;
        case PlayableState::STOPING_RUN:
            spriteStopingRun.update(dt);
            acceleration = Vec2(-5 * getDirectionHorizontalMultiplier(), 0);
            if (spriteStopingRun.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                spriteIdle.setFrame(0);

            }
            break;
        case PlayableState::RUNNING_JUMP_START:
            spriteRunningJumpStartJump.update(dt);
            if (spriteRunningJumpStartJump.isThistLastFrame()) {
                currentState = PlayableState::RUNNING_JUMP_JUMPING;
                spriteRunningJumpJumping.setFrame(0);
                centerPos += Vec2(0, -5);
                speed += Vec2(0, -RUNNING_JUMP_UPWARD_INITIAL_VELOCITY);
            }
            break;
        case PlayableState::FALLING:
            spriteFalling.update(dt);
            break;
        case PlayableState::RUNNING_JUMP_JUMPING:
            spriteRunningJumpJumping.update(dt);
            if (spriteRunningJumpJumping.isThistLastFrame()) {
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);
            }
            break;
        case PlayableState::RUNNING_JUMP_END:
            spriteJumpEnd.update(dt);
            if (spriteJumpEnd.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                spriteRunning.setFrame(0);
            }
            break;
        case PlayableState::TURNING:
            acceleration = Vec2(0, 0);
            spriteTurning.update(dt);
            if (spriteTurning.isThistLastFrame()) {
                currentState = PlayableState::IDLE;
                toogleDirection();
                spriteIdle.setFrame(0);

            }
            break;
        case PlayableState::TURN_RUN:
            spriteTurnRunning.update(dt);
            if (spriteTurnRunning.isThistLastFrame()) {
                currentState = PlayableState::RUNNING;
                toogleDirection();
                spriteRunning.setFrame(0);
            }
            break;
        case PlayableState::DASHING :
            spriteDashing.update(dt);
            acceleration -= ForceField::getInstance()->getForceAt(centerPos);
            speed.x = DASH_VELOCITY * getDirectionHorizontalMultiplier();
            speed.y = 0;
            if (activeActionTimer.get() > DASH_TIME) {
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);

            }
            break;
        case PlayableState::SMASHING_UPWARD :
            spriteSmashingUpward.update(dt);
            acceleration -= ForceField::getInstance()->getForceAt(centerPos);
            speed.x = 0;
            speed.y = -1 * DASH_VELOCITY;
            if (activeActionTimer.get() > DASH_TIME) {
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);
                isSmashing = false;
            }
            break;
        case PlayableState::SMASHING_FORWARD :
            spriteSmashingForward.update(dt);
            acceleration -= ForceField::getInstance()->getForceAt(centerPos);
            speed.x = DASH_VELOCITY * getDirectionHorizontalMultiplier();
            speed.y = 0;
            if (activeActionTimer.get() > DASH_TIME) {
                currentState = PlayableState::FALLING;
                spriteFalling.setFrame(0);
                isSmashing = false;
            }
            break;
        case PlayableState::WINNING :
            spriteWinning.update(dt);
            speed.x = 0;
            speed.y = -5;
            acceleration -= ForceField::getInstance()->getForceAt(centerPos);
            if (spriteWinning.isThistLastFrame()) {
                Game::getInstance().getCurrentState().requestPop();
                Game::getInstance().push(new EndState({true}));
            }
            break;
        case PlayableState::LOSING :
            spriteLosing.update(dt);
            speed.x = 0;
            speed.y = 0;
            acceleration -= ForceField::getInstance()->getForceAt(centerPos);
            if (spriteLosing.isThistLastFrame()) {
                defeated = true;
                Game::getInstance().getCurrentState().requestPop();
                Game::getInstance().push(new EndState({false}));
            }
            break;
        default:
            break;

    }

    acceleration += ForceField::getInstance()->getForceAt(centerPos);

    //integrate time equation
    speed += dt * acceleration;

    //speed maginute must be limited
    static const int maxSpeed = 800;

    if (speed.magnitude() > maxSpeed) {
        speed = Vec2::getVec2FromPolar(maxSpeed, speed.ang_rad());

    }

    updatePos(centerPos + dt * speed);

    canGrabLeft = false;
    canGrabRight = false;

}

int PlayableEmotion::getDirectionHorizontalMultiplier() const { return ((currentlyFacing == RIGHT) ? 1 : -1); }

void PlayableEmotion::toogleDirection() {
    currentlyFacing = (currentlyFacing == RIGHT) ? LEFT : RIGHT;
}

bool PlayableEmotion::isDead() {
    return defeated;
}

void PlayableEmotion::notifyCollision(GameObject &other) {
    if (currentState == LOSING || currentState == WINNING)
        return;
    else if (other.is("KillingRectangle")) {
        currentState = PlayableState::LOSING;
        spriteLosing.setFrame(0);
    }
    else if (other.is("VictoryRectangle")) {
        currentState = PlayableState::WINNING;
        spriteWinning.setFrame(0);

    }
    else if (isSmashing && other.is("DestroyableRectangle")) {
        ((DestroyableRectangle &) other).smashThis();
    }
    else if (other.is("CollidableAABBGameObject")) {
        Rect collidingRect = ((AxisAlignedBoundingBox *) other.getCollisionVolume())->axisAlignedRectangle;

        //among internal circles, find the one that is closes to the axis aligned rectangle
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
        const Vec2 closestPointOnRectangle = collidingRect.getClosestPointTo(closestInnerCircle);

        float grab_angle = (closestPointOnRectangle - centerPos).ang_rad();

        if(grab_angle >= -M_PI_2 && grab_angle <= M_PI_2){
            canGrabLeft = true;
            canGrabRight = false;
            grabableToTheLeft = &other;
            grabableToTheRight = nullptr;
        }else{
            canGrabLeft = false;
            canGrabRight = true;
            grabableToTheLeft = nullptr;
            grabableToTheRight = &other;

        }

        double overlap = auxCollisionVolume[min_k].getRadius() - distanceBetweenCircleAndRectangle;

        static double TOLERANCE = M_PI / 20.0;

        if (overlap >= 0.0) {
            Vec2 directionToEscape = (closestInnerCircle - closestPointOnRectangle).getNormalizedVector();
            updatePos(centerPos + overlap * directionToEscape);

            double angleToEscape = directionToEscape.ang_rad();

            if (abs(angleToEscape) < TOLERANCE || abs(angleToEscape - M_PI) < TOLERANCE) {
                if (iterationsSinceLastVerticalSupportNodification > 10) {
                    TyperInput::getInstance().addEventOnFront(
                            TyperInput::TypingEvent::VERTICAL_SUPPORT_FOUND);
                    iterationsSinceLastVerticalSupportNodification = 0;

                }
//                speed.x = 0;
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
            spriteRunning.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::GETTING_TO_RUN:
            spriteGettingToRun.render((int) pos.x, (int) pos.y, rotation,
                                      (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE:
            spriteIdle.render((int) pos.x, (int) pos.y, rotation,
                              (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE_JUMP_START:
            spriteIdleJumpStart.render((int) pos.x, (int) pos.y, rotation,
                                       (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::IDLE_JUMP_JUMPING:
            spriteIdleJumpJumping.render((int) pos.x, (int) pos.y, rotation,
                                         (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                   : SDL_FLIP_NONE);
            break;
        case PlayableState::JUMP_END:
            spriteJumpEnd.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_START:
            spriteRunningJumpStartJump.render((int) pos.x, (int) pos.y, rotation,
                                              (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                        : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_JUMPING:
            spriteRunningJumpJumping.render((int) pos.x, (int) pos.y, rotation,
                                            (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                      : SDL_FLIP_NONE);
            break;
        case PlayableState::RUNNING_JUMP_END:
            spriteJumpEnd.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::STOPING_RUN:
            spriteStopingRun.render((int) pos.x, (int) pos.y, rotation,
                                    (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::TURNING:
            spriteTurning.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::TURN_RUN:
            spriteTurnRunning.render((int) pos.x, (int) pos.y, rotation,
                                     (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::FALLING:
            spriteFalling.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::DASHING:
            spriteDashing.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            break;
        case PlayableState::SMASHING_FORWARD:
            spriteSmashingForward.render((int) pos.x, (int) pos.y, rotation,
                                         (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                   : SDL_FLIP_NONE);
            break;
        case PlayableState::SMASHING_UPWARD:
            spriteSmashingUpward.render((int) pos.x, (int) pos.y, rotation,
                                        (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                                  : SDL_FLIP_NONE);
            break;
        case PlayableState::WINNING:
            spriteWinning.render((int) pos.x, (int) pos.y, rotation,
                                 (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                           : SDL_FLIP_NONE);
            break;
        case PlayableState::LOSING:
            spriteLosing.render((int) pos.x, (int) pos.y, rotation,
                                (currentlyFacing == PlayableFacing::LEFT) ? SDL_FLIP_HORIZONTAL
                                                                          : SDL_FLIP_NONE);
            break;

        default:
            break;

    }

    for (unsigned k = 0; k < 3; k++)
        auxCollisionVolume[k].render();
}

void PlayableEmotion::updatePos(Vec2 center) {

    centerPos = center;

    //update axis aligned bouding box
    AxisAlignedBoundingBox *box = (AxisAlignedBoundingBox *) collisionVolume;
    box->setLeftTopCorner(centerPos + center_LT_displacement);

    //update auxliar collision circles
    const Vec2 &aux = Vec2::getVec2FromPolar(box->axisAlignedRectangle.h / 4, (float) M_PI_2);
    auxCollisionVolume[0].setCenter(centerPos + aux);
    auxCollisionVolume[1].setCenter(centerPos);
    auxCollisionVolume[2].setCenter(centerPos + aux * (-1.0));

}

void PlayableEmotion::loadSettings() {

    SettingsLoad *settings = SettingsLoad::getInstance();
    RUNNING_VELOCITY = (int) settings->get("RUNNING_VELOCITY", 80);
    IDLE_JUMP_INITIAL_UPWARD_VELOCITY = (int) settings->get("IDLE_JUMP_INITIAL_UPWARD_VELOCITY", 150);
    RUNNING_JUMP_UPWARD_INITIAL_VELOCITY = (int) settings->get("RUNNING_JUMP_UPWARD_INITIAL_VELOCITY", 180);
    DASH_TIME = 1.0 * settings->get("DASH_TIME_TENS_OF_SECONDS", 20) / 10.0;
    DASH_VELOCITY = (int) settings->get("DASH_VELOCITY", 200);
    SCALE_FACTOR_ON_MAIN_CHAR = settings->get("SCALE_ON_MAIN_CHAR", 70) / 100.0f;
    CIRCLE_VERTICAL_SPACING = settings->get("CIRCLES_SPACING", 20);
    CIRCLE_RADIUS = settings->get("CIRCLES_RADIUS", 10);


}

void PlayableEmotion::applyScaleFactor(float f) {
    GameObject::applyScaleFactor(f);

    for (int i = 0; i < 3; i++) {
        auxCollisionVolume[i].applyScale(f);
    }

    for (int i = 0; i < allSprites.size(); i++) {
        allSprites[i]->setScale(Vec2(f, f));
    }
    int width = spriteRunning.getSpriteFullWidth() / 15;
    int height = spriteRunning.getSpriteFullHeight();

    center_LT_displacement = -0.5 * Vec2(width, height);
}
