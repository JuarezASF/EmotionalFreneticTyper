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
    Sprite bg;
    int leftWidth, rightWidth;
    Text worda, wordb, wordc;
    bool printa, printb, printc;
};


#endif //IDJ201601T1_PANEL_H
