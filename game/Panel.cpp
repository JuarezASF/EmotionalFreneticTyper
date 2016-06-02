//
// Created by jasf on 5/25/16.
//

#include "Panel.h"
#include "Game.h"
#include "StageState.h"
#include "TyperInput.h"

Panel::Panel(int leftWidth, int rightWidth)
: bg("img/panel.png"), leftWidth(leftWidth), rightWidth(rightWidth),
  worda("font/Call me maybe.ttf", 40, Text::TextStyle::BLENDED, "inicioA", SDL_Color{255, 0, 0, 255}, 0, 0),
  wordb("font/Call me maybe.ttf", 40, Text::TextStyle::BLENDED, "inicioB", SDL_Color{255, 0, 0, 255}, 0, 0),
  wordc("font/Call me maybe.ttf", 40, Text::TextStyle::BLENDED, "inicioC", SDL_Color{0, 0, 0, 255}, 0, 0),
  printa(true), printb(true), printc(true){
	Vec2 windowSize = Game::getInstance().getScreenDimensions();
	worda.setPos(windowSize.x - rightWidth + 20, windowSize.y/2 - 300, false, false);
	wordb.setPos(windowSize.x - rightWidth + 20, windowSize.y/2 - 150, false, false);
	wordc.setPos(windowSize.x - rightWidth + 20, windowSize.y/2, false, false);
}

void Panel::update(float dt) {
	printa = printb = printc = false;
	std::string inputBuf = TyperInput::getInstance().getBuffer();
	if(!inputBuf.empty()) {
		wordc.setText(TyperInput::getInstance().getBuffer());
		printc = true;
	}
	vector<string> recentlyUsedWords = ((StageState&) Game::getInstance().getCurrentState()).recentlyUsedWords;
	if(!recentlyUsedWords.empty()) {
    	int i = recentlyUsedWords.size();
    	if(!recentlyUsedWords[i-1].empty()) {
			wordb.setText(recentlyUsedWords[i-1]);
			printb = true;
    	}
    	if(i > 1) {
    		if(!recentlyUsedWords[i-2].empty()) {
    			worda.setText(recentlyUsedWords[i-2]);
				printa = true;
    		}
    	}
    }
}

void Panel::render() {
	bg.setClip(0,0,rightWidth,bg.getHeight());
	bg.render(0, 0, 0, (SDL_FLIP_NONE));
	bg.setClip(bg.getWidth() - rightWidth,0,rightWidth,bg.getHeight());
	bg.render(Game::getInstance().getScreenDimensions().x - rightWidth, 0, 0, (SDL_FLIP_NONE));
	if(printa)
		worda.render();
	if(printb)
		wordb.render();
	if(printc)
		wordc.render();
}

int Panel::GetLeftWidth() { return leftWidth; }

int Panel::GetRightWidth() { return rightWidth; }
