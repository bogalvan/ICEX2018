#ifndef ANCHOR_H
#define ANCHOR_H

#include <string>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FrustumObj.h"
#include "ray.h"
#include "Utilities.h"
using namespace std;
using namespace glm;

struct Anchor: public Obj{
public:
	Anchor(){
	}

	Anchor(string dir, string Mesh, string vertShader, string fragShader, string texture, int textNum, int textType, int sType){
		RESOURCE_DIR = dir;
		mesh = Mesh; 
		vert = vertShader;
		frag = fragShader;
		textureName = texture;
		textureNum = textNum;
		textureType = textType;
		shapeType = sType;
		anchorType = -1;
		parentIndex = -1;
		pathLength =1;
	}


   float anchorType;
   Camera camera; 
   vec3 pos;
   vec3 view;
   FrustumObj fObj;
   float sampleNums;
   float weight=0;
   float depth;
   float camPhi;
   float camTheta;
   int parentIndex;
   float robotTheta;
   int pathLength;
   int ndex;
   int radius = 80;
   int root =0;
   vec3 constVelMults = vec3(1.0, 1.0, 0.8);

	void createLookAt(vec3 origin, vec3 *lookAt){
		float radius = 6;
		float theta;
		vec3 pitch, yaw;
	   //vec3 temp = vec3(origin.x+.001,origin.y-100, origin.z+.001);
	   vec3 temp = vec3(0,0,0);
	   vec3 look = vec3(0,-1,0);

		camPhi = rand() / float(RAND_MAX);
		camPhi = (1.0f - camPhi) * -20 + camPhi * 20;
		//cout << "phi: " << camPhi << "\n";
   	float delta = radians(camPhi);
   	pitch.x = 0;
   	pitch.z = look.z*cos(delta) - look.y*sin(delta);
   	pitch.y = look.y*cos(delta) - look.z*sin(delta);
   	//pitch = normalize(pitch);
   	//cout << "pitch: " << pitch.x  << " " << pitch.y << " " << pitch.z << "\n";
   	yaw = getPerpendicularVector(pitch);
   	//yaw = normalize(yaw);		
   	camTheta = atan(yaw.x/yaw.z);
   	//cout << "yaw: " << yaw.x  << " " << yaw.y << " " << yaw.z << "\n";
   	//cout << "theta: " << camTheta << "\n";


   	vec3 dir = pitch + yaw; 
   	dir *= vec3(1,1,1);
   	temp = dir + origin;

   	//cout << "dir: " << dir.x  << " " << dir.y << " " << dir.z << "\n";
   	temp = dir;
	   lookAt->x = temp.x;
		lookAt->y = temp.y;
		lookAt->z = temp.z;

		/*
		lookAt->x =rand()%1000;
		lookAt->y =rand()%1000;
		lookAt->z =rand()%1000;
		*/
		/*
		lookAt->x = 0;
		lookAt->y = 0;
		lookAt->z = 0;
		*/
	}


   vec3 createPos(float height, int pathLength, int numNodes){
   	double posTheta = pathLength * ((2 * M_PI) / numNodes);
   	double curRadius = randRangef(radius - 1, radius + 1);
   	return glm::vec3(
			constVelMults[0] * curRadius * cos(posTheta), 
			constVelMults[1] * height,
			constVelMults[2] * curRadius * sin(posTheta));
   }

   vec3 getPerpendicularVector(vec3 v){
   	float y;
   	y = (-v.x-v.z)/v.y;
   	return vec3(1,y,1);
   }


   void setPositionAndLookAt(Anchor *prev, vec3 *lookAt, vec3 *pos, int iteration){
   	*lookAt = vec3(0,0,0);
   	*pos = prev->pos;


   	vec3 pitch, yaw, heading;
   	vec3 look = vec3(0,-1,0); //camera is looking straight down 
   	//srand(time(0));
		float delta = rand() / float(RAND_MAX);
		delta = (1.0f - delta) * -20 + delta * 20;
   	cout << "delta: " << delta << "\n";
   	//cout << "prev Phi: " << prev->camPhi << " \nprev Theta: " << prev->camTheta << "\n";; 
   	//if(iteration%2 == 0){ //even iteration change yaw (effects x and z) by 20 degrees and set position
   		camPhi = prev->camPhi + delta;
   		if(camPhi > 20){
   			camPhi = 20;
   		}else if(camPhi < -20){
   			camPhi = -20;
   		}
   		//cout << "phi: " << camPhi << "\n";


   		
   		delta = radians(camPhi);
   		yaw.x = look.x*cos(delta) - look.y*sin(delta);
   		yaw.y = look.y*cos(delta) - look.x*sin(delta);
   		yaw.z =0;   		

   		pitch = getPerpendicularVector(yaw);
   		//yaw = normalize(yaw);		
   		camTheta = atan(pitch.x/pitch.z);
   		camTheta = degrees(camTheta);


   	/*}else{ //odd iteration change pitch (effects y and z) by 20 degrees and set lookAt
   		camTheta = prev->camTheta + delta;
   		if(camTheta > 20){
   			camTheta =20;
   		}else if(camTheta < -20){
   			camTheta = -20;
   		}

   		delta = radians(camTheta);
   		pitch.x = 0;
   		pitch.y = look.y*cos(delta) - look.z*sin(delta);
   		pitch.z = look.z*cos(delta) - look.y*sin(delta);
   		
   		//cout << "pitch: " << pitch.x  << " " << pitch.y << " " << pitch.z << "\n";
   		yaw = getPerpendicularVector(pitch);	
   		camPhi= atan(yaw.x/yaw.z);		
   		//cout << "yaw: " << yaw.x  << " " << yaw.y << " " << yaw.z << "\n";
   		//cout << "theta: " << camTheta << "\n";
   	}*/
   	heading = normalize(cross(pitch,yaw));
   	//cout << "yaw: " << yaw.x << " " << yaw.y << " " << yaw.z << "\n";
   	//cout << "pitch: " << pitch.x << " " << pitch.y << " " << pitch.z << "\n";
      //cout << "heading: " << heading.x << " " << heading.y << " " << heading.z << "\n";
   	look = pitch+yaw;
   	*pos = prev->pos + heading*vec3(4,4,4);
   	*lookAt = *pos + look; 	

   	lookAt->x = 0;
		lookAt->y = 0;
		lookAt->z = 0;
   }


