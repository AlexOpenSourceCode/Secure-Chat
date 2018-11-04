#include "App.h"
#include "gui_button.h"

//rando on stack overflow
//As a rule, put your includes in the.cpp files when you can, and only in the.h files when that is not possible.


typedef int(*int_func_noparams)();
typedef int(*int_func_map)(map<string, string>);



App::App(){

}

void App::add_chat(shared_ptr<Chat>& active_chat, int_func_map external_set_active_chat){

	active_chat->set_file_download_path(file_download_path);

	chats.push_back(active_chat);


	int last_chat_index = chats.size() - 1;
	string index_string = to_string(last_chat_index);

	float list_item_start_y = 250;

	list_item_start_y += last_chat_index * 50;


	shared_ptr<gui_button> list_chat_button = make_shared<gui_button>("list_chat_" + index_string, active_chat->get_chat_type() + " - " + active_chat->get_ip_and_port_string(), 0, list_item_start_y, 200, 50);

	map<string, string> callback_params;
	callback_params.insert({ "chat_index", index_string });


	list_chat_button->set_callback_function(external_set_active_chat, callback_params);
	list_chat_button->set_class_name("chat_button");



	if (last_chat_index == 0){
		list_chat_button->set_active(true);
	}


	my_gui->add_object(dynamic_pointer_cast<gui_obj>(list_chat_button));
}

void App::set_active_chat(int chat_index){
	if (chat_index < chats.size()){
		active_chat_index = chat_index;
	}
}

shared_ptr<Chat> App::get_chat(int chat_index){
	if (chat_index < chats.size()){
		return chats[chat_index];
	}

	return nullptr;
}

void App::draw(){
	my_gui->draw();
}

shared_ptr<Chat> App::get_active_chat(){
	return get_chat(active_chat_index);
}



void App::set_file_download_path(const string& new_path){
	file_download_path = new_path;

	for (const auto& obj : chats){
		obj->set_file_download_path(new_path);
	}
}

const string& App::get_file_download_path() const {
	return file_download_path;
}



shared_ptr<Chat> App::find_chat(const string& client_or_server, const string& _ip, const string& _port){
	for (const auto& chat : chats){
		if (chat->get_chat_type() == client_or_server && chat->get_ip() == _ip && chat->get_port() == _port){
			return chat;
		}
	}
	return nullptr;
}


bool App::chat_already_exists(const string& client_or_server, const string& _ip, const string& _port){
	for (const auto& chat : chats){

		//cout << "chat_already_exists?: " << chat->get_chat_type() << " " << chat->get_ip() << " " << chat->get_port() << endl;
		//cout << "params?: " << client_or_server << " " << _ip << " " << _port << endl;

		if (chat->get_chat_type() == client_or_server && chat->get_ip() == _ip && chat->get_port() == _port){
			return true;
		}
	}

	return false;
}

