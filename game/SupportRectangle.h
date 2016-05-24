//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLERECTANGLE_H
#define IDJ201601T1_COLLIDABLERECTANGLE_H


#include "GameObject.h"

class SupportRectangle : public GameObject{

private:

    void construct(Vec2 leftTop, Vec2 bottomRight);

public:
    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    SupportRectangle(Vec2 leftTop, Vec2 bottomRight);
    SupportRectangle(Vec2 leftTop, int width, int height);

    virtual void render();


};


#endif //IDJ201601T1_COLLIDABLERECTANGLE_H
