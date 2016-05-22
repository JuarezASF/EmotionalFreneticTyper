//
// Created by jasf on 3/17/16.
//

#ifndef IDJ201601T1_SPRITE_H
#define IDJ201601T1_SPRITE_H


#include <string>
#include <SDL_rect.h>
#include <SDL_render.h>

using namespace std;

#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"

class Sprite {

protected:

    int width, height;
    //clipping rectangle: determine part of the image that is rendered
    SDL_Rect clipRect, dst_rect;
    //the image itself
    SDL_Texture *texture;
    SDL_Point *center;

    float scaleX, scaleY;

    int frameCount, currentFrame;
    float timeElapsed, frameTime;
    int frameWidth, frameHeight;


    void construct(int frameCount, float frameTime);

public:

    Sprite();

    Sprite(const Sprite &sp);

    Sprite(string filename, int frameCount = 1, float frameTime = 1.0);

    void setCenter(Vec2 c);

    virtual ~Sprite();

    void open(string filename);

    void setClip(int x, int y, int w, int h);

    void render(int x, int y, float angle);

    bool isOpen();

    int getWidth() const;

    int getHeight() const;

    Vec2 getDimensions() const;

    SDL_Texture *getTexture();

    void setScaleX(float scale);

    void setScaleY(float scale);

    void update(float dt);
    void setFrame(int frame);
    void incFrame();


    void setFrameCount(int frameCount);
    void setFrameTime(float frameTime);

    int getFrameWidth();



};


#endif //IDJ201601T1_SPRITE_H
