//
// Created by jasf on 7/8/16.
//

#ifndef IDJ201601T1_PLATAFORMASMANAGER_H
#define IDJ201601T1_PLATAFORMASMANAGER_H


#include<string>
#include<vector>
#include<map>
#include "Sprite.h"
#include "CollidableAABBGameObject.h"

using namespace std;
class PlataformasManager {

private:
    PlataformasManager();

    map<string,tuple<string, string, int, int, int,int>> config;

public:
    static PlataformasManager *getInstance();

    Platform *getPlatformCenteredAt(Vec2 center, string name);


};


#endif //IDJ201601T1_PLATAFORMASMANAGER_H
