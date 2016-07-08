//
// Created by jasf on 7/8/16.
//

#include <fstream>
#include <sstream>
#include "PlataformasManager.h"


PlataformasManager::PlataformasManager() {

    const string configFileName = "txt/plataformas_config.txt";

    ifstream ifs;
    ifs.open(configFileName, ios_base::in);

    if (!ifs.is_open()) {
        cerr << "could not load file:" << configFileName << endl;
        cerr.flush();
        return;
    }

    string buffer;
    stringstream ss;

    int qtdImages;
    string imgA, imgB;
    int up, down, left, right;
    string name;

    char msg[200];
    while (getline(ifs, buffer)) {
        if (buffer.compare("*") == 0)
            continue;

        ss.clear();
        ss.str(buffer);

        ss >> name;
        ss >> qtdImages;

        ss >> imgA;

        imgB = "";
        if (qtdImages == 2)
            ss >> imgB;

        ss >> up >> down >> left >> right;

        config.insert(make_pair(name, make_tuple(imgA, imgB, up, down, left, right)));

        sprintf(msg, "name: %s fileA: %s fileB: %s config: %d %d %d %d", name.c_str(), imgA.c_str(), imgB.c_str(), up,
                down, left, right);

        cout << string(msg) << endl;

    }

}

PlataformasManager *PlataformasManager::getInstance() {
    static PlataformasManager *instance = new PlataformasManager();
    return instance;
}

Platform *PlataformasManager::getPlatformCenteredAt(Vec2 center, string name) {
    if (config.find(name) == config.end()) {
        cerr << "invalid platform name:" << name << endl;
        return nullptr;
    }
    const tuple<string, string, int, int, int, int> &c = config[name];

    int top = (int)get<2>(config[name]);
    int bot = (int)get<3>(config[name]);
    int left = (int)get<4>(config[name]);
    int right = (int)get<5>(config[name]);

    Vec2 verticalLimist(top, bot);
    Vec2 horizontalLimits(left, right);

    Platform *out = new Platform(get<0>(c), get<1>(c), center, verticalLimist, horizontalLimits);

    return out;

}
