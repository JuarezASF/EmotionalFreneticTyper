//
// Created by jasf on 4/6/16.
//

#ifndef IDJ201601T1_CAMERA_H
#define IDJ201601T1_CAMERA_H


#include "GameObject.h"

class Camera {

private:
    static GameObject *focus;
    static Vec2 speed;
    static std::vector<Vec2> paralax_pos;
    static std::vector<float> paralax_sinsitivity;

    static bool hasBenInitialized;

public:

    static unsigned BACKGROUND_VIEW;
    static unsigned PLAYER_GROUND_VIEW;
    static unsigned TOPGROUND_VIEW;

    static void setLvsOfParalax(unsigned int k, std::vector<float>);

    static void follow(GameObject *newFocus);

    static void unfollow();

    static void update(float dt);

    static Vec2 getPos(unsigned int paralax_lv);
    static inline Vec2 getSpeed(){return speed;}

    static GameObject *getPlayer();



};


#endif //IDJ201601T1_CAMERA_H
