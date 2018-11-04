#pragma once
#ifndef GUI_INPUT_H
#define GUI_INPUT_H

#include "gui_obj.h"


class gui_input : public gui_obj {
public:

	
	string value;
	string label;
	float active_r;
	float active_g;
	float active_b;



	//Player::Player(float _x, float _y, float _z, string _name, float _life) : Object(_x, _y, _z),


	gui_input(string _name, float _x, float _y, float _width, float _height, float _r = 0.3, float _g = 0.3, float _b = 0.3, float _ar = 0.14, float _ag = 0.55, float _ab = 0.14);

	gui_input(string _name, string _label, string _value, float _x, float _y, float _width, float _height, float _r = 0.3, float _g = 0.3, float _b = 0.3, float _ar = 0.14, float _ag = 0.55, float _ab = 0.14);

	//	////allows for
	//	////  const string& lol_cref = thingOne.get_name();
	//	////which uses the same memory as class for string
	//	////or just string lol = thingOne.get_name(); for a string copy
	//	//const string& get_name() const {
	//	//	return name;

	const string& get_value() const;

	void set_value(const string& txt);
	const string& get_label() const;

	void set_label(const string& txt);

	

	void draw();

	void click();
};

#endif
