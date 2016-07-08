//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_TITLESTATE_H
#define IDJ201601T1_TITLESTATE_H


static const double PRESS_SKIP_TEST_BLINK_TIME = 0.7;

#include "State.h"
#include "Sprite.h"
#include "Timer.h"
#include "Music.h"
#include "Text.h"
#include "Animation.h"
#include <memory>
#include <stack>

class TitleState : public State {

private:

    //array of images to display on startup
    std::vector<std::unique_ptr<Sprite>> imageStack;

    //used to controle fad in/fade out transition
    unsigned char currentImgIdx;
    Timer fadeTimer;
    float alpha;
    float fadeInSpeed, fadeOutSpeed;
    bool isFadingIn;
    float secondsPerImage;
    float timeOnHold, timeOnFadeIn, timeOnFadeOut;

    typedef enum FadeState {
        FADING_IN, FADE_HOLDING, FADING_OUT
    } FadeState;

    FadeState currentFadeState;

    //music starts when a given image index is reached
    Music music;
    int startsMusicAt;

    //used to display skip text on bottom of the screen
    Text skipText;
    Timer skipTextTimer;
    bool showSkipText;

    int currentAnimation;
    vector<MatrixAnimation*> animationsToPlay;

public:

    TitleState();

    void update(float);

    void render();

    void pause();

    void resume();



};


#endif //IDJ201601T1_TITLESTATE_H
