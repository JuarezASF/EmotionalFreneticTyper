//
// Created by jasf on 5/22/16.
//

#include <fstream>
#include <string.h>
#include <SDL_events.h>
#include "TyperInput.h"
#include "defines.h"
#include "Game.h"
#include "StageState.h"

std::map<std::string, TyperInput::TypingEvent> TyperInput::eventStringMap =
        {
                {"MOVE", TypingEvent::MOVE},
                {"TURN", TypingEvent::TURN},
                {"RUN",  TypingEvent::RUN},
                {"STOP", TypingEvent::STOP},
                {"JUMP", TypingEvent::JUMP},
                {"DASH", TypingEvent::DASH},
                {"UP", TypingEvent::UP},
                {"DOWN", TypingEvent::DOWN},
                {"SKIP", TypingEvent::SKIP},
                {"START", TypingEvent::START},
                {"PAUSE", TypingEvent::PAUSE},
                {"SMASH", TypingEvent::SMASH},
                {"QUIT", TypingEvent::QUIT},
                {"CONTINUE", TypingEvent::CONTINUE},
                {"GRAB", TypingEvent::GRAB}

        };

std::map<TyperInput::TypingEvent, std::string> TyperInput::eventToStringMap = {
        {TypingEvent::MOVE, "MOVE"},
        {TypingEvent::TURN, "TURN"},
        {TypingEvent::RUN,  "RUN"},
        {TypingEvent::STOP, "STOP"},
        {TypingEvent::JUMP, "JUMP"},
        {TypingEvent::DASH, "DASH"},
        {TypingEvent::UP, "UP"},
        {TypingEvent::DOWN, "DOWN"},
        {TypingEvent::SKIP, "SKIP"},
        {TypingEvent::START, "START"},
        {TypingEvent::PAUSE, "PAUSE"},
        {TypingEvent::SMASH, "SMASH"},
        {TypingEvent::QUIT, "QUIT"},
        {TypingEvent::CONTINUE, "CONTINUE"},
        {TypingEvent::GRAB , "GRAB"}
};

void TyperInput::loadDictionary(std::string filename) {

    std::ifstream is(filename);

    if (!is.is_open()) {
        std::cerr << "Cannot open file " << filename << std::endl;
        quitRequested = true;
        return;
    }

    int qtdKeyWords;

    is >> qtdKeyWords;
    std::string key;
    std::string value;
    int qtd;

    for (int k = 0; k < qtdKeyWords; k++) {
        is >> key;

        if (TyperInput::eventStringMap.find(key) == TyperInput::eventStringMap.end()) {
            std::cerr << "Key " << key << " is not valid!" << std::endl;
            quitRequested = true;
            return;
        }

        TypingEvent currentType = eventStringMap[key];

        is >> qtd;

        for (int i = 0; i < qtd; i++) {
            is >> value;

            if (eventStringMap.find(value) != eventStringMap.end()) {
                std::cerr << "This is probably a mistake! String " << value << " read as value" << std::endl;
                return;
            }
            stringEventMap[value] = currentType;
            if (eventPossibleStringsMap.find(currentType) == eventPossibleStringsMap.end()) {
                eventPossibleStringsMap.insert(std::make_pair(currentType, std::vector<std::string>()));
            }
            eventPossibleStringsMap[currentType].push_back(value);
        }
    }


    is.close();
}

void TyperInput::update() {

    SDL_Event event;

    quitRequested = false;


    // SDL_PollEvent retorna 1 se encontrar eventos, zero caso contrário
    while (SDL_PollEvent(&event) != 0) {

        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;

                break;


            case SDL_KEYDOWN:
                if (event.key.keysym.sym == ESCAPE_KEY) {
                    quitRequested = true;
                }
                if (event.key.repeat){
                    std::cout << "skiping repeated key" << std::endl;
                    continue;

                }
                //ignore backspace
                if (event.key.keysym.sym == SDLK_BACKSPACE ) {
                    std::cout << "skiping backspace" << std::endl;
                    continue;
                }

                if (event.key.keysym.sym == SDLK_RETURN) {
                    std::cout << "on return" << std::endl;
                    clearBuffer();
                    continue;
                }

                //only consider keys that were previously released
                if (!keyState[event.key.keysym.sym]) {
                    if (event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z'){
                        buffer += event.key.keysym.sym;
                        checkForKnownWord();
                    }
                    else{
                        clearBuffer();
                    }
                }
                keyState[event.key.keysym.sym] = true;

                break;

            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;

                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            case SDL_WINDOWEVENT:
            case SDL_AUDIODEVICEADDED:
            case SDL_TEXTINPUT:
                break;

            default:
                std::cerr << "Unexpecte envet of code " << event.type << std::endl;
                break;

        }
    }

}

TyperInput::~TyperInput() {

}

TyperInput &TyperInput::getInstance() {
    static TyperInput intance = TyperInput();
    return intance;
}

TyperInput::TyperInput() {
    for (unsigned int code = 0x0; code <= 0x7F; code++) {
        keyState[code] = 0;
    }

    for (unsigned int code = 0x40000000; code <= 0x4000011A; code++) {
        keyState[code] = 0;
    }

}

void TyperInput::printDictionary() {

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "direct map" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    for (auto it : eventPossibleStringsMap) {
        std::cout << eventToStringMap[it.first] << ":|";
        for (unsigned k = 0; k < it.second.size(); k++) {
            std::cout << it.second[k] << "|";
        }
        std::cout << std::endl;
    }

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "reverse map" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (auto it : stringEventMap) {
        std::cout << "|" << it.first << "|:" << eventToStringMap[it.second] << std::endl;
    }

}

void TyperInput::clearBuffer() {

    if (stringEventMap.find(buffer) != stringEventMap.end()) {
        std::cout << "recognized:" << buffer << std::endl;
        typedCommands.push_back(stringEventMap[buffer]);
    }

    std::cout << "buffer cleared. Content was:" << buffer << std::endl;
    buffer.clear();

}

std::string TyperInput::getBuffer() { return buffer; }

void TyperInput::checkForKnownWord() {

    for(auto it : stringEventMap ){

        size_t pos = buffer.find(it.first);
        if (pos != string::npos){
            std::cout << "recognized:" << it.first << std::endl;
            typedCommands.push_back(it.second);
            typedValidWords.push_back(it.first);
            buffer.erase(0, pos + it.first.length());
        }
    }

}

TyperInput::TypingEvent TyperInput::getTypingEvent() {
    TypingEvent out(typedCommands.front());
    typedCommands.pop_front();
    return out;
}

TyperInput::TypingEvent TyperInput::peakTypingEvent() {
    TypingEvent out(typedCommands.front());
    return out;
}

void TyperInput::flushTypedValidWords() {
    typedValidWords.clear();

}
