#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#include "points.h"
#include <math.h>
bool isInsideTriangle (Point* p, Point* p1, Point* p2, Point* p3);
bool isInside (Point* p, Point* poly, Point* center, int npoints, bool relative);
void rotatePoint(Point *ps, Point* pd, double sina , double cosa);
void rotatePoints(Point *ps, Point* pd, double angle, int np);
inline float sqr(float x) {return x * x;}
inline float dist2 (float x1, float y1, float x2, float y2)
    {return sqr(x1 -x2) + sqr (y1 -y2);}
inline float dist (float x1, float y1, float x2, float y2)
    {return sqrt(dist2(x1, y1, x2, y2));}

#endif // MATH_HELPER_H
