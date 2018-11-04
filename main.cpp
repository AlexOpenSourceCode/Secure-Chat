//#define WIN32_LEAN_AND_MEAN

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>

#include "shader_loader.h"

#include "Object.h"
#include "Player.h"
#include "Spell.h"
#include "Server.h"

//#include <thread>         // std::thread



#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>



#include <string>


#include <iostream>
#include <fstream>
#include <cstring> // for std::strlen
#include <cstddef> // for std::size_t -> is a typedef on an unsinged int

#include <iostream>


#include <array>


// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")



#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>

using namespace rmp;

using namespace Shaders;

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <Eigen/Dense>
#include <math.h>       /* ceil */
#include "base.h"
//add public to inheritance to fix:
//type cast conversion from child* to parent* exists, but inaccessible

#include "App.h"

#include "gui.h"


using Eigen::Vector3f;

//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle = 0.0;

float cRadius = 20.0f; // our radius distance from our character

float lastx, lasty;

//positions of the cubes
float positionz[10];
float positionx[10];


//bool* keyStates = new bool[256]; // Create an array of boolean values of length 256 (0-255)  
bool keyStates[256] = { 0 };

float mouse_sensitivity = 0.3f;
float movement_sensitivity = 0.05f;


int window_width = 1000;
int window_height = 650;


int center_x = window_width / 2;
int center_y = window_height / 2;


bool server_is_running = false;
bool client_is_running = false;


using namespace std;


float ORG[3] = { 0, 0, 0 };
float w, h, tip = 0, turn = 0;

float XP[3] = { 1, 0, 0 }, XN[3] = { -1, 0, 0 },
YP[3] = { 0, 1, 0 }, YN[3] = { 0, -1, 0 },
ZP[3] = { 0, 0, 1 }, ZN[3] = { 0, 0, -1 };

# define M_PI           3.14159265358979323846  /* pi */

#include <vector>
#include <memory>
#include <vector>



bool chat_input_active = false;
std::string chat_input_text = "";



#define _CRT_SECURE_NO_DEPRECATE
#include <cstdio>
#include <iostream>
#include <osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include <cryptlib.h>
using CryptoPP::Exception;

#include <hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <des.h>
using CryptoPP::DES_EDE2;

#include <modes.h>
using CryptoPP::CBC_Mode;

#include <secblock.h>
using CryptoPP::SecByteBlock;
#include <iostream>
#include <string>
#include <modes.h>
#include <aes.h>
#include <filters.h>

#include "rsa.h"
using CryptoPP::RSA;
using CryptoPP::InvertibleRSAFunction;
using CryptoPP::RSAES_OAEP_SHA_Encryptor;
using CryptoPP::RSAES_OAEP_SHA_Decryptor;

#include "sha.h"
using CryptoPP::SHA1;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::PK_EncryptorFilter;
using CryptoPP::PK_DecryptorFilter;

#include "files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::BufferedTransformation;

#include "SecBlock.h"
using CryptoPP::SecByteBlock;

#include "cryptlib.h"
using CryptoPP::Exception;
using CryptoPP::DecodingResult;
using CryptoPP::ByteQueue;

#include <string>
using std::string;

#include <exception>
using std::exception;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>
#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
#include "aes.h"
using CryptoPP::AES;
#include "ccm.h"
using CryptoPP::CBC_Mode;
#include "assert.h"
//boost build
//ms-t -> b2 runtime-link=static -mt
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hmac.h"
using CryptoPP::HMAC;

#include "sha.h"
using CryptoPP::SHA256;

#include "base64.h"
//using CryptoPP::Base64Encoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;



#include "Client.h";

#include <map>
#include "base64.h";
#include "RSAObj.h"

using namespace std;

#include "Chat.h"


#include "gui.h"
#include "gui_input.h"
#include "gui_button.h"
#include "gui_text_view.h"


Client* active_client = nullptr;
Client* oclient = nullptr;
Server* active_server = nullptr;



