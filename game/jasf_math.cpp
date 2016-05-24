#include "jasf_math.h"

double cross2d(Vec2 A, Vec2 B){
    return A.x*B.y - A.y*B.x;
}

bool lineIntersect(Vec2 A_start, Vec2 A_end,
                            Vec2 B_start, Vec2 B_end){
    
    //A(t) = P + t.R : reta 1
    Vec2 P(A_start);
    Vec2 R = (A_end - A_start);
    
    //B(t) = Q + u.S : reta 2
    Vec2 Q(B_start);
    Vec2 S = (B_end - B_start);

    double t = cross2d(Q-P, S)/cross2d(R, S);
    double u = cross2d(Q-P, R)/cross2d(R, S);


    if(cross2d(R,S)==0 && cross2d(Q-P, R)==0){
    //the two line segments are collinear
        if(( (Q-P).dot(R)>=0 && (Q-P).dot(R) <= R.dot(R) ) ||  
            ( (P-Q).dot(S)>=0 && (P-Q).dot(R) <= S.dot(S) ))
            return true;//the line segments are overlapping
        else
            return false;//are collinear but disjoint
    }
    else if(cross2d(R,S)==0 && cross2d(Q-P, R)!=0)
        return false;// parallel and non-intersecting.
    else if(cross2d(R,S)!=0 && t>=0 && t<= 1 && u>=0 && u<= 1)
        return true;//the two lines intersect at P + t*R=Q + U*S
    else
        return false;
}


