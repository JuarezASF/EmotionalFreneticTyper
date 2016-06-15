//
// Created by jasf on 5/22/16.
//

#ifndef IDJ201601T1_PLAYBLEEMOTION_H
#define IDJ201601T1_PLAYBLEEMOTION_H


#define SECONDS_PER_FRAME 0.0833f

#include "GameObject.h"
#include "Sprite.h"
#include "Timer.h"
#include "CollidableCircle.h"


class PlayableEmotion : public GameObject{
public:
    enum PlayableState{IDLE, RUNNING, TURNING, GETTING_TO_RUN, STOPING_RUN, RUNNING_JUMP_START, RUNNING_JUMP_JUMPING,
        RUNNING_JUMP_END, IDLE_JUMP_START, IDLE_JUMP_JUMPING, JUMP_END, TURN_RUN, DASHING, FALLING};

    enum PlayableFacing{LEFT, RIGHT};

private:
    static int RUNNING_VELOCITY;
    static int IDLE_JUMP_INITIAL_UPWARD_VELOCITY;
    static int RUNNING_JUMP_UPWARD_INITIAL_VELOCITY;

    Sprite runnigSp, gettingToRunSp, stopingToRunSp, turningSp, idleSp;
    Sprite idleStartJump, idleJump, runningJumpSp, runningStartJumpSp, jumpEndSp;
    Sprite turnRunSp, dashSp, fallingSp;
    PlayableState currentState;
    PlayableFacing currentlyFacing;
    Vec2 speed, acceleration, previousPos;
    float speedAngle, speedMagnitude;
    float accelerationAngle, accelerationMagnitude;
    vector<pair<Vec2, Vec2>> restrictingLineSegments;

    int lifePoints;

    Timer activeActionTimer;

    bool defeated;

    bool wasRunning;

    CollidableCircle auxCollisionVolume[3];

public:

    PlayableEmotion(int x, int y);

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    void render();

    void toogleDirection();

    int getDirectionHorizontalMultiplier() const;

    void updatePos(Vec2 center);

    void loadSettings();
};


#endif //IDJ201601T1_PLAYBLEEMOTION_H
