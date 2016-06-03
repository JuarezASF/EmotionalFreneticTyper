//
// Created by jasf on 6/2/16.
//

#ifndef IDJ201601T1_COLLIDABLECIRCLE_H
#define IDJ201601T1_COLLIDABLECIRCLE_H


#include "Vec2.h"
#include "Collidable.h"

class CollidableCircle : public Collidable {

public:
    Vec2 center;
    float r;

    virtual bool is(string className);

    virtual void render();

    void setCenter(Vec2 p);

    inline void setRadius(float f) { r = f; }

    inline const Vec2 &getCenter(){return center;}

    inline float getRadius(){return r;}

    void moveCenter(Vec2 d){center += d;}


};


#endif //IDJ201601T1_COLLIDABLECIRCLE_H
