#include "gui_button.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "gui_globals.h"



gui_button::gui_button(string _name, string _text, float _x, float _y, float _width, float _height, float _r, float _g, float _b, float _ar, float _ag, float _ab) : gui_obj(_name, _x, _y, _width, _height, _r, _g, _b), text(_text), active_r(_ar), active_g(_ag), active_b(_ab) {
	type = "button";
}

void gui_button::set_callback_function(int_func_map _callback_function, map<string, string> callback_func_param_map){
	callback_func_w_map_param = _callback_function;
	callback_function_params = callback_func_param_map;
}

void gui_button::set_callback_function(int_func_noparams _callback_function){
	callback_function = _callback_function;
}


const string& gui_button::get_text() const {
	return text;
}

void gui_button::set_text(const string& txt){
	text = txt;
}


void gui_button::draw(){
	//draw text input
	//if (active){

	glColor3f(0.4f, 0.2f, 0);
	gui_globals::draw_text(label, x + margin, y + 15, width - margin, height - margin);

	glColor3f(0.4f, 0.2f, 0);
	gui_globals::draw_text(text, x + margin, y + 35, width - 5, y + margin);

	//}


	glBegin(GL_QUADS);



	//Draw input bock
	float _r = r;
	float _g = g;
	float _b = b;

	if (hovering){
		_r = active_r;
		_g = active_g;
		_b = active_b;


		_r = 36.0 / 256.0;
		_g = 120.0 / 256.0;
		_b = 36.0 / 256.0;
	}

	if (active){
		_r = active_r;
		_g = active_g;
		_b = active_b;


		_r = 36.0 / 256.0;
		_g = 120.0 / 256.0;
		_b = 36.0 / 256.0;
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

void gui_button::set_hovering(bool b){
	hovering = b;
}

void gui_button::hover() {
	hovering = true;
}

void gui_button::click(){
	std::cout << "CALLING CALLBACK FUNCTION" << endl;
	
	if (callback_function_params.size() > 0){
		this->set_active(true);
		callback_func_w_map_param(callback_function_params);	
	}
	else{
		callback_function();
	}



}