float circle_number(float num){
	if (num >= 360.0){
		num = 0.0;
	}
	else if (num <= 0.0){
		num = 360 + num;
	}
	return num;
}


vector<string> split(const string& s, const string& delim, const bool keep_empty = true) {
	vector<string> result;
	if (delim.empty()) {
		result.push_back(s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());
		string temp(substart, subend);
		if (keep_empty || !temp.empty()) {
			result.push_back(temp);
		}
		if (subend == s.end()) {
			break;
		}
		substart = subend + delim.size();
	}
	return result;
}



void draw_axes2(void){
	// save previous matrix
	glPushMatrix();

		// clear matrix
		glLoadIdentity();
		// apply rotations
		glRotatef(xrot, 1.0, 0.0, 0.0);
		glRotatef(yrot, 0.0, 1.0, 0.0);
		glRotatef(0, 0.0, 0.0, 1.0);
		// move the axes to the screen corner
		glTranslatef(0.0, 0.05, 0.0);
		// draw our axes



		float size = 0.1;


		glBegin(GL_LINES);

			//RGB
			// X = RED
			// Y = GREEN
			// Z = BLUE

			// draw line for x axis
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(size, 0.0, 0.0);
			// draw line for y axis
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, size, 0.0);
			// draw line for Z axis
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, size);
		glEnd();
	// load the previous matrix
	glPopMatrix();
}


void draw_axes(void) {
	glPushMatrix();

	glLineWidth(2.0);

	glBegin(GL_LINES);
		glColor3f(1, 0, 0); // X axis is red.
		glVertex3fv(ORG);

		float x_position[3] = { xpos, 0, 0 };
		glVertex3fv(x_position);

		glColor3f(0, 1, 0); // Y axis is green.
		glVertex3fv(ORG);

		float y_position[3] = { 0, ypos, 0 };
		glVertex3fv(y_position);

		glColor3f(0, 0, 1); // z axis is blue.
		glVertex3fv(ORG);

		float z_position[3] = { 0, 0, zpos };
		glVertex3fv(z_position);

	glEnd();

	glPopMatrix();
}





void cubepositions(void) { //set the positions of the cubes

	for (int i = 0; i<10; i++) {
		positionz[i] = rand() % 5 + 1;
		positionx[i] = rand() % 5 + 1;
	}
}




//draw the cube
void cube(void) {
	for (int i = 0; i<10 - 1; i++) {
		glPushMatrix();
		glTranslated(-positionx[i + 1] * 10, 0, -positionz[i + 1] *
			10); //translate the cube
		glutSolidCube(2); //draw the cube
		glPopMatrix();
	}
}

void init(void) {
	cubepositions();
}

void enable(void) {
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH); //set the shader to smooth shader

}




int counter = 0;


//Determines how many lines of text there would be of a string in a given width
int compute_line_count(const string& str, float width, float char_width = 9.0){
	size_t str_length = str.length();
	int line_count = ceil((str_length * char_width) / width);
	return line_count;
}


string string_multiply(string str, int num){
	string new_str = "";
	for (int x = 0; x < num; x++){
		new_str += str;
	}

	return new_str;
}


typedef int(*int_func_noparams)();
typedef int(*int_func_map)(map<string, string>);


shared_ptr<App> my_app = std::make_shared<App>();

int external_set_active_chat(map<string, string> param_map){
	int new_active_chat_index = stoi(param_map["chat_index"]);

	//cout << "Setting active chat to : " << new_active_chat_index << endl;

	my_app->set_active_chat(new_active_chat_index);
	return 1;
}



