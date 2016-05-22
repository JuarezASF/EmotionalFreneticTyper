//
// Created by jasf on 5/7/16.
//

#include "Timer.h"

Timer::Timer() {

    time = 0.0;

}

void Timer::update(float dt) {
    time += dt;

}

void Timer::restart() {
    time = 0.0;

}

float Timer::get() {
    return time;
}
