#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <string>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

struct BoundingBox{
public:
	BoundingBox(){
		min = vec3(1.1754E+38F);
		max = vec3(-1.1754E+38F);
	}

	BoundingBox(vec3 min, vec3 max){
		min = min;
		max = max;
	}

	vec3 min;
	vec3 max;


	void addBoundingBox(BoundingBox b){
		if(b.min.x < min.x){
			min.x = b.min.x;
		}
		if(b.min.y < min.y){
			min.y = b.min.y;
		}
		if(b.min.z < min.z){
			min.z = b.min.z;
		}
		if(b.max.x > max.x){
			max.x = b.max.x;
		}
		if(b.max.y > max.y){
			max.y = b.max.y;
		}
		if(b.max.z > max.z){
			max.z = b.max.z;
		}
	}

};

#endif