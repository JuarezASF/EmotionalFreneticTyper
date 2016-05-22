//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_STATE_H
#define IDJ201601T1_STATE_H

#include <memory>

#include "GameObject.h"

/**
 * Interface to be implemented by every state( stage, startup screen, gameover screen and so on )
 */

class State {
protected:

    bool popRequested, quitRequested;

    std::vector <std::unique_ptr<GameObject>> objectArray;

public:

    State();

    virtual ~State();

    virtual void update(float dt) = 0;

    virtual void render() = 0;

    virtual void pause() = 0;

    virtual void resume() = 0;

    virtual void addObject(GameObject *obj);

    bool getPopRequested();

    bool getQuitRequested();


    /***
     * update state of all objects insice objectArray.
     */
    virtual void updateArray(float dt);

    /**
     * Render array of objects!
     */
    virtual void renderArray();


};


#endif //IDJ201601T1_STATE_H
