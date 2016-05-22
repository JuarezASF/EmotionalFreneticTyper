//
// Created by jasf on 5/22/16.
//

#ifndef IDJ201601T1_PLAYBLEEMOTION_H
#define IDJ201601T1_PLAYBLEEMOTION_H


#include "GameObject.h"
#include "Sprite.h"

class PlaybleEmotion : public GameObject{

    Sprite sp;
    int lifePoints;

public:

    PlaybleEmotion();


    virtual void update(float dt);

    virtual bool isDead();

    virtual void notifyCollision(GameObject &other);

    virtual bool is(std::string type);

    virtual void render();
};


#endif //IDJ201601T1_PLAYBLEEMOTION_H
