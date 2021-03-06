//
// Created by jasf on 3/25/16.
//

#include "TileSet.h"
#include "Vec2.h"
#include <iostream>
#include <unistd.h>

TileSet::TileSet(int tileWidth, int tileHeight, std::string file) : tileWidth(tileWidth), tileHeight(tileHeight) {
    tileSet.open(file);

    //TODO test if open was successfull
    int totalHeight = tileSet.getSpriteFullHeight();
    int totalWidth = tileSet.getSpriteFullWidth();

    columns = totalWidth / tileWidth;
    rows = totalHeight / tileHeight;

    cout << "Loading tileset with " << columns << " columns and " << rows << "rows" << endl;

}

void TileSet::render(unsigned int index, float x, float y) {

    if (index < columns * rows && (index >> 31) == 0) {
        int row = index / columns;
        int col = index % columns;
        Vec2 tileLT = Vec2(tileWidth * col, tileHeight * row);

        tileSet.setClip(tileLT.x, tileLT.y, tileWidth, tileHeight);
        tileSet.render(x, y, 0, (SDL_FLIP_NONE));

    }
    else {
        if(index != (unsigned int)-1)
            std::cerr << "Invalid tile index " << index << "!" << std::endl;
    }

}

int TileSet::getTileWidth() {
    return tileWidth;
}

int TileSet::getTileHeight() {
    return tileHeight;
}
