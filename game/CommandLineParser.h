//
// Created by jasf on 3/25/16.
//

#ifndef IDJ201601T1_COMMANDLINEPARSER_H
#define IDJ201601T1_COMMANDLINEPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

class CommandLineEntry{
public:
    std::string name, type;
    bool required;
    int iVal;
    std::string strVal;
    double dVal;
    bool bVal;

    bool iSet, dSet, strSet, bSet;

    CommandLineEntry();
    CommandLineEntry(std::string name, std::string type, bool required = false);
    CommandLineEntry(std::string name, std::string type, bool required = false, int defVal = 0);
    CommandLineEntry(std::string name, std::string type, bool required = false, double defVal = 0.0);
    CommandLineEntry(std::string name, std::string type, bool required = false, std::string devVal = "");
    CommandLineEntry(std::string name, std::string type, bool required = false, bool defVal = false);

private:
    void init();


};

class CommandLineParser {

protected:
    std::map<std::string, std::string> argTypeMap;
    std::map<std::string, CommandLineEntry> argValMap;

    static std::set<std::string> validTypes;

public:

    CommandLineParser();


    void addIntArg(std::string name,  bool required, int defaultValue);

    void addDoubleArg(std::string name, bool required, double defaultValue);

    void addStrArg(std::string name,  bool required, std::string defaultValue);

    void addBoolArg(std::string name,  bool required, bool defaultValue);

    void parseCommandLine(int argc, char **argv);

    void parseCommandLine(std::vector<std::string> argv);

    void help();

    bool wasDefined(std::string name);

    int getIntVal(std::string argName);
    double getDoubleVal(std::string argName);
    std::string getStrVal(std::string argName);
    double getBoolVal(std::string argName);







};


#endif //IDJ201601T1_COMMANDLINEPARSER_H
