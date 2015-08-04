#include <math.h>
#include "math_helper.h"
bool isInside(Point* p, Point* poly, Point* center, int npoints, bool relative)
{
    for (int i=0; i< npoints; i++)
    {
        Point p1, p2;
        int i1 = (i+1) % npoints;
        p1 = poly[i];
        p2 = poly[i1];
        if (relative)
        {
            p1.x = p1.x + center->x;
            p1.y = p1.y + center->y;
            p2.x = p2.x + center->x;
            p2.y = p2.y + center->y;
        }
        if (isInsideTriangle (p, &p1, &p2, center))
            return true;
    }
    return false;
}

bool isInsideTriangle(Point *p, Point *p1, Point *p2, Point *p3)
{
    float d1 = (p1->x - p->x) * (p2->y - p1->y) - (p2->x - p1->x) * (p1->y - p->y);
    float d2 = (p2->x - p->x) * (p3->y - p2->y) - (p3->x - p2->x) * (p2->y - p->y);
    if (d1*d2 < 0) return false;
    float d3 = (p3->x - p->x) * (p1->y - p3->y) - (p1->x - p3->x) * (p3->y - p->y);
    bool b = d2*d3>=0;
    return b;
}

void rotatePoint(Point *ps, Point* pd, double sina , double cosa)
{
    pd->x = ps->x * cosa - ps->y * sina;
    pd->y = ps->x * sina +ps->y * cosa;
}
void rotatePoints(Point *ps, Point* pd, double angle, int np)
{
    double sina = sin(angle);
    double cosa = cos(angle);
    for (int i=0; i<np; i++)
        rotatePoint(&ps[i], &pd[i], sina, cosa);
}

