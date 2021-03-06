//
// Created by jasf on 3/23/16.
//

#ifndef IDJ201601T1_GAMEOBJECT_H
#define IDJ201601T1_GAMEOBJECT_H

#include "Rect.h"
#include "Sound.h"
#include "Collidable.h"

class GameObject {
protected:
    Collidable* collisionVolume;

    //center pos
    Vec2 centerPos;
    float rotation;
    bool supported;

    bool skipCollision;

    float currentScaleFactor;

    // pos + LT_center_displacemente = LEFT_TOP corner
    Vec2 center_LT_displacement;

    void construct();

public:
    GameObject();

    virtual ~GameObject();

    virtual void update(float dt) = 0;

    virtual void render();

    virtual bool isDead() = 0;

    inline Collidable *getCollisionVolume() { return collisionVolume; }

    inline Vec2 getCenterPos() { return centerPos; }

    inline float getRotation() { return rotation; }

    virtual void notifyCollision(GameObject &other) = 0;

    virtual void notifySupported();

    virtual void clearSupported();

    virtual bool is(std::string type) = 0;

    virtual void applyScaleFactor(float f);

    virtual void renderForeground();
    virtual void renderBackground();

    virtual bool isForeground();
    virtual bool isBackground();
    virtual bool isPlayerGround();

};


#endif //IDJ201601T1_GAMEOBJECT_H
