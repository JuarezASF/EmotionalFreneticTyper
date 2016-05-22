//
// Created by jasf on 3/25/16.
//

#ifndef IDJ201601T1_TILESET_H
#define IDJ201601T1_TILESET_H

#include <string>
#include "Sprite.h"

class TileSet {

protected:

    Sprite tileSet;

    unsigned int rows, columns;

    int tileWidth, tileHeight;

public:
    TileSet(int tileWidth, int tileHeight, std::string file);

    void render(unsigned int index, float x, float y);

    int getTileWidth();

    int getTileHeight();

};


#endif //IDJ201601T1_TILESET_H
