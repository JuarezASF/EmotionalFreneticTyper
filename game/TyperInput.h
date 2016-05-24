//
// Created by jasf on 5/22/16.
//

#ifndef IDJ201601T1_TYPERINPUT_H
#define IDJ201601T1_TYPERINPUT_H


#include <unordered_map>
#include "Vec2.h"

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <queue>

class TyperInput {
public:
    typedef enum TypingEvent {
        MOVE, TURN, RUN, STOP, JUMP, DASH, UP, DOWN
    } TypingEvent;

    static std::map <std::string, TypingEvent> eventStringMap;
    static std::map <TypingEvent, std::string> eventToStringMap;

private:
    TyperInput();

    std::string buffer;

    // associates input string with the equivalent event produced
    std::map<std::string, TypingEvent> stringEventMap;
    // each event may be produced by a list a strings
    std::map<TypingEvent, std::vector<std::string>> eventPossibleStringsMap;

    std::unordered_map<int, bool> keyState;

    std::queue<TypingEvent> typedCommands;

    bool quitRequested;

    int updateCounter;


public:

    void loadDictionary(std::string filename);

    void printDictionary();

    void update();

    virtual ~TyperInput();

    static TyperInput &getInstance();

    inline bool getQuitRequested(){return quitRequested;}

    void clearBuffer();

    void checkForKnownWord();

    inline bool hasTypintEvent(){return !typedCommands.empty();}

    TypingEvent getTypingEvent();


};


#endif //IDJ201601T1_TYPERINPUT_H
