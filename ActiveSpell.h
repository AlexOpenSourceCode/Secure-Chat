#pragma once



#ifndef ACTIVESPELL_H
#define ACTIVESPELL_H


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

	class ActiveSpell : public Object {
	public:
		float damage;
		Spell* spell;
		Eigen::Vector3f center;


		ActiveSpell(Eigen::Vector3f _center, Spell* _spell);


		ActiveSpell(Eigen::Vector3f _center, Spell* _spell, float _dx, float _dy, float _dz);

		ActiveSpell(float _x, float _y, float _z, Spell* _spell);

		void draw();

	};

}


#endif



