#ifndef JASF_MATH_H
#define JASF_MATH_H

#include <iostream>
#include <vector>
#include "Vec2.h"

using namespace std;

//returns true in case the line segments intersect
bool lineIntersect(Vec2 A_start, Vec2 A_end,
                            Vec2 B_start, Vec2 B_end);

bool lineIntersect(vector<Vec2> segmentA, vector<Vec2> segmentB);


#endif
