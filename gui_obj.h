#pragma once
#ifndef GUI_OBJ_H
#define GUI_OBJ_H

#include <string>
using namespace std;


class gui_obj {
public:
	float x;
	float y;
	float width;
	float height;
	float r;
	float g;
	float b;

	float margin = 10;


	string name;

	string type = "default";

	string class_name = "";

	bool hovering = false;
	bool active = false;


	gui_obj(string _name, float _x, float _y, float _width = 200, float _height = 50, float _r = 0.2f, float _g = 0.2f, float _b = 0.2f);

	void set_color(float _r, float _g, float _b);
	void set_active(bool b);

	void set_class_name(const string& new_class_name);


	const bool& is_active() const;

	const string& get_name() const;


	const string& get_class_name() const;



	//The virtual function provides the ability to define a function in a base class 
	//and have a function of the same name and type in a derived class called when 
	//a user calls the base class function.

	//virtual print() in Parent
	//Parent* obj = new Child();
	//obj->print(); //prints 'in child'
	virtual void draw();


	virtual void click();

	virtual void hover();

	virtual void set_hovering(bool b);

};
#endif
