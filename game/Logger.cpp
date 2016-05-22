//
// Created by jasf on 4/1/16.
//

#include <iostream>
#include "ctime"
#include "chrono"
#include "Logger.h"

bool Logger::printToScreen = true;
bool Logger::printToFile = false;

void Logger::setPrintToScreen(bool b) {
    Logger::printToScreen = b;

}

void Logger::setPrintToFile(bool b) {
    Logger::printToFile = b;
}

void Logger::log(std::string msg) {
    time_t now = time(0);
    std::string dt = ctime(&now);
    dt = dt.substr(0, dt.size() -1 );

    if (printToScreen) {
        std::cout << "[" << dt << "]" << msg << std::endl;
    }
    if (printToFile){
        //TODO
    }

}

void Logger::err(std::string msg) {
    log(msg);

}
