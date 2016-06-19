//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLEHITBOX_H
#define IDJ201601T1_COLLIDABLEHITBOX_H


#include "Collidable.h"
#include "Rect.h"

typedef unsigned char uchar;

class AxisAlignedBoundingBox : public Collidable {

public:
    Rect axisAlignedRectangle;

    uchar colorR, colorG, colorB, colorA;
    bool shouldFill;

    virtual void render();

    virtual bool is(string className);

    AxisAlignedBoundingBox(Vec2 topLeft, int width, int heigth);

    static AxisAlignedBoundingBox getAABBCenteredAt(Vec2 c, int w, int h);
    static AxisAlignedBoundingBox getAABBTopLeftAt(Vec2 c, int w, int h);

    void setLeftTopCorner(Vec2 v);

    void setCenter(Vec2 v);

    Vec2 getCenter();

    class CollisionAvoidanceInfo{
    public:
        //move the objet in this direction to avoid collision
        Vec2 directionToAvoidCollision;

        //move this much to avoid collision
        float qtdOfOverlap;
        CollisionAvoidanceInfo(Vec2 d, float q) : directionToAvoidCollision(d), qtdOfOverlap(q){}
    };

    AxisAlignedBoundingBox::CollisionAvoidanceInfo getInfoToAvoidCollision(Vec2 circleCenter, float circleRadius);

    vector<Vec2> getNormals();

    void setColor(uchar r, uchar g, uchar b, uchar a);

    inline void setShouldFill(bool t){shouldFill = t;}


};


#endif //IDJ201601T1_COLLIDABLEHITBOX_H
