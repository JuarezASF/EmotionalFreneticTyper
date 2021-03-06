//
// Created by jasf on 5/21/16.
//

#include <fstream>
#include "TitleState.h"
#include "TyperInput.h"
#include "defines.h"
#include "StageState.h"

TitleState::TitleState() : fadeTimer(),
                           music("audio/title_screen.ogg"),
                           skipText("font/goodfoot.ttf", 24, Text::TextStyle::BLENDED, "TYPE SKIP >>",
                                    WHITE),
                           skipTextTimer() {
    currentAnimation = 0;
    int xc, yc;
    Vec2 c = 0.5 * Game::getInstance().getScreenDimensions();
    xc = c.x;
    yc = c.y;

    animationsToPlay.push_back(
            new MatrixAnimation(xc, yc, 0, "img/intro/animation0.png", 4.0f, false, 7, 7, SECONDS_PER_FRAME));
    animationsToPlay.push_back(
            new MatrixAnimation(xc, yc, 0, "img/intro/animation1.png", 4.0f, false, 1, 6, SECONDS_PER_FRAME));
    animationsToPlay.push_back(
            new MatrixAnimation(xc, yc, 0, "img/intro/animation2.png", 4.0f, false, 1, 12, SECONDS_PER_FRAME));
    animationsToPlay.push_back(
            new MatrixAnimation(xc, yc, 0, "img/intro/animation3.png", 4.0f, false, 1, 6, SECONDS_PER_FRAME));
    for (auto p : animationsToPlay)
        p->restart();



//we'll use this to paint the screen balck on fade in/ou effects
    SDL_SetRenderDrawColor(Game::getInstance()

                                   .

                                           getRenderer(),

                           0, 0, 0, 255);
    string configFileName = "txt/openingConfig.txt";

    ifstream ifs(configFileName);

    if (!ifs.

            is_open()

            ) {
        cerr << "Cannot load opening config from " << configFileName <<
        endl;
    }

    string buffer;
    int val;
    float percentageOfTimeIn;
    float percentageOfTimeOnHold;
    float percentageOfTimeOut;

    ifs >>
    buffer;
    ifs >>
    val;
    secondsPerImage = val / 10.0f;
    ifs >>
    buffer;
    ifs >>
    val;
    percentageOfTimeIn = val / 100.0f;
    ifs >>
    buffer;
    ifs >>
    val;
    percentageOfTimeOnHold = val / 100.0f;
    ifs >>
    buffer;
    ifs >>
    val;
    percentageOfTimeOut = val / 100.0f;


    timeOnFadeIn = percentageOfTimeIn * secondsPerImage;
    timeOnHold = percentageOfTimeOnHold * secondsPerImage;
    timeOnFadeOut = percentageOfTimeOut * secondsPerImage;

    fadeInSpeed = 255.0f / timeOnFadeIn;
    fadeOutSpeed = 255.0f / timeOnFadeOut;

    cout << "configured" <<
    endl;
    cout << "\tseconds per image:" << secondsPerImage <<
    endl;
    cout << "\tseconds on fade in:" << timeOnFadeIn <<
    endl;
    cout << "\tseconds on fade hold:" << timeOnHold <<
    endl;
    cout << "\tseconds on fade out:" << timeOnFadeOut <<
    endl;

    ifs >>
    buffer;
    ifs >>
    val;
    int qtdImgsBeforeMusic = val;
    startsMusicAt = val;

    for (
            int i = 0;
            i < qtdImgsBeforeMusic;
            i++) {
        ifs >>
        buffer;
        cout << "adding img: " << buffer << " before music starts" <<
        endl;
        imageStack.emplace_back(new Sprite(buffer));
    }

    ifs >> buffer >>
    val;
    int qtdImgsAfterMusic = val;
    for (
            int i = 0;
            i < qtdImgsAfterMusic;
            i++) {
        ifs >>
        buffer;
        cout << "adding img: " << buffer << " after music starts" <<
        endl;
        imageStack.emplace_back(new
                                        Sprite(buffer)
        );
    }


    currentImgIdx = 0;
    if (startsMusicAt == currentImgIdx)
        music.play(0);

    alpha = 0.0;
    currentFadeState = FADING_IN;

    skipText.setPos((
                            int) (

                                    Game::getInstance()

                                            .

                                                    getScreenDimensions()

                                            .x - 200),
                    (int) (

                            Game::getInstance()

                                    .

                                            getScreenDimensions()

                                    .y - 80),
                    true, true);
    showSkipText = true;

    cout.

            flush();


}

void TitleState::update(float dt) {
    static TyperInput &im = TyperInput::getInstance();

    animationsToPlay[currentAnimation]->update(dt);
    if (animationsToPlay[currentAnimation]->isDead()) {
        currentAnimation = (currentAnimation + 1) % animationsToPlay.size();
        animationsToPlay[currentAnimation]->restart();
    }

//    if (animationsToPlay.isDead()) {
//        animationsToPlay.restart();
//
//    }

    quitRequested = im.getQuitRequested();

    fadeTimer.update(dt);

    float currenTime = fadeTimer.get();

    switch (currentFadeState) {
        case FADING_IN:
            alpha += fadeInSpeed * dt;
            if (currenTime > timeOnFadeIn) {
                alpha = 255;
                currentFadeState = FADE_HOLDING;
                fadeTimer.restart();
            }
            break;
        case FADE_HOLDING:
            if (currenTime > timeOnHold) {
                currentFadeState = FADING_OUT;
                fadeTimer.restart();
            }
            break;
        case FADING_OUT:
            alpha -= fadeOutSpeed * dt;
            if (currenTime > timeOnFadeOut) {
                alpha = 0;
                currentFadeState = FADING_IN;
                fadeTimer.restart();

                currentImgIdx++;
                if (startsMusicAt == currentImgIdx){
                    music.play(0);

                }

                if (currentImgIdx >= imageStack.size()) {
                    Game::getInstance().push(new StageState());
                    currentFadeState = FADE_HOLDING;
                }
            }
            break;
        default:
            break;
    }

    if (currentImgIdx < imageStack.size())
        imageStack[currentImgIdx]->setAlpha(alpha);

    skipTextTimer.update(dt);
    if (skipTextTimer.get() >= PRESS_SKIP_TEST_BLINK_TIME) {
        showSkipText = !showSkipText;
        skipTextTimer.restart();
    }

    //test if we should skip intro
    if (im.hasTypintEvent()) {
        TyperInput::TypingEvent e = im.getTypingEvent();
        if (e == TyperInput::TypingEvent::SKIP) {
            //if (!music.isPlaying())
                //music.play(-1);
            Game::getInstance().push(new StageState());
        }

    }
}

void TitleState::render() {
    if (currentImgIdx < imageStack.size()) {
        SDL_Renderer *renderer = Game::getInstance().getRenderer();
        SDL_RenderClear(renderer);
        unique_ptr<Sprite> &currentImgPtr = imageStack[currentImgIdx];
        const Vec2 &screenDimensions = Game::getInstance().getScreenDimensions();
        currentImgPtr->render((int) (screenDimensions.x / 2 - currentImgPtr->getSpriteFullWidth() / 2),
                              (int) (screenDimensions.y / 2 - currentImgPtr->getSpriteFullHeight() / 2),
                              0, SDL_FLIP_NONE);
        if (showSkipText)
            skipText.render();

        if(currentImgIdx >= startsMusicAt)
            animationsToPlay[currentAnimation]->render();
    }

}

void TitleState::pause() { }

void TitleState::resume() { }
