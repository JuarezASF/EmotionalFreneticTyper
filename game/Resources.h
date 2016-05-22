//
// Created by jasf on 3/25/16.
//

#ifndef IDJ201601T1_RESOURCES_H
#define IDJ201601T1_RESOURCES_H

#include <unordered_map>
#include <string>
#include <SDL_render.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Resources {

protected:
    static std::unordered_map<std::string, SDL_Texture *> imageTable;
    static std::unordered_map<std::string, Mix_Music*> musicTable;
    static std::unordered_map<std::string, Mix_Chunk*> soundTable;
    static std::unordered_map<std::string, TTF_Font*> fontTable;

public:

    static SDL_Texture *getImage(std::string filename);

    static Mix_Music *getMusic(std::string file);

    static Mix_Chunk *getSound(std::string file);

    static TTF_Font *getFont(std::string file, int ptSize);


    static void clearImages();

    static void clearMusics();

    static void clearSounds();

    static void clearFonts();

};


#endif //IDJ201601T1_RESOURCES_H
