//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_KILLINGRECTANGLE_H
#define IDJ201601T1_KILLINGRECTANGLE_H


#include "CollidableAABBGameObject.h"

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


#endif //IDJ201601T1_KILLINGRECTANGLE_H
