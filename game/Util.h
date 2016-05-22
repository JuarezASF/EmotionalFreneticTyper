//
// Created by jasf on 3/16/16.
//

#ifndef IDJ201601T1_UTIL_H
#define IDJ201601T1_UTIL_H


#include <vector>
#include <map>
#include <cmath>

using namespace std;

namespace Util {

    vector<int> getKeys(map<int, string> &m);
    vector<string> getValues(map<int,string> &m);

    inline float rad2deg(float rad){return rad * 180/M_PI;}
    inline float deg2rad(float deg){return deg * M_PI/180.0;}

}


#endif //IDJ201601T1_UTIL_H
