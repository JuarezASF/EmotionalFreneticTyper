//
// Created by jasf on 5/7/16.
//

#ifndef IDJ201601T1_TIMER_H
#define IDJ201601T1_TIMER_H


class Timer {

private:

    float time;

public:

    Timer();

    void update(float dt);

    void restart();

    float get();


};


#endif //IDJ201601T1_TIMER_H
