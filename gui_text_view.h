#pragma once
#ifndef GUI_TEXT_VIEW_H
#define GUI_TEXT_VIEW_H

#include "gui_obj.h"
#include "App.h"

class gui_text_view : public gui_obj {
public:

	bool active;
	string text;

	float active_r;
	float active_g;
	float active_b;

	gui_text_view(string _name, float _x, float _y, float _width, float _height, float _r = 0.3, float _g = 0.3, float _b = 0.3, float _ar = 0.1, float _ag = 0.7, float _ab = 0.5);


	shared_ptr<App> my_app = nullptr;


	//	////allows for
	//	////  const string& lol_cref = thingOne.get_name();
	//	////which uses the same memory as class for string
	//	////or just string lol = thingOne.get_name(); for a string copy
	//	//const string& get_name() const {
	//	//	return name;

	const string& get_text() const;
	void set_text(const string& txt);
	void set_active(bool b);
	void draw();
	void click();


	void set_app(shared_ptr<App> _my_app);
};
#endif
