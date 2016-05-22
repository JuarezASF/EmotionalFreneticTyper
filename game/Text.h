//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_TEXT_H
#define IDJ201601T1_TEXT_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_render.h>
#include <string>
#include "Rect.h"

class Text {
public:
    enum TextStyle{SOLID, SHADED, BLENDED};

private:
    TTF_Font *font;
    SDL_Texture* texture;

    std::string text;
    int fontSize;
    SDL_Color color;
    Rect box;
    TextStyle style;

public:

    Text(std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, int x = 0, int y = 0);

    ~Text();

    void render(int cameraX=0, int cameraY = 0);

    void setPos(int x, int y, bool centerX = false, bool centerY = false);

    void setText(std::string text);

    void setColor(SDL_Color color);

    void setStyle(TextStyle style);

    void setFontSize(int fontSize);

private:
    void remakeTexture();



};


#endif //IDJ201601T1_TEXT_H
