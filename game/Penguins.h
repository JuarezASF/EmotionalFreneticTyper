//
// Created by jasf on 4/23/16.
//

#ifndef IDJ201601T1_PENGUINS_H
#define IDJ201601T1_PENGUINS_H


#include "Sprite.h"
#include "Vec2.h"
#include "GameObject.h"
#include "Timer.h"
#include "Sound.h"

class Penguins : public GameObject{

protected:

    Sprite bodySp, cannonSp;
    Vec2 speed;
    float linearSpeed, cannonAngle;
    int hp;
    Vec2 penguint_LT_displacement;
    Vec2 cannon_center_LT_displacement;

    static float MAX_LIN_SPEED;
    static float MIN_LIN_SPEED;

    Timer coolDownTimer;


public:

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    Penguins(float x, float y);

    virtual ~Penguins();

    void update(float dt);

    void render();

    bool isDead();

    void shoot();

    static Penguins* player;

    void damage(int dm);

};


#endif //IDJ201601T1_PENGUINS_H
