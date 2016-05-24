//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLEHITBOX_H
#define IDJ201601T1_COLLIDABLEHITBOX_H


#include "Collidable.h"
#include "Rect.h"

class CollidableBox : public Collidable {

public:
    Rect box;
    float rotation;

    virtual void render();

    virtual bool is(string className);

    CollidableBox(Vec2 topLeft, int width, int heigth);

    inline void setRotation(float f) { rotation = f; }

    void setLT(Vec2 v);
};


#endif //IDJ201601T1_COLLIDABLEHITBOX_H
