#pragma once



#ifndef GUI_H
#define GUI_H



#include "Chat.h"
#include "gui_obj.h"
#include "gui_input.h"
#include <memory>

class gui {
public:
	//vector<shared_ptr<gui_obj>> objects;

	map<string, shared_ptr<gui_obj>> objects;

	

	gui();


	


	void add_object(shared_ptr<gui_obj>& obj);
	void draw();

	shared_ptr<gui_obj> click(int click_x, int click_y);

	shared_ptr<gui_obj> hover(int click_x, int click_y);


	shared_ptr<gui_input> get_active_input();


	shared_ptr<gui_obj> get_object_by_name(const string& gui_input_name);
	vector<shared_ptr<gui_obj>> get_objects_by_class_name(const string& class_name);

	string get_value(const string& gui_input_name);


};




#endif