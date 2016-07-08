//
// Created by jasf on 5/8/16.
//

#ifndef IDJ201601T1_ANIMATION_H
#define IDJ201601T1_ANIMATION_H


#include "Timer.h"
#include "Sprite.h"
#include "GameObject.h"

class Animation : public GameObject {
protected:
    Timer endTimer;
    float timeLimit;
    bool oneTimeOnly;
public:

    Sprite sp;

public:

    Animation() { }

    virtual void notifyCollision(GameObject &other);

    Animation(float centerX, float centerY, float rotation, string sprite, float timeLimit, bool ends, int frameCount,
              float frameTime);
    Animation(float centerX, float centerY, float rotation, float timeLimit, bool ends);

    virtual void update(float dt);

    virtual void render();

    bool isDead();

    void notifyCollision(GameObject *other);

    bool is(std::string type);

    virtual void restart();


};

class MatrixAnimation : public Animation {

public:
    MatrixSprite matrixSprite;

    MatrixAnimation(float centerX, float centerY, float rotation, string sprite, float timeLimit, bool ends, int qtdRows,
              int qtdCols, float frameTime);


    virtual void restart() override;

    bool isDead();

    virtual void render() override;

    virtual void update(float dt) override;
};


#endif //IDJ201601T1_ANIMATION_H