void draw_gui(){
		/*glBegin(GL_QUADS);
			glVertex3f(20.0f, 20.0f, 0.0f);
			glVertex3f(20.0f, -20.0f, 0.0f);
			glVertex3f(-20.0f, -20.0f, 0.0f);
			glVertex3f(-20.0f, 20.0f, 0.0f);
		glEnd();*/

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, window_width, window_height, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);


	//DRAW IN 2D
	//Draw action bars
	float action_bar_width = 200;
	float action_bar_height = 500;
	float action_bar_bottom_gap = 10;

	float action_bar_x = 0;
	float action_bar_y = 0;


	//glEnd();my_gui

	my_app->draw();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewportto the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}



bool objects_collide(Object a, Object b) {
	//check the X axis
	if (abs(a.get_x() - b.get_x()) < a.get_width() + b.get_width())
	{
		//check the Y axis
		if (abs(a.get_y() - b.get_y()) < a.get_height() + b.get_height())
		{
			//check the Z axis
			if (abs(a.get_z() - b.get_z()) < a.get_height() + b.get_height())
			{
				return true;
			}
		}
	}

	return false;
}




bool move_camera = false;

//Stores the position the user right clicked at to start moving camera
float mouse_camera_x = 0.0;
float mouse_camera_y = 0.0;

void mouse_click(int button, int state, int x, int y){

	if (button == GLUT_LEFT_BUTTON ){
		if (state == GLUT_DOWN){
			shared_ptr<gui_obj> clicked_gui_object = my_app->my_gui->click(x, y);
			if (clicked_gui_object == nullptr){
				//cout << "NO GUI OBJECT WAS CLICKED" << endl;
			}
			else{
				//cout << "GUI OBJECT <" << clicked_gui_object->get_name() << " was clicked " << endl;
			}

		}
		else {

		}
	}


}



void print_vector(const vector<string>& v){
	//cout << " vector<";
	for (size_t x = 0; x < v.size(); x++) {
		//cout << v[x];
		if (x + 1 < v.size()){
			//cout << ", ";
		}
	}
	//cout << ">" << endl;
}



bool warped = false;

void mouseMovement(int x, int y) {
	static bool wrap = false;

	

	//int diffx = x - lastx; //check the difference between the current x and the last x position
	//int diffy = y - lasty; //check the difference between the current y and the last y position lastx = x; //set lastx to the current x position


	////cout << x << endl;
	////cout << lastx << endl;
	////cout << diffx << endl;


	//lasty = y; //set lasty to the current y position
	//lastx = x;


	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);


	float dx = (mouse_camera_x) - x;
	float dy = (mouse_camera_y) - y;


	float x_rotation_change = (float)dy * mouse_sensitivity * -1;
	float y_rotation_change = (float)dx * mouse_sensitivity * -1;


	my_app->my_gui->hover(x, y);


}



int start_client(){

	string ip = my_app->my_gui->get_value("ip_input");
	string port = my_app->my_gui->get_value("port_input");

	client_is_running = true;
	shared_ptr<Chat> client_chat = make_shared<Chat>("client", ip, port);
	my_app->add_chat(client_chat, external_set_active_chat);

	return 0;
}

int start_server(){
	//cout << "start_server();" << endl;

	if (!server_is_running){

		//cout << "STARTING SERVER THREAD" << endl;
		server_is_running = true;

		string ip = my_app->my_gui->get_value("ip_input");
		string port = my_app->my_gui->get_value("port_input");

		shared_ptr<Chat> active_chat = make_shared<Chat>("SERVER", ip, port);

		my_app->add_chat(active_chat, external_set_active_chat);

	}

	return 1;
}



string clean_msg_string(const string& msg_str){
	string new_string = msg_str;
	string server_string = "server: ";
	string client_string = "client: ";

	if (msg_str.find(server_string) == 0){
		new_string = msg_str.substr(server_string.size(), msg_str.size());
	}
	else if (msg_str.find(client_string) == 0){
		new_string = msg_str.substr(client_string.size(), msg_str.size());
	}

	return new_string;
}


int previous_message_index_from_end = -1;

