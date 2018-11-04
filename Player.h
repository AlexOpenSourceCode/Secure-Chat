#pragma once



#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Object.h"
#include "V2ctor.h"
#include "OBB2D.h"
#include "Spell.h"
#include "ActiveSpell.h"

#include <Eigen/Dense>


using namespace std;


namespace rmp {


	class Player : public Object {
		public:
			float max_life;
			float current_life;
			string name = "";
			Player* target;
			vector<Spell*> spells;
			vector<ActiveSpell*> active_spells;

			Player(float _x, float _y, float _z, string _name = "New Player", float _life = 10.0);


			void add_spell(Spell& spell);


			Spell* get_spell(string name);

			vector<Spell*> get_spells();

			void cast_spell();



			float get_max_life();

			float get_current_life();



			void take_damage(float damage);


			string get_name();

			void set_name(string new_name);

			void set_target(Player* other_player);


			Player get_target();

			void draw();


	};

}


#endif






