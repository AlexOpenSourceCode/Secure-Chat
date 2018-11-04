#include "gui.h"
#include "gui_obj.h"
#include "gui_input.h"
#include "gui_button.h"

gui::gui(){

}

void gui::add_object(shared_ptr<gui_obj>& obj){
	objects.insert({ obj->get_name(), obj });
}

void gui::draw(){
	//std::map< std::string, std::map<std::string, std::string> > mymap

	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> val = obj.second;

		val->draw();
	}


}

shared_ptr<gui_obj> gui::click(int click_x, int click_y){
	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> this_obj = obj.second;

		if ((click_x >= this_obj->x && click_x <= this_obj->x + this_obj->width) && (click_y > this_obj->y && click_y < this_obj->y + this_obj->height)){

			if (this_obj->type == "input"){
				//Unselect other inputs
				shared_ptr<gui_input> current_active_input = get_active_input();
				if (current_active_input != nullptr){
					if (this_obj->get_name() != current_active_input->get_name()){
						current_active_input->set_active(false);
					}
				}
			}
			else if (this_obj->type == "button"){
				if (this_obj->get_class_name() == "chat_button"){
					vector<shared_ptr<gui_obj>> chat_buttons = get_objects_by_class_name("chat_button");

					for (int x = 0; x < chat_buttons.size(); x++){
						chat_buttons[x]->set_active(false);
					}
				}
			}

			this_obj->click();
			return this_obj;
		}
	}

	return nullptr;
}

shared_ptr<gui_obj> gui::hover(int click_x, int click_y){
	shared_ptr<gui_obj> hover_obj = nullptr;

	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> this_obj = obj.second;
		this_obj->set_hovering(false);

		if ((click_x >= this_obj->x && click_x <= this_obj->x + this_obj->width) && (click_y > this_obj->y && click_y < this_obj->y + this_obj->height)){
			this_obj->hover();
			hover_obj = this_obj;
		}
	}

	return hover_obj;
}





shared_ptr<gui_input> gui::get_active_input(){
	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> _obj = obj.second;


		shared_ptr<gui_input> ptr(dynamic_pointer_cast<gui_input>(_obj));
		if (ptr) {
			if (ptr->active){
				return ptr;
			}

		}

	}

	return nullptr;
}


string gui::get_value(const string& gui_input_name){
	shared_ptr<gui_obj> gui_input_base_obj = get_object_by_name(gui_input_name);
	if (gui_input_base_obj == nullptr){
		return nullptr;
	}


	shared_ptr<gui_input> gui_input_obj(dynamic_pointer_cast<gui_input>(gui_input_base_obj));


	if (gui_input_obj) {
		return gui_input_obj->get_value();
	}
	else{
		return nullptr;
	}

}

shared_ptr<gui_obj> gui::get_object_by_name(const string& gui_input_name){
	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> _obj = obj.second;

		if (_obj && _obj->get_name() == gui_input_name){
			return _obj;
		}
	}

	return nullptr;
}


vector<shared_ptr<gui_obj>> gui::get_objects_by_class_name(const string& class_name){
	vector<shared_ptr<gui_obj>> obj_list;
	for (auto const &obj : objects) {
		string key = obj.first;
		shared_ptr<gui_obj> _obj = obj.second;

		if (_obj && _obj->get_class_name() == class_name){
			obj_list.push_back(_obj);
		}
	}

	return obj_list;
}


