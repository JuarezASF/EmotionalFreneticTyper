//
// Created by jasf on 6/15/16.
//

#ifndef IDJ201601T1_SETTINGSLOAD_H
#define IDJ201601T1_SETTINGSLOAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

class SettingsLoad {

public:

    std::map<std::string, long> argumentValMap;

    static SettingsLoad *getInstance();

    void load(std::string fileName);

    /**
     * default value: the value to e returned in case the argument was not found in the settings.txt
     */
    long get(std::string argument, long defaultVaule = 0);

};


#endif //IDJ201601T1_SETTINGSLOAD_H
