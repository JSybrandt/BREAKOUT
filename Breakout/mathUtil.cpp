#include "mathUtil.h"

Vector3 rotateX(Vector3 v, float deg)
{
	Vector3 ret;
	ret.y = v.y * cos(deg) - v.z*sin(deg);
	ret.z = v.y * sin(deg) + v.z*cos(deg);
	ret.x = v.x;
	return ret;
}
Vector3 rotateY(Vector3 v, float deg)
{
	Vector3 ret;
	ret.z = v.z * cos(deg) - v.x*sin(deg);
	ret.x = v.z * sin(deg) + v.x*cos(deg);
	ret.y = v.y;
	return ret;
}
Vector3 rotateZ(Vector3 v, float deg)
{
	Vector3 ret;
	ret.x = v.x * cos(deg) - v.y*sin(deg);
	ret.y = v.x * sin(deg) + v.y*cos(deg);
	ret.z = v.z;
	return ret;
}


Vector3 rotate(Vector3 p, Vector3 r)
{
	p = rotateX(p,r.x);
	p = rotateY(p,r.y);
	p = rotateZ(p,r.z);
	return p;
}

int random(int num) {
	return rand()%num+1;
}

float rand01()
{
	return (rand()%100)/100.0;
}