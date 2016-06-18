//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_KILLINGRECTANGLE_H
#define IDJ201601T1_KILLINGRECTANGLE_H


#include "CollidableAABBGameObject.h"

class KillingRectangle : public CollidableAABBGameObject {
private:
    Vec2 constSpeed;

public:


    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);


    virtual void render();


};


#endif //IDJ201601T1_KILLINGRECTANGLE_H
