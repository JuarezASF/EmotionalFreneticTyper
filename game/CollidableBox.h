//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLEHITBOX_H
#define IDJ201601T1_COLLIDABLEHITBOX_H


#include "Collidable.h"
#include "Rect.h"

typedef unsigned char uchar;

class CollidableBox : public Collidable {

public:
    Rect box;
    float rotation;

    uchar colorR, colorG, colorB, colorA;

    virtual void render();

    virtual bool is(string className);

    CollidableBox(Vec2 topLeft, int width, int heigth, float rotation=0.0);

    inline void setRotation(float f) { rotation = f; box.setRotation(f); }

    void setLT(Vec2 v);

    Vec2 getCenter();

    Vec2 getClosestNormalToPoint(Vec2 p);

    class CollisionAvoidanceInfo{
    public:
        Vec2 direction;
        float qtd;
        int dirIdx;
        CollisionAvoidanceInfo(Vec2 d, float q, int i) : direction(d), qtd(q), dirIdx(i){}
    };

    CollidableBox::CollisionAvoidanceInfo getInfoToAvoidCollision(Vec2 p, float r);

    vector<Vec2> getNormals();

    void setColor(int r, int g, int b, int a);


};


#endif //IDJ201601T1_COLLIDABLEHITBOX_H
