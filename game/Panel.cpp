//
// Created by jasf on 5/25/16.
//

#include "Panel.h"
#include "Game.h"
#include "StageState.h"
#include "TyperInput.h"
#include "defines.h"

Panel::Panel(int leftWidth, int rightWidth)
: panelL("img/panelL.jpg"),panelR("img/panelR.jpg"), iconL1("img/iconDash.png"), iconL2("img/iconL2.jpg"),
  iconL3("img/iconL3.jpg"), iconL4("img/iconL4.jpg"), iconR1("img/iconStop.png"), iconR2("img/iconWalk.png"),
  iconR3("img/iconJump.png"), iconR4("img/iconTurn.png"), leftWidth(leftWidth), rightWidth(rightWidth),
  typingWord("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "typingWord", WHITE),
  typedWord("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "typedWord", ORANGE_YELLOW),
  iconTextR1("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "dash", WHITE, 165, 280),
  iconTextR2("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "skill1", WHITE, 165, 350),
  iconTextR3("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "skill2", WHITE, 165, 420),
  iconTextR4("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "skill3", WHITE, 165, 490),
  iconTextL1("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "stop", WHITE, 1150, 280),
  iconTextL2("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "run", WHITE, 1150, 350),
  iconTextL3("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "jump", WHITE, 1150, 420),
  iconTextL4("font/goodfoot.ttf", 30, Text::TextStyle::BLENDED, "turn", WHITE, 1150, 490),
  previousRecentlyTypedWordSize(),lastTypingWord(),printTypingWord(),printTypedWord(),
  printIconTextR1(),printIconTextR2(),printIconTextR3(),printIconTextR4(),printIconTextL1(),
  printIconTextL2(),printIconTextL3(),printIconTextL4(),typedWordAlpha(){
	panelL.setClip(0,0,leftWidth, panelL.getSpriteFullHeight());
	panelR.setClip(0,0,rightWidth, panelR.getSpriteFullHeight());
	panelRxPosition = Game::getInstance().getScreenDimensions().x - rightWidth;
	iconL1.setAlpha(50);
	iconL2.setAlpha(50);
	iconL3.setAlpha(50);
	iconL4.setAlpha(50);
	iconR1.setAlpha(50);
	iconR2.setAlpha(50);
	iconR3.setAlpha(50);
	iconR4.setAlpha(50);
}

void Panel::update(float dt) {
	printTypingWord = false;
	if(!TyperInput::getInstance().getBuffer().empty()) {
		if(lastTypingWord != TyperInput::getInstance().getBuffer()) {
			lastTypingWord = TyperInput::getInstance().getBuffer();
			typingWord.setText(lastTypingWord);
			typingWord.setPos(Game::getInstance().getScreenDimensions().x/2,50,true,false);
		}
		printTypingWord = true;
	}
	if(!TyperInput::getInstance().getRecentlyTypedWord().empty() &&
			(TyperInput::getInstance().getRecentlyTypedWord().size() > previousRecentlyTypedWordSize)) {
		typedWord.setText(TyperInput::getInstance().getRecentlyTypedWord().back());
		typedWord.setPos(Game::getInstance().getScreenDimensions().x/2,50, true, false);
		typedWordAlpha = 255;
		printTypedWord = true;
		switch(TyperInput::getInstance().peakTypingEvent()) {
			case TyperInput::DASH:
				iconL1.setAlpha(ALPHA_OPAQUE);
				printIconTextR1 = true;
				break;
			case TyperInput::STOP:
				iconR1.setAlpha(ALPHA_OPAQUE);
				printIconTextL1 = true;
				break;
			case TyperInput::RUN:
				iconR2.setAlpha(ALPHA_OPAQUE);
				printIconTextL2 = true;
				break;
			case TyperInput::JUMP:
				iconR3.setAlpha(ALPHA_OPAQUE);
				printIconTextL3 = true;
				break;
			case TyperInput::TURN:
				iconR4.setAlpha(ALPHA_OPAQUE);
				printIconTextL4 = true;
				break;
	        default:
	            break;
		}
		if(TyperInput::getInstance().getRecentlyTypedWord().size() > 10)
			TyperInput::getInstance().flushTypedValidWords();
		previousRecentlyTypedWordSize = TyperInput::getInstance().getRecentlyTypedWord().size();
	}
	if(printTypedWord) {
		typedWordAlpha -= 7;
		if (typedWordAlpha > 7)
			typedWord.setAlpha(typedWordAlpha);
		else
			printTypedWord = false;
	}
}

void Panel::render() {
	panelL.render(0, 0, 0, (SDL_FLIP_NONE));
	panelR.render(panelRxPosition, 0, 0, (SDL_FLIP_NONE));
	iconL1.render(230, 260, 0, (SDL_FLIP_NONE));
	iconL2.render(230, 330, 0, (SDL_FLIP_NONE));
	iconL3.render(230, 400, 0, (SDL_FLIP_NONE));
	iconL4.render(230, 470, 0, (SDL_FLIP_NONE));
	iconR1.render(1083, 260, 0, (SDL_FLIP_NONE));
	iconR2.render(1083, 330, 0, (SDL_FLIP_NONE));
	iconR3.render(1083, 400, 0, (SDL_FLIP_NONE));
	iconR4.render(1083, 470, 0, (SDL_FLIP_NONE));
	if(printIconTextR1)
		iconTextR1.render();
	if(printIconTextR2)
		iconTextR2.render();
	if(printIconTextR3)
		iconTextR3.render();
	if(printIconTextR4)
		iconTextR4.render();
	if(printIconTextL1)
		iconTextL1.render();
	if(printIconTextL2)
		iconTextL2.render();
	if(printIconTextL3)
		iconTextL3.render();
	if(printIconTextL4)
		iconTextL4.render();
	if(printTypedWord)
		typedWord.render();
	if(printTypingWord)
		typingWord.render();
}

int Panel::GetLeftWidth() { return leftWidth; }

int Panel::GetRightWidth() { return rightWidth; }
