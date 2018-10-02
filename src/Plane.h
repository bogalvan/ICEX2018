#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

struct Plane {
public:
	Plane(){}

	vec3 pt1, pt2, pt3, pt4;

	void updatePlane(vec3 p1, vec3 p2, vec3 p3, vec3 p4){
		pt1 = p1;
		pt2 = p2;
		pt3 = p3;
		pt4 = p4;
	}
};

#endif 