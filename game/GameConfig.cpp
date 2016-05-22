//
// Created by jasf on 3/25/16.
//

#include "GameConfig.h"

bool GameConfig::debugMode = false;
bool GameConfig::printHitBox = false;

void GameConfig::setDebugMode(bool b) { debugMode = b; }

void GameConfig::setPrintHitBot(bool b) {
    printHitBox = b;
}
