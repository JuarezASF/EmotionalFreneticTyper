//
// Created by jasf on 3/25/16.
//

#include "TileMap.h"

TileMap::TileMap(std::string filename, TileSet *tileSet) {
    load(filename);

    this->tileSet = tileSet;
}

void TileMap::load(string file) {
    FILE *fp;
    fp = fopen(file.c_str(), "r");
    if (fp == nullptr) {
        std::cerr << "Could not open file " << file << " for tileset" << std::endl;
    }
    fscanf(fp, "%d,%d,%d,\n\n", &mapWidth, &mapHeight, &mapDepth);

    int d = 0;
    int read = -1;

    for (int k = 0; k < mapDepth; k++) {
        for (int j = 0; j < mapHeight; j++) {
            for (int i = 0; i < mapWidth; i++) {
                read = fscanf(fp, "%d,", &d);
                if (read != 1) {
                    std::cerr << "Failed to read tile index!" << std::endl;
                }
                tileMatrix.push_back(d - 1);
            }
        }
        fscanf(fp, "\n");
    }


    fclose(fp);


}

void TileMap::setTileSet(TileSet *tileSet) {

    if (this->tileSet) {
        delete tileSet;
    }
    this->tileSet = tileSet;

}

int &TileMap::at(int x, int y, int z) {
    return tileMatrix[mapWidth * mapHeight * z + mapWidth * y + x];
}

void TileMap::renderLayer(int layer, const Vec2 &cam) {
    renderLayer(layer, cam.x, cam.y);
}

void TileMap::renderLayer(int layer, int cameraX, int cameraY) {

    int tileIndex = -1;
    for (int j = 0; j < getHeight(); j++)
        for (int i = 0; i < getWidth(); i++) {
            tileIndex = at(i, j, layer);
            tileSet->render(at(i, j, layer), -cameraX + i * tileSet->getTileWidth(),
                            -cameraY + j * tileSet->getTileHeight());
        }

}

int TileMap::getWidth() {
    return mapWidth;
}

int TileMap::getHeight() {
    return mapHeight;
}

int TileMap::getDepth() {
    return mapDepth;
}

void TileMap::render(int cameraX, int cameraY) {
    for (int k = 0; k < mapDepth; k++)
        renderLayer(k, cameraX, cameraY);

}
