//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_SOUND_H
#define IDJ201601T1_SOUND_H

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_mixer.h>

class Sound {
private:
    Mix_Chunk *chunk;
    int channel;

public:

    Sound();

    Sound(std::string file);

    void play(int times);

    void stop();

    void open(std::string file);

    bool isOpen();


};


#endif //IDJ201601T1_SOUND_H
