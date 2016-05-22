//
// Created by jasf on 4/21/16.
//

#ifndef IDJ201601T1_ALIEN_H
#define IDJ201601T1_ALIEN_H


#include <queue>
#include "GameObject.h"
#include "Sprite.h"
#include "Minion.h"
#include "Timer.h"

class Alien : public GameObject {

private:

    enum AlientState{ MOVING, RESTING};

    AlientState state;

    Timer restTimer;

    Vec2 destination;

    Sprite sp;
    Vec2 speed;
    static float VELOCITY_CONST;

    static float alienCoolDownTime;

    static int alienCount;

    int hp;

    std::vector<Minion *> minionArray;
    float angular_velocity;

public:
    Alien(float x, float y, int nMinions);

    virtual bool is(std::string type);

    virtual void notifyCollision(GameObject &other);

    virtual ~Alien();

    void update(float dt);

    void render();

    bool isDead();

    void damage(int dm);

    inline static int getAlientCount(){return Alien::alienCount;}

};


#endif //IDJ201601T1_ALIEN_H
