//
// Created by jasf on 5/21/16.
//

#ifndef IDJ201601T1_MUSIC_H
#define IDJ201601T1_MUSIC_H


#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Music {

private:

    Mix_Music *music;

public:

    Music();

    Music(std::string file);

    void play(int times);

    void stop();

    void open(std::string fle);

    bool isOpen();




};


#endif //IDJ201601T1_MUSIC_H
