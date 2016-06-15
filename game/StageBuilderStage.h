//
// Created by jasf on 6/6/16.
//

#ifndef IDJ201601T1_STAGEBUILDERSTAGE_H
#define IDJ201601T1_STAGEBUILDERSTAGE_H


#include "State.h"

class StageBuilderStage : public State{


public:
    virtual void update(float dt) override;

    virtual void render() override;

    virtual void pause() override;

    virtual void resume() override;

    virtual void updateArray(float dt) override;

    virtual void renderArray() override;
};


#endif //IDJ201601T1_STAGEBUILDERSTAGE_H
