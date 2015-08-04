#ifndef MAT4_H
#define MAT4_H

#include "points.h"
class Mat4;
class Mat4
{
public:
	Mat4();
    Mat4(float _scale);
	void translate(Point& p);
    void scale (Point& p);
	void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void setToIdentity();
	void rotateZ(float deg);
	float m[4][4];
	Mat4 &	operator*=(const Mat4 & other);

};

Mat4 operator * (const Mat4 &m1, const Mat4 &m2);

#endif // MAT4_H
