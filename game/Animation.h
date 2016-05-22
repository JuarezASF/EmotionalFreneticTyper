//
// Created by jasf on 5/8/16.
//

#ifndef IDJ201601T1_ANIMATION_H
#define IDJ201601T1_ANIMATION_H


#include "Timer.h"
#include "Sprite.h"
#include "GameObject.h"

class Animation : public GameObject {
private:
    Timer endTimer;
    float timeLimit;
    bool oneTimeOnly;
    Sprite sp;

public:

    virtual void notifyCollision(GameObject &other);

    Animation(float x, float y, float rotation, string sprite, float timeLimit, bool ends, int frameCount,
              float frameTime);

    void update(float dt);

    void render();

    bool isDead();

    void notifyCollision(GameObject *other);

    bool is(std::string type);


};


#endif //IDJ201601T1_ANIMATION_H
