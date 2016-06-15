//
// Created by jasf on 6/15/16.
//

#include "SettingsLoad.h"

SettingsLoad *SettingsLoad::getInstance() {
    static SettingsLoad* instance = new SettingsLoad();
    return instance;
}

void SettingsLoad::load(std::string fileName) {

    std::ifstream is(fileName);

    if(!is.is_open()){
        std::cerr << "Cannot open input settings on file " << fileName << std::endl;
    }

    int qtdArguments = 0;

    is >> qtdArguments;

    std::string parameterName;
    long val;

    argumentValMap.clear();

    while(qtdArguments > 0){
        is >> parameterName;
        is >> val;

        std::cout << "Loading value:" << val << " for parameter name" << parameterName << std::endl;

        argumentValMap[parameterName] = val;

        qtdArguments--;
    }

}

long SettingsLoad::get(std::string argument, long defaultVaule) {
    if(argumentValMap.find(argument) == argumentValMap.end()){
        std::cerr << "Using default value " << defaultVaule << " for parameter named:" << argument << std::endl;
        return defaultVaule;
    }

    return argumentValMap[argument];
}
