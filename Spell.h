#pragma once



#ifndef SPELL_H
#define SPELL_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include <Eigen/Dense>


using namespace std;


namespace rmp {

	class Spell {
		public:
			string name;

			float damage;

			string type;

			float speed;

			Spell(string _name, float _damage, string _type, float _speed = 0);

			void set_name(const string& _name);
			string get_name();

	};

}


#endif