void keyPressed(unsigned char key, int x, int y) {
	//KEY CODE VALUES
	//http://nehe.gamedev.net/article/msdn_virtualkey_codes/15009/
	
	////cout << "key: " << key << endl;
	shared_ptr<gui_input> active_input = my_app->my_gui->get_active_input();

	if (active_input){

		const string& this_input_value = active_input->get_value();
		
		if (active_input->get_name() == "chat_input"){

			shared_ptr<Chat> active_chat = my_app->get_active_chat();
			if (active_chat == nullptr){
				//cout << "keyPressed() enter: No active chat" << endl;
				return;
			}

			if (key == VK_RETURN) {
				previous_message_index_from_end = 0;
				if (this_input_value.length() > 0){
					
					//active_chat->send_message(this_input_value);

					//cout << this_input_value << endl;
					//Check if the chat_input_text was a command
					if (this_input_value.at(0) == '$'){
						//cout << this_input_value << " $$$" << endl;
						vector<string> command_parts = Base::split(this_input_value.substr(1, this_input_value.size()), ' ');

						vector<string> commands = { "start server", "start client", "send_file C:/file/path", "help"};


						if (command_parts[0] == "start"){
							if (command_parts[1] == "server"){
								start_server();
							}
							else if (command_parts[1] == "client"){
								start_client();
							}
						}
						else if (command_parts[0] == "send_file"){
							string file_path = command_parts[1];
							active_chat->send_file(file_path);
						}
						else if (command_parts[0] == "help"){
							active_chat->add_message("===== Help - commands ==================");
							for (const string& cmd : commands){
								active_chat->add_message(cmd);
							}
							active_chat->add_message("========================================");
						}
					}
					else{
						active_chat->send_aes_encrypted_message(this_input_value);
					}

					active_input->set_value("");
				}
				else{
					active_input->set_active(false);
				}
			}
			else if (key == VK_UP){
				////cout << "UP KEY PRESSED" << endl;
				previous_message_index_from_end += 1;
				if (previous_message_index_from_end > active_chat->message_count()){
					previous_message_index_from_end = active_chat->message_count();
				}

				////cout << "previous_message_index_from_end: " << previous_message_index_from_end << endl;
				active_input->set_value(clean_msg_string(active_chat->get_message(active_chat->message_count() - previous_message_index_from_end)));
			}
			else if (key == VK_DOWN){
				////cout << "DOWN KEY PRESSED" << endl;

				previous_message_index_from_end -= 1;
				if (previous_message_index_from_end < 0){
					previous_message_index_from_end = 0;
				}

				////cout << "previous_message_index_from_end: " << previous_message_index_from_end << endl;
				active_input->set_value(clean_msg_string(active_chat->get_message(active_chat->message_count() - previous_message_index_from_end)));
			}
			else{

				string current_value = active_input->get_value();
				if (key == VK_BACK){

					if (current_value.length() > 0){
						current_value.pop_back();
					}
				}
				else{
					current_value += key;
				}
				active_input->set_value(current_value);

			}

			
		}
		else{
			active_input->set_active(false);
		}
	}
	else{
		chat_input_active = true;
	}




	if (key == VK_RETURN){
		//cout << "ENTER KEY WAS PRESSED" << endl;

		

		//std::cout << "CHAT INPUT ACTIVE: " << chat_input_active << endl;
	}
	else if (key == '['){
		start_server();
	}
	else if (key == ']'){
		start_client();
	}
	else {
		
	}

	
}

void keyUp(unsigned char key, int x, int y) {

	keyStates[key] = false; // Set the state of the current key to not pressed  
}




void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		keyPressed(VK_UP, x, y);
		break;
	case GLUT_KEY_DOWN:
		keyPressed(VK_DOWN, x, y);
		break;
	case GLUT_KEY_LEFT:
		//do something here
		break;
	case GLUT_KEY_RIGHT:
		//do something here
		break;
	}
}




