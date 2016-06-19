//
// Created by jasf on 5/24/16.
//

#ifndef IDJ201601T1_COLLIDABLERECTANGLE_H
#define IDJ201601T1_COLLIDABLERECTANGLE_H


#include "GameObject.h"

/**
 * Collidable Axis Aligned Game Object
 */
class CollidableAABBGameObject : public GameObject {

private:

    void construct(Vec2 leftTop, Vec2 bottomRight);

protected:
    CollidableAABBGameObject();

    CollidableAABBGameObject(Vec2 leftTop, Vec2 bottomRight);

public:
    static CollidableAABBGameObject * getCenteredAt(Vec2 center, int w, int h);
    static CollidableAABBGameObject * getTopLeftAt(Vec2 topLeft, int w, int h);

    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);


    virtual void render();


};

class VictoryRectangle : public CollidableAABBGameObject {
public:
    VictoryRectangle(Vec2 leftTop, Vec2 bottomRight);

    virtual bool is(std::string type);

    virtual void render();

};

class KillingRectangle : public CollidableAABBGameObject {
private:
    Vec2 constSpeed;

protected:
    KillingRectangle(Vec2 topLeft, Vec2 bottomRight, Vec2 speed);

public:
    static KillingRectangle *getTopLeftAt(Vec2 topLeft, int width, int heigth, Vec2 speed);
    static KillingRectangle *getCenteredAt(Vec2 center, int width, int height, Vec2 speed);
    virtual void update(float dt);
    virtual bool is(std::string type);


    virtual void render() override;
};

class DestroyableRectangle : public CollidableAABBGameObject {
private:
    Vec2 constSpeed;

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

#endif //IDJ201601T1_COLLIDABLERECTANGLE_H