   void createAnchor(int iteration,Anchor *prev, int numNodes, int pathlength, float aspect, float zNear, BoundingBox bb){
		vec3 lookAt;
   	vec3 position;
   	setPositionAndLookAt(prev,&lookAt,&position,iteration);
      pos = position; 
      //cout << "pos: " << pos.x << " " << pos.y << " " << pos.z << "\n";
      //cout << "lookAt: " << lookAt.x << " " << lookAt.y << " " << lookAt.z << "\n";
   	pathLength = pathlength;
   	Camera c = Camera(position, lookAt, 1);
   	c.updatePerspective(aspect, 45, zNear, 90);
		c.setRayParameters(1,200);
		c.createFrustum();
		setCamera(c,c.startPos.y);
		getWeight(bb);
   }

   void setCamera(Camera c,float Depth){
   	camera = c;
   	depth = Depth;
   	robotTheta = randRangef(0.0f, M_PI);

   }

   bool rayBBIntersection(Ray r, BoundingBox b){
   	float tmin = (b.min.x - r.origin.x) / r.dir.x; 
      float tmax = (b.max.x - r.origin.x) / r.dir.x; 
 
	    if (tmin > tmax) swap(tmin, tmax); 
	 
	    float tymin = (b.min.y - r.origin.y) / r.dir.y; 
	    float tymax = (b.max.y - r.origin.y) / r.dir.y; 
	 
	    if (tymin > tymax) swap(tymin, tymax); 
	 
	    if ((tmin > tymax) || (tymin > tmax)) 
	        return false; 
	 
	    if (tymin > tmin) 
	        tmin = tymin; 
	 
	    if (tymax < tmax) 
	        tmax = tymax; 
	 
	    float tzmin = (b.min.z - r.origin.z) / r.dir.z; 
	    float tzmax = (b.max.z - r.origin.z) / r.dir.z; 
	 
	    if (tzmin > tzmax) swap(tzmin, tzmax); 
	 
	    if ((tmin > tzmax) || (tzmin > tmax)) 
	        return false; 
	 
	    if (tzmin > tmin) 
	        tmin = tzmin; 
	 
	    if (tzmax < tmax) 
	        tmax = tzmax; 
 
   	return true;
   }

   void getWeight(BoundingBox b){
   	int intersections = 0;
   	sampleNums = camera.sampleNums;
   	for(int i 	= 0; i < camera.rays.size(); i++){ 	 
   		intersections += rayBBIntersection(camera.rays[i],b);;
   	}
   	//cout << "intersections: " << intersections << "\n"; 
   	weight = intersections/sampleNums;
   }

   void addPosition(vec3 loc){
   	pos = loc;
   }

   void addObjInfo(string dir, string Mesh, string vertShader, string fragShader, string texture, int textNum, int textType, int sType){
		RESOURCE_DIR = dir;
		mesh = Mesh; 
		vert = vertShader;
		frag = fragShader;
		textureName = texture;
		textureNum = textNum;
		textureType = textType;
		shapeType = sType;
	}

	void addUniforms(){
    	prog->addUniform("P");
	 	prog->addUniform("M");
		prog->addUniform("V");
		prog->addAttribute("vertPos");
      prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");
		prog->addUniform("anchorType");
      prog->addUniform("fragTexture");
	}

	void updateAnchorType(int type){
		anchorType = type;
	}

	void draw(shared_ptr<MatrixStack> M, shared_ptr<MatrixStack>  P, mat4 V){

		prog->bind();
		texture->bind(prog->getUniform("fragTexture"));
	   glUniform1f(prog->getUniform("anchorType"),anchorType);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		M->pushMatrix();
		M->translate(trans);
		M->scale(scale);
		if(angle != 0){
			M->rotate(angle, rot);
		}
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		if(shapeType == 0){
			shape->draw(prog);
		}else{
			for (int i = 0; i < AllShapes.size() ; i++) {
			   AllShapes[i]->draw(prog);
		   }
		}
		
		M->popMatrix();
		prog->unbind();
	}

};

#endif