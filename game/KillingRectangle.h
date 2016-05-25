//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_KILLINGRECTANGLE_H
#define IDJ201601T1_KILLINGRECTANGLE_H


#include "SupportRectangle.h"

class KillingRectangle : public SupportRectangle{
private:
    Vec2 constSpeed;

public:

    KillingRectangle(Vec2 cPos, int w, int h, Vec2 speed);

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);


    virtual void render();


};


#endif //IDJ201601T1_KILLINGRECTANGLE_H
