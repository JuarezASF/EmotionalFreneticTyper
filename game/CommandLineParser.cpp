//
// Created by jasf on 3/25/16.
//

#include "CommandLineParser.h"
#include <exception>

CommandLineParser::CommandLineParser() {

}


void CommandLineParser::help() {

}

int CommandLineParser::getIntVal(std::string argName) {
    if(argValMap.find(argName) == argValMap.end())
        throw std::runtime_error("argument " + argName + " was not found");

    if(!argValMap[argName].iSet)
        throw std::runtime_error("argument " + argName + " was not set");

    return argValMap[argName].iVal;
}

double CommandLineParser::getDoubleVal(std::string argName) {
    if(argValMap.find(argName) == argValMap.end())
        throw std::runtime_error("argument " + argName + " was not found");

    if(!argValMap[argName].dSet)
        throw std::runtime_error("argument " + argName + " was not set");

    return argValMap[argName].dVal;
}

std::string CommandLineParser::getStrVal(std::string argName) {
    if(argValMap.find(argName) == argValMap.end())
        throw std::runtime_error("argument " + argName + " was not found");

    if(!argValMap[argName].strSet)
        throw std::runtime_error("argument " + argName + " was not set");

    return argValMap[argName].strVal;
}


std::set<std::string> CommandLineParser::validTypes{"int", "string", "double", "bool"};



bool CommandLineParser::wasDefined(std::string name) {
    return false;
}

void CommandLineParser::addIntArg(std::string name, bool required, int defaultValue) {
    std::string type = "int";
    if (validTypes.find(type) == validTypes.end())
        throw std::runtime_error("Invalid type " + type);

    if (argTypeMap.find(name) != argTypeMap.end())
        throw std::runtime_error("arg " + name + "was already defined!");

    argTypeMap[name] = type;

    argValMap[name] = CommandLineEntry(name, type, required, defaultValue);
}

void CommandLineParser::addDoubleArg(std::string name, bool required, double defaultValue) {
    std::string type = "double";
    if (validTypes.find(type) == validTypes.end())
        throw std::runtime_error("Invalid type " + type);

    if (argTypeMap.find(name) != argTypeMap.end())
        throw std::runtime_error("arg " + name + "was already defined!");

    argTypeMap[name] = type;

    argValMap[name] = CommandLineEntry(name, type, required, defaultValue);

}

void CommandLineParser::addStrArg(std::string name, bool required, std::string defaultValue) {
    std::string type = "string";
    if (validTypes.find(type) == validTypes.end())
        throw std::runtime_error("Invalid type " + type);

    if (argTypeMap.find(name) != argTypeMap.end())
        throw std::runtime_error("arg " + name + "was already defined!");

    argTypeMap[name] = type;

    argValMap[name] = CommandLineEntry(name, type, required, defaultValue);

}
CommandLineEntry::CommandLineEntry(std::string name, std::string type, bool required) :
        name(name), type(type), required(required) {
    init();

}

CommandLineEntry::CommandLineEntry(std::string name, std::string type, bool required, int defVal) :
        name(name), type(type), required(required), iVal(defVal) {
    init();
}

CommandLineEntry::CommandLineEntry(std::string name, std::string type, bool required, double defVal) :
        name(name), type(type), required(required), dVal(defVal) {
    init();

}

CommandLineEntry::CommandLineEntry(std::string name, std::string type, bool required, std::string defVal) :
        name(name), type(type), required(required), strVal(defVal) {
    init();

}
CommandLineEntry::CommandLineEntry(std::string name, std::string type, bool required, bool defVal) :
        name(name), type(type), required(required), bVal(defVal) {
    init();
}

void CommandLineParser::addBoolArg(std::string name, bool required, bool defaultValue) {
    std::string type = "bool";
    if (validTypes.find(type) == validTypes.end())
        throw std::runtime_error("Invalid type " + type);

    if (argTypeMap.find(name) != argTypeMap.end())
        throw std::runtime_error("arg " + name + "was already defined!");

    argTypeMap[name] = type;

    argValMap[name] = CommandLineEntry(name, type, required, defaultValue);

}

void CommandLineEntry::init() {
    iSet = false;
    dSet = false;
    strSet = false;
    bSet = false;

}

CommandLineEntry::CommandLineEntry() {

}

void CommandLineParser::parseCommandLine(std::vector<std::string> argv) {
    for (unsigned int i = 0; i < argv.size(); i++) {
        std::string current_arg = argv[i];
        if (argTypeMap.find(current_arg) == argTypeMap.end())
            throw std::runtime_error("invalid arg " + current_arg);

        if (argTypeMap[current_arg].compare("int") == 0) {
            if (i + 1 >= argv.size())
                throw std::runtime_error("Missing value for argument " + current_arg);
            int val = std::stoi(argv[i + 1]);
            i++;

            argValMap[current_arg].iVal = val;
            argValMap[current_arg].iSet = true;

        }
        else if (argTypeMap[current_arg].compare("double") == 0) {
            if (i + 1 >= argv.size())
                throw std::runtime_error("Missing value for argument " + current_arg);
            double val = std::stod(argv[i + 1]);
            i++;

            argValMap[current_arg].dVal = val;
            argValMap[current_arg].dSet = true;

        }
        else if (argTypeMap[current_arg].compare("string") == 0) {
            if (i + 1 >= argv.size())
                throw std::runtime_error("Missing value for argument " + current_arg);
            std::string val = argv[i + 1];
            i++;

            argValMap[current_arg].strVal = val;
            argValMap[current_arg].strSet = true;

        }
        else if (argTypeMap[current_arg].compare("bool") == 0) {
            argValMap[current_arg].bVal = true;
            argValMap[current_arg].bSet = true;
        }
        else{
            throw std::runtime_error("Bad formating on commmand line input!");
        }

    }

}
void CommandLineParser::parseCommandLine(int argc, char **argv) {
    std::vector<std::string> v;
    for (int i = 1 ; i < argc; i++)
        v.push_back(argv[i]);

    parseCommandLine(v);

}

double CommandLineParser::getBoolVal(std::string argName) {
    if(argValMap.find(argName) == argValMap.end())
        throw std::runtime_error("argument " + argName + " was not found");

    //if required, make sure the value was set
    if(!argValMap[argName].bSet && argValMap[argName].required)
        throw std::runtime_error("argument " + argName + " was not set");

    return argValMap[argName].bVal;
}
