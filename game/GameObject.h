//
// Created by jasf on 3/23/16.
//

#ifndef IDJ201601T1_GAMEOBJECT_H
#define IDJ201601T1_GAMEOBJECT_H

#include "Rect.h"
#include "Sound.h"

class GameObject {
protected:
    Rect box;

    //center pos
    Vec2 pos;
    float rotation;

    // pos + LT_center_displacemente = LEFT_TOP corner
    Vec2 center_LT_displacement;

    void constuct();

    Sound dyingSound;

public:
    GameObject();

    virtual ~GameObject();

    virtual void update(float dt) = 0;

    virtual void render();

    virtual bool isDead() = 0;

    inline Rect &getBox() { return box; }

    inline Vec2 getCenterPos() { return pos; }

    inline float getRotation() { return rotation; }

    virtual void notifyCollision(GameObject &other) = 0;

    virtual bool is(std::string type) = 0;



};


#endif //IDJ201601T1_GAMEOBJECT_H
