//
// Created by jasf on 3/15/16.
//

#ifndef IDJ201601T1_GAMEEXCEPTION_H
#define IDJ201601T1_GAMEEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class GameException : public std::exception {
private:
    string msg;


public:

    GameException(string msg);


    virtual const char* what() const noexcept {
        return msg.c_str();
    };
};


#endif //IDJ201601T1_GAMEEXCEPTION_H
