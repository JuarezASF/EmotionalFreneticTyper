//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLERECTANGLE_H
#define IDJ201601T1_COLLIDABLERECTANGLE_H


#include "GameObject.h"
#include "Sprite.h"
#include "Game.h"

/**
 * Collidable Axis Aligned Game Object
 */
class CollidableAABBGameObject : public GameObject {

protected:

    void construct(Vec2 leftTop, Vec2 bottomRight);

protected:
    CollidableAABBGameObject();

    CollidableAABBGameObject(Vec2 leftTop, Vec2 bottomRight);

public:
    static CollidableAABBGameObject *getCenteredAt(Vec2 center, int w, int h);

    static CollidableAABBGameObject *getTopLeftAt(Vec2 topLeft, int w, int h);

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);


    virtual void render();


};

class VictoryRectangle : public CollidableAABBGameObject {
public:
    VictoryRectangle(Vec2 leftTop, Vec2 bottomRight);

    static VictoryRectangle *getTopLeftAt(Vec2 topLeft, int w, int h);

    virtual bool is(std::string type);

    virtual void render();

};

class KillingRectangle : public CollidableAABBGameObject {
private:
    Vec2 constSpeed;
    Sprite borderSp, bodySp;
    Vec2 borderDimensions;

protected:
    KillingRectangle(Vec2 topLeft, std::string smokeBorderFileName, std::string smokeBodyFileName, Vec2 speed);

public:
    static KillingRectangle *getTopLeftAt(Vec2 topLeft, std::string smokeBorderFN,
                                          std::string smokeBodyFN, Vec2 speed);

    virtual void update(float dt);

    virtual bool is(std::string type);


    virtual void render() override;
};

class DestroyableRectangle : public CollidableAABBGameObject {
private:
    bool alive;

protected:
    DestroyableRectangle(Vec2 topLeft, Vec2 bottomRight);

public:
    static DestroyableRectangle *getTopLeftAt(Vec2 topLeft, int width, int heigth);

    static DestroyableRectangle *getCenteredAt(Vec2 center, int width, int height);

    virtual void update(float dt);

    virtual bool is(std::string type);

    virtual bool isDead() override;

    virtual void render() override;

    void smashThis();
};

class TrackerObject : public GameObject {
protected:
    GameObject *target;
    Vec2 offset;

public:

    TrackerObject(GameObject *target, Vec2 offset);

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);
};

class GrabableSquare : public CollidableAABBGameObject {
private:

protected:
    GrabableSquare(Vec2 topLeft, int length);

public:
    static GrabableSquare *getTopLeftAt(Vec2 topLeft, int width);

    virtual void update(float dt);

    virtual bool is(std::string type);

    virtual bool isDead() override;

    virtual void render() override;
};

#endif //IDJ201601T1_COLLIDABLERECTANGLE_H
