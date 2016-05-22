//
// Created by jasf on 3/25/16.
//

#include "Resources.h"
#include "Game.h"

std::unordered_map<std::string, SDL_Texture *> Resources::imageTable;
std::unordered_map<std::string, Mix_Music *> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk*> Resources::soundTable;
std::unordered_map<std::string, TTF_Font*> Resources::fontTable;

SDL_Texture *Resources::getImage(std::string filename) {
    if (imageTable.find(filename) != imageTable.end())
        return imageTable[filename];

    SDL_Texture *texture = IMG_LoadTexture(Game::getInstance().getRenderer(), filename.c_str());

    if (!texture) {
        std::cerr << "Error while loading texture on file " << filename << std::endl;
        std::cerr << SDL_GetError() << std::endl;
    }

    imageTable.emplace(std::make_pair(filename, texture));

    return texture;

}

void Resources::clearImages() {

    for (auto tableEntry : imageTable) {
        if (tableEntry.second)
            SDL_DestroyTexture(tableEntry.second);
    }

    imageTable.clear();


}

Mix_Music *Resources::getMusic(std::string file) {

    if (musicTable.find(file) == musicTable.end()) {
        Mix_Music *newMusic = Mix_LoadMUS(file.c_str());

        if (newMusic != nullptr)
            musicTable[file] = newMusic;
        else
            std::cerr << "Cannot open music on file " + file << std::endl;
    }
    return musicTable[file];
}

void Resources::clearMusics() {
    for (auto e : musicTable) {
        if (e.second)
            Mix_FreeMusic(e.second);
    }

    musicTable.clear();

}

Mix_Chunk *Resources::getSound(std::string file) {
    if (soundTable.find(file) == soundTable.end()) {
        Mix_Chunk *newSound = Mix_LoadWAV(file.c_str());

        if (newSound != nullptr)
            soundTable[file] = newSound;
        else
            std::cerr << "Cannot open sound on file " + file << std::endl;
    }
    return soundTable[file];
}

void Resources::clearSounds() {
    for (auto e : soundTable) {
        if (e.second)
            Mix_FreeChunk(e.second);
    }

    soundTable.clear();

}

TTF_Font *Resources::getFont(std::string file, int ptSize) {
    std::string nameOnMap = file + std::to_string(ptSize);
    if (fontTable.find(nameOnMap) == fontTable.end()){
        TTF_Font *newFont = TTF_OpenFont(file.c_str(), ptSize);

        if (newFont)
            fontTable[nameOnMap] = newFont;
        else{
            std::cerr << "Cannot open font on file " << file << std::endl;
            Game::getInstance().requestQuit();
        }
    }
    return fontTable[nameOnMap];
}

void Resources::clearFonts() {

    for( auto e : fontTable)
        if(e.second)
            TTF_CloseFont(e.second);

    fontTable.clear();

}
