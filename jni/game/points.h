#ifndef POINTS_H
#define POINTS_H

struct Point3D
{
	Point3D () : x (0.0), y(0.0), z(0.0) {}
	Point3D (float _x, float _y, float _z) : x (_x), y(_y), z(_z) {}
	Point3D (float _x, float _y) : x (_x), y(_y), z(0.0) {}

	float x,y,z;

};
struct Point4D
{
	Point4D () : r (0.0), g(0.0), b(0.0), alpha(1.0) {}
    Point4D (float _r, float _g, float _b) : r (_r), g(_g), b(_b), alpha(1.0) {}
	Point4D (float _r, float _g, float _b, float _alpha) : r (_r), g (_g), b(_b), alpha(_alpha) {}
	float r , g, b, alpha;
	float x() {return r;}
	float y() {return g;}
	float z() {return b;}
};
typedef Point3D Point;
typedef Point4D Color;
#define COLOR_BLACK Color(0.0, 0.0, 0.0)
#define COLOR_WHITE Color(1.0, 1.0, 1.0)
#define COLOR_RED Color(1.0, 0.0, 0.0)
#define COLOR_GREEN Color(0.0, 1.0, 0.0)
#define COLOR_BLUE Color(0.0, 0.0, 1.0)
#define COLOR_YELLOW Color(1.0, 1.0, 0.0)
#define COLOR_CYAN Color(0.0, 0.8.0, 1.0)
#define COLOR_MAGENTA Color(1.0, 0.0, 0.8)
#define COLOR_BROWN Color(0.3, 0.15, 0.0)
#endif // POINTS_H
