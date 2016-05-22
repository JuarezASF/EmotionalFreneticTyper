//
// Created by jasf on 5/21/16.
//

#include "Text.h"
#include "Resources.h"
#include "Game.h"

Text::Text(std::string fontFile, int fontSize, Text::TextStyle style, std::string text, SDL_Color color, int x, int y)
        : font(Resources::getFont(fontFile, fontSize)), texture(nullptr), text(text), fontSize(fontSize), color(color),
          box(x, y, 0, 0), style(style) {

    if (font)
        remakeTexture();
    else {
        std::cerr << "Problem when initializing font!" << std::endl;
        Game::getInstance().requestQuit();
    }

}

Text::~Text() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

}

void Text::render(int cameraX, int cameraY) {
    SDL_Rect dst_rect;
    dst_rect.x = (int) box.x;
    dst_rect.y = (int) box.y;
    dst_rect.w = (int) box.w;
    dst_rect.h = (int) box.h;

    SDL_RenderCopy(Game::getInstance().getRenderer(), texture, nullptr, &dst_rect);

}

void Text::setPos(int x, int y, bool centerX, bool centerY) {

    box.x = (centerX) ? Game::getInstance().getScreenDimensions().x/2 : x;
    box.y = (centerY) ? Game::getInstance().getScreenDimensions().y/2 : y;

}

void Text::setText(std::string text) {

    this->text = text;
    remakeTexture();
}

void Text::setColor(SDL_Color color) {
    this->color = color;
    remakeTexture();

}

void Text::setStyle(Text::TextStyle style) {

    this->style = style;

    remakeTexture();

}

void Text::setFontSize(int fontSize) {
    this->fontSize = fontSize;
    remakeTexture();

}

void Text::remakeTexture() {

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface *surface;

    switch (style) {
        case TextStyle::SOLID:
            surface = TTF_RenderText_Solid(font, text.c_str(), color);
            break;
        case TextStyle::SHADED:
            surface = TTF_RenderText_Shaded(font, text.c_str(), SDL_Color{0, 0, 0, 0}, color);
            break;
        case TextStyle::BLENDED:
            surface = TTF_RenderText_Blended(font, text.c_str(), color);
            break;
        default:
            std::cerr << "Invalid style!" << std::endl;
            Game::getInstance().requestQuit();
            break;
    }

    texture = SDL_CreateTextureFromSurface(Game::getInstance().getRenderer(), surface);
    box.w = surface->w;
    box.h = surface->h;

    SDL_FreeSurface(surface);

}
