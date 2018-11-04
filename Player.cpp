#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;


#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>

#include "shader_loader.h";
#include "Object.h";
#include "V2ctor.h";



#include "Player.h"

#include <Eigen/Dense>


using namespace std;
using namespace Eigen;


namespace rmp {




	Player::Player(float _x, float _y, float _z, string _name, float _life) : Object(_x, _y, _z),
		max_life( _life ),
		current_life( _life ),
		name( _name )
	{};


	void Player::add_spell(Spell& spell){
		spells.push_back(&spell);
	}


	Spell* Player::get_spell(string name){
		for (size_t x = 0; x < spells.size(); x++){
			if (spells[x]->get_name() == ""){
				return spells[x];
			}
		}

		return nullptr;
	}

	vector<Spell*> Player::get_spells(){
		return spells;
	}



	void Player::cast_spell(){
		Spell* used_spell = spells[0];

		//cout << "CASTING SPELL: " + used_spell->name << endl;

		//Vector3f active_spell_center(0,0,0);
		//ActiveSpell* new_active_spell = new ActiveSpell(active_spell_center, used_spell);

		//ActiveSpell* new_active_spell = new ActiveSpell(center, used_spell);


		ActiveSpell* new_active_spell = new ActiveSpell(center, used_spell, dx, dy, dz);



		active_spells.push_back(new_active_spell);

	}



	float Player::get_max_life(){
		return max_life;
	}

	float Player::get_current_life(){
		return current_life;
	}



	void Player::take_damage(float damage){
		current_life -= damage;

		if (current_life < 0){
			current_life = 0;
		}

		////cout << "current_life = " + to_string(current_life) << endl;
	}


	string Player::get_name(){
		return name;
	}

	void Player::set_name(string new_name){
		name = new_name;
	}


	void Player::set_target(Player* other_player){
		target = other_player;
	}


	Player Player::get_target(){
		return *target;
	}


	void Player::draw(){
		Object::draw();

		//draw the active spells
		//for (size_t x = 0; x < active_spells.size(); x++){

		//	active_spells[x]->draw();
		//}
	}




}










