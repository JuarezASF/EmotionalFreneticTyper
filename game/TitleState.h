//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_TITLESTATE_H
#define IDJ201601T1_TITLESTATE_H


#include "State.h"
#include "Sprite.h"
#include "Timer.h"
#include "Music.h"
#include "Text.h"
#include <memory>
#include <stack>

class TitleState : public State {

private:
    std::stack<std::unique_ptr<Sprite>> imageStack;
    unsigned char alpha;
    unsigned char fadeSpeed;
    bool isFadingIn;
    float opaqueTime;
    Timer opaqueTimer;
    Music music;
    unsigned char startsMusicAt;
    unsigned char imageNumber;
    Text skipText;
	bool showSkipText;
	Timer skipTextTimer;

public:

    TitleState();

    void update(float);

    void render();

    void pause();

    void resume();


};


#endif //IDJ201601T1_TITLESTATE_H
