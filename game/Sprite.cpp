//
// Created by jasf on 3/17/16.
//

#include <iostream>
#include "Game.h"
#include "Resources.h"
#include "Logger.h"
#include "Sprite.h"
#include <GameConfig.h>

int Sprite::getWidth() const {
    return width * scaleX;
}

int Sprite::getHeight() const {
    return height * scaleY;
}

SDL_Texture *Sprite::getTexture() {
    return texture;
}


bool Sprite::isOpen() {
    return texture != nullptr;
}

Sprite::Sprite() {
    construct(1, std::numeric_limits<float>::infinity());
}

Sprite::Sprite(string filename, int frameCount, float frameTime) {
    construct(frameCount, frameTime);
    open(filename);
}

Sprite::~Sprite() {
    //texture is now free by resources
    if (center != nullptr)
        delete center;

}

void Sprite::open(string filename) {
    texture = Resources::getImage(filename);


    int attempts_left = 5;
    while ((SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) < 0) && attempts_left > 0) {
        std::cerr << SDL_GetError() << std::endl;
        attempts_left--;
    }

    frameWidth = width / this->frameCount;
    frameHeight = height;

    setClip(0, 0, frameWidth, height);
}

void Sprite::setClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::render(int x, int y, float angle) {
    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.w = clipRect.w * scaleX;
    dst_rect.h = clipRect.h * scaleY;

    if (center) {
        SDL_Point c;
        c.x = center->x * scaleX;
        c.y = center->y * scaleY;
        SDL_RenderCopyEx(Game::getInstance().getRenderer(), texture, &clipRect, &dst_rect, angle * 180.0 / M_PI, &c,
                         SDL_FLIP_NONE);
    } else {
        SDL_RenderCopyEx(Game::getInstance().getRenderer(), texture, &clipRect, &dst_rect, angle * 180.0 / M_PI,
                         nullptr,
                         SDL_FLIP_NONE);

    }


    if (GameConfig::debugMode)
        SDL_RenderPresent(Game::getInstance().getRenderer());

}

void Sprite::setScaleX(float scale) {
    scaleX = scale;

}

void Sprite::setScaleY(float scale) {
    scaleY = scale;

}

void Sprite::construct(int frameCount, float frameTime) {
    texture = nullptr;
    scaleX = 1.0;
    scaleY = 1.0;

    currentFrame = 0;
    timeElapsed = 0.0;

    this->frameCount = frameCount;
    this->frameTime = frameTime;

    center = nullptr;

}

void Sprite::update(float dt) {
    timeElapsed += dt;

    if (timeElapsed > frameTime) {
        timeElapsed = 0.0;
        currentFrame = (currentFrame + 1) % frameCount;

        setClip(currentFrame * frameWidth, 0, frameWidth, frameHeight);

    }

}

void Sprite::setFrame(int frame) {
    this->currentFrame = frame;
    if (frame >= frameCount)
        Logger::log("Frame idx greater than frame count");

    currentFrame = (frame) % frameCount;

    setClip(currentFrame * frameWidth, 0, frameWidth, frameHeight);

}

void Sprite::incFrame() {

    setFrame((currentFrame + 1)%frameCount);

}

void Sprite::setFrameCount(int frameCount) {
    this->frameCount = frameCount;
}

void Sprite::setFrameTime(float frameTime) {
    this->frameTime = frameTime;
}

int Sprite::getFrameWidth() {
    return frameWidth;
}

Vec2 Sprite::getDimensions() const {
    return Vec2(width, height);
}

void Sprite::setCenter(Vec2 c) {
    if (center != nullptr)
        delete center;

    center = new SDL_Point();
    center->x = c.x;
    center->y = c.y;

}

Sprite::Sprite(const Sprite &sp) {
    texture = sp.texture;
    scaleX = sp.scaleX;
    scaleY = sp.scaleY;

    currentFrame = 0;
    timeElapsed = 0;

    this->frameCount = sp.frameCount;
    this->frameTime = sp.frameTime;

    this->center = sp.center;

    frameWidth = sp.width;
    frameHeight = sp.height;

    setClip(0, 0, frameWidth, height);

}
