//
// Created by jasf on 3/17/16.
//

#include <iostream>
#include "Game.h"
#include "Resources.h"
#include "Logger.h"
#include "Sprite.h"
#include "GameConfig.h"

int Sprite::getSpriteFullWidth() const {
    return spriteFullWidth * scaleX;
}

int Sprite::getSpriteFullHeight() const {
    return spriteFullHeight * scaleY;
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
    while ((SDL_QueryTexture(texture, nullptr, nullptr, &spriteFullWidth, &spriteFullHeight) < 0) && attempts_left > 0) {
        std::cerr << SDL_GetError() << std::endl;
        attempts_left--;
    }

    originalFrameWidth = spriteFullWidth / this->frameCount;
    originalframeHeight = spriteFullHeight;

    setClip(0, 0, originalFrameWidth, spriteFullHeight);
    setAlpha(255);
}

void Sprite::setClip(int x, int y, int w, int h) {
    clipRect.x = x;
    clipRect.y = y;
    clipRect.w = w;
    clipRect.h = h;
}

void Sprite::render(int topLeftX, int topLeftY, float angle, SDL_RendererFlip flip) {
    dst_rect.x = topLeftX;
    dst_rect.y = topLeftY;
    dst_rect.w = (int)(clipRect.w * scaleX);
    dst_rect.h = (int) (clipRect.h * scaleY);


    if (center) {
        SDL_Point c;
        c.x = (int)(center->x * scaleX);
        c.y = (int)(center->y * scaleY);
        SDL_RenderCopyEx(Game::getInstance().getRenderer(), texture, &clipRect, &dst_rect, angle * 180.0 / M_PI, &c,
                         flip);
    } else {
        SDL_RenderCopyEx(Game::getInstance().getRenderer(), texture, &clipRect, &dst_rect, angle * 180.0 / M_PI,
                         nullptr,
                         flip);

    }

}

void Sprite::setScaleX(float scale) {
    scaleX = scale;

}

void Sprite::setScaleY(float scale) {
    scaleY = scale;

}

void Sprite::setAlpha(unsigned char alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
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

    setAlpha(255);

}

void Sprite::update(float dt) {
    timeElapsed += dt;

    if (timeElapsed > frameTime) {
        timeElapsed = 0.0;
        currentFrame = (currentFrame + 1) % frameCount;

        updateSourceOfFrame();

    }

}

void Sprite::setFrame(int frame) {
    this->currentFrame = frame;
    if (frame >= frameCount)
        Logger::log("Frame idx greater than frame count");

    currentFrame = (frame) % frameCount;

    setClip(currentFrame * originalFrameWidth, 0, originalFrameWidth, originalframeHeight);

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
    return originalFrameWidth*scaleY;
}

Vec2 Sprite::getDimensions() const {
    return Vec2(spriteFullWidth, spriteFullHeight);
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

    originalFrameWidth = sp.spriteFullWidth;
    originalframeHeight = sp.spriteFullHeight;

    setClip(0, 0, originalFrameWidth, spriteFullHeight);
    setAlpha(255);

}

bool Sprite::isThistLastFrame() {
    return currentFrame == (frameCount - 1);
}

void Sprite::setScale(Vec2 scale) {
    setScaleX(scale.x);
    setScaleY(scale.y);
}

void Sprite::updateSourceOfFrame() {
    setClip(currentFrame * originalFrameWidth, 0, originalFrameWidth, originalframeHeight);
}


void MatrixSprite::updateSourceOfFrame() {
    currentColumns = currentFrame % qtdColumns;
    currentRow = currentFrame / qtdColumns;

    setClip(currentColumns * originalFrameWidth, currentRow * originalframeHeight, originalFrameWidth, originalframeHeight);
}


MatrixSprite::MatrixSprite(std::string filename, int qtdRows, int qtdColumns, float frameTime) {
    construct(qtdColumns * qtdRows, frameTime);
    open(filename);

    currentColumns = 0;
    currentRow = 0;
    this->qtdRows = qtdRows;
    this->qtdColumns = qtdColumns;

    int attempts_left = 5;
    while ((SDL_QueryTexture(texture, nullptr, nullptr, &spriteFullWidth, &spriteFullHeight) < 0) && attempts_left > 0) {
        std::cerr << SDL_GetError() << std::endl;
        attempts_left--;
    }

    originalFrameWidth = spriteFullWidth / this->qtdColumns;
    originalframeHeight = spriteFullHeight / this->qtdRows;

    setClip(0, 0, originalFrameWidth, originalframeHeight);

}

int Sprite::getFrameHeight() {
    return originalframeHeight * scaleY;
}
