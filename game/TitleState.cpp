//
// Created by jasf on 5/21/16.
//

#include "TitleState.h"
#include "TyperInput.h"
#include "defines.h"
#include "StageState.h"
#include "Game.h"

//A abertura eh regulada atraves de 3 variaveis cujos valores s�o atribu�dos no construtor:
//- fadeSpeed (entre 5 e 15): qtdade de tons de fade que s�o pulados a cada quadro - 0 eh transparente, 255 eh opaco.
//- opaqueTime (entre 0.5 e 2): tempo em segundos que a imagem fica totalmente opaca (para usu�rio poder ler)
//- startsMusicAt (entre 1 e quantidade de imagens): numero da imagem a partir da qual comeca a tocar a musica
TitleState::TitleState() : alpha(0), fadeSpeed(8), isFadingIn(true), opaqueTime(1.5), opaqueTimer(),
                           music("audio/Title Screen.ogg"), startsMusicAt(3), imageNumber(1),
                           skipText("font/Call me maybe.ttf", 20, Text::TextStyle::BLENDED, "PRESS ENTER TO SKIP >>",
                                    DARK_YELLOW),
                           showSkipText(true), skipTextTimer() {
    imageStack.emplace(new Sprite("img/panel.png"));
    imageStack.emplace(new Sprite("img/moveJump_Start.png"));
    imageStack.emplace(new Sprite("img/sprites_falling.png"));
    imageStack.emplace(new Sprite("img/Sprite_Run.png"));
    imageStack.emplace(new Sprite("img/Jump_End.png"));
    //Pode colocar quantas imagens forem necessarias.
    //As primeiras a aparecerem sao as ultimas a serem adicionadas.
    if (startsMusicAt == imageNumber)
        music.play(-1);
    skipText.setPos(Game::getInstance().getScreenDimensions().x - 200, Game::getInstance().getScreenDimensions().y - 80,
                    true, true);
}

void TitleState::update(float dt) {
    TyperInput &im = TyperInput::getInstance();
    quitRequested = im.getQuitRequested();
    if (!opaqueTimer.get()) {
        if (isFadingIn) {
            if (alpha < ALPHA_OPAQUE - fadeSpeed * 1.5) {
                alpha += fadeSpeed;
                imageStack.top()->setAlpha(alpha);
            } else {
                opaqueTimer.update(dt);
            }
        } else {
            if (alpha > fadeSpeed * 1.5) {
                alpha -= fadeSpeed;
                imageStack.top()->setAlpha(alpha);
            } else {
                imageStack.top()->setAlpha(ALPHA_OPAQUE);
                imageStack.pop();
                imageNumber++;
                if (startsMusicAt == imageNumber)
                    music.play(-1);
                if (imageStack.empty()) {
                    Game::getInstance().push(new StageState());
                } else {
                    isFadingIn = true;
                    imageStack.top()->setAlpha(alpha);
                }
            }
        }
    } else {
        if (opaqueTimer.get() < opaqueTime) {
            opaqueTimer.update(dt);
        } else {
            opaqueTimer.restart();
            isFadingIn = false;
        }
    }

    if (!skipTextTimer.get()) {
        showSkipText = !showSkipText;
        skipTextTimer.update(dt);
    } else {
        skipTextTimer.update(dt);
        if (skipTextTimer.get() >= .7)
            skipTextTimer.restart();
    }

    if (im.hasTypintEvent()) {
        TyperInput::TypingEvent e = im.getTypingEvent();
        if (e == TyperInput::TypingEvent::SKIP) {
            if (startsMusicAt < imageNumber)
                music.play(-1);
            Game::getInstance().push(new StageState());
            std::stack<std::unique_ptr<Sprite>>().swap(imageStack);
        }

    }
}

void TitleState::render() {
    SDL_SetRenderDrawColor(Game::getInstance().getRenderer(), 0, 0, 0, 255);
    SDL_RenderClear(Game::getInstance().getRenderer());
    if (!imageStack.empty())
        imageStack.top()->render(Game::getInstance().getScreenDimensions().x / 2 - imageStack.top()->getWidth() / 2,
                                 Game::getInstance().getScreenDimensions().y / 2 - imageStack.top()->getHeight() / 2, 0,
                                 SDL_FLIP_NONE);
    if (showSkipText)
        skipText.render();
}

void TitleState::pause() { }

void TitleState::resume() { }
