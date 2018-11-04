#include "gui_input.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "gui_globals.h"



gui_input::gui_input(string _name, float _x, float _y, float _width, float _height, float _r, float _g, float _b, float _ar, float _ag, float _ab) : gui_obj(_name, _x, _y, _width, _height, _r, _g, _b), active_r(_ar), active_g(_ag), active_b(_ab) {
	type = "input";
	value = "test text";
	label = "Label";
}

gui_input::gui_input(string _name, string _label, string _value, float _x, float _y, float _width, float _height, float _r, float _g, float _b, float _ar, float _ag, float _ab) : gui_obj(_name, _x, _y, _width, _height, _r, _g, _b), label(_label), value(_value), active_r(_ar), active_g(_ag), active_b(_ab) {
	type = "input";

}

//	//const string& gui_input::string&() const {
//	//	return name;
//	//}

const string& gui_input::get_value() const {
	return value;
}

void gui_input::set_value(const string& txt){
	value = txt;
}

const string& gui_input::get_label() const {
	return label;
}

void gui_input::set_label(const string& txt){
	label = txt;
}



void gui_input::draw(){

	//draw text input
	//if (active){

	glColor3f(0.4f, 0.2f, 0);
	gui_globals::draw_text(label, x + margin, y + 15, width - margin, height - margin);

	glColor3f(0.4f, 0.2f, 0);
	gui_globals::draw_text(value, x + margin, y + 35, width - margin, height - margin);

	//}


	glBegin(GL_QUADS);



	//Draw input bock
	float _r = r;
	float _g = g;
	float _b = b;

	if (active){
		_r = active_r;
		_g = active_g;
		_b = active_b;


		_r = 36.0 / 256.0;
		_g = 120.0 / 256.0;
		_b = 36.0 / 256.0;
	}
	else {
		_r = r;
		_g = g;
		_b = b;
	}

	glColor3f(_r, _g, _b);

	//Top left
	glVertex2f(x, y);
	//Top right
	glVertex2f(x + width, y);

	//Bottom right
	glVertex2f(x + width, y + height);

	//Bottom left
	glVertex2f(x, y + height);

	glEnd();
}

void gui_input::click(){
	active = !active;
}

