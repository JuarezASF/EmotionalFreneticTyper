//
// Created by jasf on 5/22/16.
//

#ifndef IDJ201601T1_PLAYBLEEMOTION_H
#define IDJ201601T1_PLAYBLEEMOTION_H


#include "GameObject.h"
#include "Sprite.h"
#include "Timer.h"

class PlayableEmotion : public GameObject{
public:
    enum PlayableState{IDLE, RUNNING, TURNING, GETTING_TO_RUN, STOPING_RUN, RUNNING_JUMP_START, RUNNING_JUMP_JUMPING,
        RUNNING_JUMP_END, IDLE_JUMP_START, IDLE_JUMP_JUMPING, JUMP_END};

    enum PlayableFacing{LEFT, RIGHT};

private:

    Sprite runnigSp, gettingToRunSp, stopingToRunSp, turningSp, idleSp;
    Sprite idleStartJump, idleJump, runningJumpSp, runningStartJumpSp, jumpEndSp;
    PlayableState currentState;
    PlayableFacing currentlyFacing;
    Vec2 speed, acceleration, previousPos;
    float speedAngle, speedMagnitude;
    float accelerationAngle, accelerationMagnitude;
    vector<pair<Vec2, Vec2>> restrictingLineSegments;

    int lifePoints;

    Timer activeActionTimer;

    bool defeated;

public:

    PlayableEmotion(int x, int y);

    virtual void clearCollisionState();

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    void render();

    void toogleDirection();
};


#endif //IDJ201601T1_PLAYBLEEMOTION_H
