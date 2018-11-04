#pragma once
#ifndef APP_H
#define APP_H



#include "Chat.h"
#include "gui.h"

typedef int(*int_func_noparams)();
typedef int(*int_func_map)(map<string, string>);

class App {
public:

	string file_download_path = "D:/c++/output/";
	

	vector<shared_ptr<Chat>> chats;
	shared_ptr<gui> my_gui = make_shared<gui>();

	int active_chat_index = 0;


	App();

	void add_chat(shared_ptr<Chat>& active_chat, int_func_map external_set_active_chat);



	void set_active_chat(int chat_index);

	shared_ptr<Chat> get_chat(int chat_index);


	void draw();

	void set_file_download_path(const string& new_path);
	const string& get_file_download_path() const;


	shared_ptr<Chat> get_active_chat();



	bool chat_already_exists(const string& client_or_server, const string& _ip, const string& _port);


	shared_ptr<Chat> find_chat(const string& client_or_server, const string& _ip, const string& _port);
};

#endif
