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


#endif //IDJ201601T1_COLLIDABLERECTANGLE_H
