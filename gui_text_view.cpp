#include "gui_text_view.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream> //for //cout


#include "base.h"
#include "gui_globals.h"




//gui_text_view::gui_text_view(string _name, float _x, float _y, float _width, float _height, float _r = 0.3, float _g = 0.3, float _b = 0.3, float _ar = 0.1, float _ag = 0.7, float _ab = 0.5) : gui_obj(_name, _x, _y, _width, _height, _r, _g, _b), active_r(_ar), active_g(_ag), active_b(_ab), active(false) {
//	type = "input";
//	text = "Button";
//}

gui_text_view::gui_text_view(string _name, float _x, float _y, float _width, float _height, float _r, float _g, float _b, float _ar, float _ag, float _ab) : gui_obj(_name, _x, _y, _width, _height, _r, _g, _b), active_r(_ar), active_g(_ag), active_b(_ab), active(false) {
	type = "input";
	text = "Button";
}

//	//const string& gui_text_view::string&() const {
//	//	return name;
//	//}

const string& gui_text_view::get_text() const {
	return text;
}

void gui_text_view::set_text(const string& txt){
	text = txt;
}

void gui_text_view::set_active(bool b){
	active = b;
}

void gui_text_view::draw(){
	float chat_margin = 10;

	float chat_x = 200;
	//float chat_y = window_height - chat_height - 10;
	float chat_y = 0;



	float chat_input_margin = chat_margin;
	float chat_input_width = width - (chat_input_margin * 2);
	float chat_input_height = 30;
	float chat_input_x = chat_x + chat_input_margin;
	float chat_input_y = chat_y + height - chat_input_height - chat_input_margin;

	float chat_text_margin = 5;
	float chat_text_x = chat_x + chat_margin;
	float chat_text_y = chat_y + chat_margin;
	float chat_text_width = width - (chat_margin * 2);
	float chat_text_height = height - chat_input_height - (chat_input_margin * 3);
	//float chat_text_height = 150.0 + (chat_input_margin + (chat_input_margin / 2));

	float chat_message_inline_height = 15;


	int visible_messages_count = chat_text_height / chat_message_inline_height;

	shared_ptr<Chat> active_chat_ptr = my_app->get_active_chat();

	if (active_chat_ptr == nullptr){
		return;
	}

	vector<string> chat_messages = active_chat_ptr->get_chat_messages();
	int message_loop_start = chat_messages.size() - visible_messages_count;


	if (message_loop_start < 0){
		message_loop_start = 0;
	}

	int message_count = 0;
	int rendered_messages_height = 0;

	//Start from last message going backwards
	int message_index = chat_messages.size();
	if (message_index > 0){
		message_index--;
	}


	if (chat_messages.size() > 0){
		while (rendered_messages_height < chat_text_height && message_index > -1){
			float this_message_height = chat_message_inline_height;

			glColor3f(0, 1, 0);

			int lines_of_text = Base::compute_line_count(chat_messages[message_index], chat_text_width);

			if (lines_of_text > 1){
				this_message_height *= lines_of_text;
			}

			float chat_height_remaining = chat_text_height - rendered_messages_height;

			if (this_message_height > chat_height_remaining) {
				this_message_height = chat_height_remaining;
			}

			if (chat_height_remaining > chat_message_inline_height){
				gui_globals::draw_text(chat_messages[message_index], chat_x + chat_margin + chat_text_margin, chat_y + (chat_margin * 2) + chat_text_height - rendered_messages_height - this_message_height, chat_text_width, chat_height_remaining, true);
			}
			else{
				break;
			}
			rendered_messages_height += this_message_height;

			message_index--;
			message_count++;
		}
	}


	//if (chat_input_active){
	//	glColor3f(1, 1, 1);
	//	//Draw chat_input_text
	//	gui_globals::draw_text(chat_input_text, chat_input_x + 5, chat_input_y + chat_margin, chat_input_width - 5);
	//}



	//draw chat
	glBegin(GL_QUADS);

	glColor3f(116.0 / 256.0, 36.0 / 256.0, 145.0 / 256.0);
	//Top left
	glVertex2f(chat_text_x, chat_text_y);
	//Top right
	glVertex2f(chat_text_x + chat_text_width, chat_text_y);

	//Bottom right
	glVertex2f(chat_text_x + chat_text_width, chat_text_y + chat_text_height);

	//Bottom left
	glVertex2f(chat_text_x, chat_text_y + chat_text_height);


	//DRAW CHAT FEED INPUT
	//if (chat_input_active){
	//	glColor3f(0.0f, 0.5f, 0.7f);
	//}
	//else{
	//	glColor3f(0.0f, 0.0f, 0.9f);
	//}
	////Top left
	//glVertex2f(chat_input_x, chat_input_y);
	////Top right
	//glVertex2f(chat_input_x + chat_input_width, chat_input_y);

	////Bottom right
	//glVertex2f(chat_input_x + chat_input_width, chat_input_y + chat_input_height);

	////Bottom left
	//glVertex2f(chat_input_x, chat_input_y + chat_input_height);


	//DRAW CHAT BACKGROUND
	glColor3f(0.5f, 0.3f, 0.91f);

	//Top left
	glVertex2f(chat_x, chat_y);
	//Top right
	glVertex2f(chat_x + width, chat_y);

	//Bottom right
	glVertex2f(chat_x + width, chat_y + height);

	//Bottom left
	glVertex2f(chat_x, chat_y + height);
	glEnd();
}

void gui_text_view::click() {

	std::cout << "BUTTON " << text << " clicked" << endl;
}




void gui_text_view::set_app(shared_ptr<App> _my_app){
	my_app = _my_app;
}