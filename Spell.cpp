#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;


#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>



#include "Spell.h";

#include <Eigen/Dense>


using namespace std;
using namespace Eigen;


namespace rmp {



	Spell::Spell(string _name, float _damage, string _type, float _speed) :
		name( _name ),
		damage( _damage ),
		type( _type ),
		speed( _speed )
	{};

	void Spell::set_name(const string& _name){
		name = _name;
	}


	string Spell::get_name(){
		return name;
	}

}









