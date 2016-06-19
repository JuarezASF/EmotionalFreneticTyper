//
// Created by jasf on 3/16/16.
//

#include <fstream>
#include "Game.h"
#include "Util.h"
#include "InputManager.h"
#include "Camera.h"
#include "Collision.h"
#include "StageState.h"
#include "PlayableEmotion.h"
#include "TyperInput.h"
#include "CollidableAABBGameObject.h"
#include "KillingRectangle.h"

#define TILE_HEIGHT 170
#define TILE_WIDTH 170

StageState::StageState() : bg("img/ParedesPreto.png"), tileSet(TILE_WIDTH, TILE_HEIGHT, "img/TileBrick.png"),
                           tileMap("map/tileMap.txt", &tileSet), stagePanel(250, 250), usedEmotion() {

    //config file
    string filename = "txt/stage1config.txt";

    ifstream is(filename);

    if(!is.is_open()){
        cerr << "could not load file:" << filename << endl;
    }
    else{
        cout << "loading stage 1 from" << filename << endl;
    }

    string buffer;
    int x, y, w, h,q;

    //skip description
    is >> buffer;
    is >> x;
    is >> y;
    GameObject *mainPlayer = new PlayableEmotion(x, y);
    addObject(mainPlayer);

    Camera::follow(mainPlayer);

    //skip description
    is >> buffer;
    //read qtd rectangles
    is >> q;

    cout << "reading: " << q << " simple rectangles" << endl;

    for(int k = 0; k < q; k++){
        is >> x;
        is >> y;
        is >> w;
        is >> h;
        cout << "adding rectanble at " << Vec2(x,y) << " w,h:" << Vec2(w,h) << endl;

        addObject(CollidableAABBGameObject::getTopLeftAt(Vec2(x, y ), w, h));

        cout.flush();

    }





}

StageState::~StageState() {
    objectArray.clear();

}

Sprite StageState::getBg() {
    return bg;
}


void StageState::update(float dt) {
    TyperInput &im = TyperInput::getInstance();

    if (im.getQuitRequested())
    	quitRequested = true;
        //popRequested = true;

    Camera::update(dt);

    // unless told the opposite, no one is supported
    for(uint k = 0; k < objectArray.size(); k++)
        objectArray[k]->clearSupported();

    auto collidingPairs = checkForCollision();

    updateArray(dt);

    for(auto it : collidingPairs){
        objectArray[it.first]->notifyCollision(*objectArray[it.second]);
        objectArray[it.second]->notifyCollision(*objectArray[it.first]);
    }



    std::vector<int> toBeDeleted;
    //collect index to all objects that need to be deleted
    for (unsigned int i = 0; i < objectArray.size(); i++)
        if (objectArray[i]->isDead())
            toBeDeleted.push_back(i);
    //delete them
    auto objArrayBegin = objectArray.begin();
    for (auto it = toBeDeleted.rbegin(); it != toBeDeleted.rend(); ++it)
        objectArray.erase(objArrayBegin + *it);

    stagePanel.update(dt);
}

typedef unsigned long ulong;

vector<pair<uint, uint>> StageState::checkForCollision() const {//test for collision
    vector<pair<uint, uint>> collidingPairs;

    for (long i = objectArray.size() - 1; i >= 0; i--) {
        if (objectArray[i]->is("Animation"))
            continue;
        for (int j = 0; j < i; j++) {
            if (objectArray[j]->is("Animation"))
                continue;
            if (Collision::IsColliding(objectArray[i]->getCollisionVolume(), objectArray[j]->getCollisionVolume()
            )) {
                collidingPairs.push_back(make_pair(i, j));

            }

        }

    }

    return collidingPairs;
}

void StageState::render() {
    bg.render(0, 0, 0, (SDL_FLIP_NONE));

    tileMap.renderLayer(0, Camera::getPos(Camera::PLAYER_GROUND_VIEW));

    renderArray();

    stagePanel.render();
}

void StageState::pause() {

}

void StageState::resume() {

}

//StageState::StateInfo StageState::getStateInfo() {
//    return StageState::StateInfo();
//}

Panel& StageState::getPanel() { return stagePanel; }
