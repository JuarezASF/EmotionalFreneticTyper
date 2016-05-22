//
// Created by jasf on 3/25/16.
//

#ifndef IDJ201601T1_TILEMAP_H
#define IDJ201601T1_TILEMAP_H

#include <vector>
#include "TileSet.h"
#include "Vec2.h"

class TileMap {

protected:
    std::vector<int> tileMatrix;
    TileSet *tileSet;
    int mapWidth, mapHeight, mapDepth;

public:
    TileMap(string filename, TileSet* tileSet);
    void load(string file);
    void setTileSet(TileSet* tileSet);
    int& at(int x, int y, int z = 0);
    void renderLayer(int layer, int cameraX, int cameraY);
    void renderLayer(int layer, const Vec2 & cam);
    void render(int cameraX = 0, int cameraY = 0);

    int getWidth();
    int getHeight();
    int getDepth();

};


#endif //IDJ201601T1_TILEMAP_H
