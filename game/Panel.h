//
// Created by jasf on 5/25/16.
//

#ifndef IDJ201601T1_PANEL_H
#define IDJ201601T1_PANEL_H

#include "Sprite.h"
#include "Text.h"

class Panel {
public:
    Panel(int leftWidth, int rightWidth);
    void update(float dt);
    void render();
    int GetLeftWidth();
    int GetRightWidth();
private:
    Sprite panelL, panelR, iconL1, iconL2, iconL3, iconL4, iconR1, iconR2, iconR3, iconR4;
    int panelRxPosition;
    int leftWidth, rightWidth;
    Text typingWord, typedWord, iconTextR1, iconTextR2, iconTextR3, iconTextR4, iconTextL1, iconTextL2, iconTextL3, iconTextL4;
    unsigned previousRecentlyTypedWordSize;
    std::string lastTypingWord;
    bool printTypingWord, printTypedWord, printIconTextR1, printIconTextR2, printIconTextR3, printIconTextR4, printIconTextL1, printIconTextL2, printIconTextL3, printIconTextL4;
    unsigned char typedWordAlpha;
};


#endif //IDJ201601T1_PANEL_H
