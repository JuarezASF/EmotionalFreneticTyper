//
// Created by jasf on 5/21/16.
//

#include <SDL.h>
#include <SDL_mixer.h>
#include "Sound.h"
#include "Game.h"
#include "Resources.h"

Sound::Sound() {

}

Sound::Sound(std::string file) {

}

void Sound::play(int times) {

    if (chunk)
        channel = Mix_PlayChannel(-1, chunk, times);
    else{
        std::cerr << "Trying to playan unnoped chunk" << std::endl;
        Game::getInstance().requestQuit();
    }


}

void Sound::stop() {

    Mix_HaltChannel(channel);

}

void Sound::open(std::string file) {

    chunk = Resources::getSound(file);

}

bool Sound::isOpen() {
    return false;
}
