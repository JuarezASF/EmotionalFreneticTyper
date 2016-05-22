//
// Created by jasf on 4/21/16.
//

#ifndef IDJ201601T1_MINION_H
#define IDJ201601T1_MINION_H


#include "GameObject.h"
#include "Sprite.h"

class Minion : public GameObject {

private:
    GameObject *center;
    Sprite sp;
    float arc;
    float distance_from_center;
    float angular_velocity;

public :

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    Minion(GameObject* minionCenter, float arcOffset = 0);

    void update(float dt);

    void render();

    bool isDead();

    void shoot(Vec2 pos);

};


#endif //IDJ201601T1_MINION_H
