#include "gui_obj.h"
#include <GL/glew.h>
#include <GL/freeglut.h>


gui_obj::gui_obj(string _name, float _x, float _y, float _width, float _height, float _r, float _g, float _b) : name(_name), x(_x), y(_y), width(_width), height(_height), r(_r), g(_g), b(_b) {

}

//gui_obj::gui_obj(string _name, float _x, float _y, float _width = 200, float _height = 50, float _r = 0.2f, float _g = 0.2f, float _b = 0.2f) : name(_name), x(_x), y(_y), width(_width), height(_height), r(_r), g(_g), b(_b) {
//
//}

void gui_obj::set_color(float _r, float _g, float _b){
	r = _r;
	g = _g;
	b = _b;
}

const string& gui_obj::get_name() const {
	return name;
}

void gui_obj::draw(){
	glBegin(GL_QUADS);
		//draw background for action bar
		glColor3f(r, g, b);

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

void gui_obj::click() {

}

void gui_obj::hover() {
	hovering = true;
}

void gui_obj::set_hovering(bool b){
	hovering = b;
}

void gui_obj::set_class_name(const string& new_class_name){
	class_name = new_class_name;
}


const string& gui_obj::get_class_name() const {
	return class_name;
}


void gui_obj::set_active(bool b){
	active = b;
}


const bool& gui_obj::is_active() const {
	return active;
}