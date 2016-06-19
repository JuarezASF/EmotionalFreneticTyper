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
#include "defines.h"

#define TILE_HEIGHT 170
#define TILE_WIDTH 170

StageState::StageState() : bg("img/ParedesPreto.png"), tileSet(TILE_WIDTH, TILE_HEIGHT, "img/TileBrick.png"),
                           tileMap("map/tileMap.txt", &tileSet), stagePanel(250, 250), usedEmotion(),
						   startText("font/Call me maybe.ttf", 70, Text::TextStyle::BLENDED, "TYPE START", DARK_YELLOW),
						   startWait(true), showStartText(true), startTextTimer() {

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
    int x, y, w, h,q, vx, vy;

    //skip description
    is >> buffer;
    is >> x;
    is >> y;
    cout << "main player at tl:" << Vec2(x,y) << " w,h" << Vec2(w,h) << " speed:" << Vec2(vx,vy) << endl;
    mainPlayerStart = new PlayableEmotion(x, y);

    is >> buffer;
    is >> x;
    is >> y;
    is >> w;
    is >> h;
    is >> vx;
    is >> vy;

    cout << "killing rectangle tl:" << Vec2(x,y) << " w,h" << Vec2(w,h) << " speed:" << Vec2(vx,vy) << endl;
    addObject(KillingRectangle::getTopLeftAt(Vec2(x, y), w, h, Vec2(vx, vy)));

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

    Camera::follow(mainPlayerStart);

    startText.setPos(Game::getInstance().getScreenDimensions().x/2,Game::getInstance().getScreenDimensions().y/2,true,true);


}

StageState::~StageState() {
    objectArray.clear();
    if(mainPlayerStart)
    	delete mainPlayerStart;
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

    /*
>> Problema similar ao ocorrido no TitleState.cpp (dê uma olhada lá). Favor verificar como
adicionar a palavra start, de forma que ela somente funcione nesse contexto,
e não durante o jogo...
    TyperInput::TypingEvent e = im.getTypingEvent();
	if (e == TyperInput::TypingEvent::START) {
		startWait(false);
		showStartText(false);
		startTextTimer.restart();
		addObject(mainPlayerStart);
    	mainPlayerStart = nullptr;
	}*/

    if(!startWait) {
		// unless told the opposite, no one is supported
		for(unsigned k = 0; k < objectArray.size(); k++)
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
    } else {
		if(!startTextTimer.get()) {
			showStartText = !showStartText;
			startTextTimer.update(dt);
		} else {
			startTextTimer.update(dt);
			if(startTextTimer.get() >= .7)
				startTextTimer.restart();
		}
    }
    stagePanel.update(dt);
}

vector<pair<unsigned, unsigned>> StageState::checkForCollision() const {//test for collision
    vector<pair<unsigned, unsigned>> collidingPairs;

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

    if(showStartText)
    	startText.render();
}

void StageState::pause() {

}

void StageState::resume() {

}

//StageState::StateInfo StageState::getStateInfo() {
//    return StageState::StateInfo();
//}

Panel& StageState::getPanel() { return stagePanel; }
