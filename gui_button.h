#pragma once
#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H


#include "gui_obj.h"
using namespace std;
#include <map>

#include "base.h"


typedef int(*int_func_noparams)();
typedef int(*int_func_map)(map<string, string>);


class gui_button : public gui_obj {
public:

	string label = "";
	string text;

	float active_r;
	float active_g;
	float active_b;



	//int_func_noparams callback_function = Base::blank_function;
	int_func_noparams callback_function;
	int_func_map callback_func_w_map_param;

	map<string, string> callback_function_params;


	gui_button(string _name, string _text, float _x, float _y, float _width, float _height, float _r = 0.3, float _g = 0.3, float _b = 0.3, float _ar = 0.1, float _ag = 0.7, float _ab = 0.5);

	void set_callback_function(int_func_map _callback_function, map<string, string> callback_func_param_map);
	void set_callback_function(int_func_noparams _callback_function);

	const string& get_text() const;

	void set_text(const string& txt);

	void draw();
	void set_hovering(bool b);
	void hover();
	void click();
};
#endif
