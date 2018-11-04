#pragma once



#ifndef CAMERA_H
#define CAMERA_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include "V2ctor.h"
#include "OBB2D.h"
#include "Object.h"

#include <Eigen/Dense>


using namespace std;


namespace rmp {

	class Camera : public Object  {

	public:
		int life;

		Camera(float _x = 0.0, float _y = 0.0, float _z = 0.0);



	};

}


#endif