void keyOperations(void) {



	//float scale = 0.0001f;
	if (keyStates['w']) {

	}

}




#include <chrono>


using namespace std::chrono;




milliseconds last_render_ms;

//RENDER SCENE
void render_scene(void) {

	milliseconds this_render_ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
	);
	



	glClearColor(0.0, 0.0, 0.0, 1.0); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	keyOperations();
	//clear the color buffer and the depth buffer
	enable();

	glLoadIdentity();

	//check_collisions();

	glTranslatef(0.0f, 0.0f, -cRadius);
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glColor3f(1.0f, 0.0f, 0.0f);

	

	//glRotatef(yrot + 90, 0.0, 1.0, 0.0);  //rotate our camera on the y - axis(up and down)
	glRotatef(yrot, 0.0, 1.0, 0.0);  //rotate our camera on the y - axis(up and down)
	glTranslated(-xpos, 0.0f, -zpos); //translate the screento the position of our camera
	glColor3f(1.0f, 1.0f, 1.0f);

	


	glColor3f(0.0, 1.0, 0.0); //clear the screen to black





	draw_gui();

	reshape(window_width, window_height);

	glutSwapBuffers(); //swap the buffers
	angle++; //increase the angle



	last_render_ms = this_render_ms;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}


#include <iostream>

void xerr(const char* msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}



void toClipboard(const std::string &s){
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}


string get_clipboard_text(){
	char * buffer;
	if (OpenClipboard(0))
	{

		buffer = (char*)GetClipboardData(CF_TEXT);
		//do something with buffer here 
		//before it goes out of scope
		return buffer;
	}

	CloseClipboard();

	//xclip -selection c
}



int main(int argc, char **argv) {
	shared_ptr<gui_obj> ip_input = make_shared<gui_input>("ip_input", "IP", "127.0.0.1", 0, 0, 200, 50);
	shared_ptr<gui_obj> port_input = make_shared<gui_input>("port_input", "Port", DEFAULT_PORT, 0, 50, 200, 50);
	

	shared_ptr<gui_button> client_connect_button = make_shared<gui_button>("client_connect_button", "Connect to server", 0, 100, 200, 50);
	client_connect_button->set_callback_function(start_client);


	shared_ptr<gui_button> server_host_button = make_shared<gui_button>("server_host_button", "Host server", 0, 150, 200, 50);
	server_host_button->set_callback_function(start_server);


	shared_ptr<gui_obj> file_download_path_input = make_shared<gui_input>("file_download_path_input", "Download path", "C:/code/c++/output", 0, 200, 200, 50);



	shared_ptr<gui_text_view> chat_text_view = make_shared<gui_text_view>("chat_text_view", 200, 0, 800, 500);
	chat_text_view->set_app(my_app);

	shared_ptr<gui_obj> chat_input = make_shared<gui_input>("chat_input", "Type message here...", "", 210, 455, 450, 40);


	my_app->my_gui->add_object(ip_input);
	my_app->my_gui->add_object(port_input);
	my_app->my_gui->add_object(dynamic_pointer_cast<gui_obj>(client_connect_button));
	my_app->my_gui->add_object(dynamic_pointer_cast<gui_obj>(server_host_button));
	my_app->my_gui->add_object(dynamic_pointer_cast<gui_obj>(chat_text_view));
	my_app->my_gui->add_object(chat_input);
	my_app->my_gui->add_object(file_download_path_input);
	

	//system("pause");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Secure Chat - Project 2");
	init();
	glutDisplayFunc(render_scene);
	glutIdleFunc(render_scene);
	glutReshapeFunc(reshape);

	glutMotionFunc(mouseMovement); //for Active motion
	glutPassiveMotionFunc(mouseMovement); // for Passive motion


	glutMouseFunc(mouse_click);

	glutPassiveMotionFunc(mouseMovement); //check for mousemovement

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(SpecialInput);
	glutMainLoop();


	

	std::system("pause");
	return 0;
}