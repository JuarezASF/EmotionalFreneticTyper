//
// Created by jasf on 4/1/16.
//

#ifndef IDJ201601T1_LOGGER_H
#define IDJ201601T1_LOGGER_H

#include <string>


class Logger {

protected:
    static bool printToScreen;
    static bool printToFile;

public:
    static void setPrintToScreen(bool b);

    static void setPrintToFile(bool b);

    static void log(std::string msg);

    static void err(std::string msg);


};


#endif //IDJ201601T1_LOGGER_H
