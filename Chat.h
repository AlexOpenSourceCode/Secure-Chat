#pragma once



#ifndef CHAT_H
#define CHAT_H

//#define WIN32_LEAN_AND_MEAN


#define DEFAULT_BUFFER_LENGTH	512

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


#define DEFAULT_BUFFER_LENGTH	512

#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>



#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <Eigen/Dense>

#include <math.h>       /* ceil */


#include "base.h"


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


#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;


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


using namespace std;




#include "Server.h"
#include "Client.h"

#include "RSAObj.h"
#include "AESObj.h"

#include <stdio.h>


class Chat {
public:
	bool DEBUG = true;

	int RECEIVE_TEXT = 0;
	int RECEIVE_FILE = 1;
	int RECEIVE_AES_ENCRYPTED = 2;
	int RECEIVE_RSA_ENCRYPTED = 3;
	int RECEIVE_PUBLIC_KEY = 4;
	int RECEIVE_AES_KEY = 5;
	int RECEIVE_AES_IV = 6;
	int RECEIVE_END_FILE = 7;

	std::string TEXT_COMMAND = "<TEXT>";
	std::string FILE_COMMAND = "<FILE>";
	std::string END_FILE_COMMAND = "<////FILE>";
	std::string AES_ENCRYPTED_COMMAND = "<AES_ENCRYPTED>";
	std::string RSA_ENCRYPTED_COMMAND = "<RSA_ENCRYPTED>";
	std::string PUBLIC_KEY_COMMAND = "<PUBLIC_KEY>";
	std::string AES_KEY_COMMAND = "<AES_KEY>";
	std::string AES_IV_COMMAND = "<AES_IV>";


	std::string END_COMMAND = "</END>";


	std::string HEADER_STRING = "HEADER";
	std::string HEADER_SEPERATOR = "="; // dont use _ because its in the command
	std::string HEADER_END = ":";

	//  HEAD_<TEXT>_len(data)
	//  HEAD_<TEXT>_56:


	std::map<std::string, int> create_command_map(){
		std::map<std::string, int> m;
		m[TEXT_COMMAND] = RECEIVE_TEXT;
		m[FILE_COMMAND] = RECEIVE_FILE;
		m[END_FILE_COMMAND] = RECEIVE_END_FILE;
		m[AES_ENCRYPTED_COMMAND] = RECEIVE_AES_ENCRYPTED;
		m[RSA_ENCRYPTED_COMMAND] = RECEIVE_RSA_ENCRYPTED;
		m[PUBLIC_KEY_COMMAND] = RECEIVE_PUBLIC_KEY;
		m[AES_KEY_COMMAND] = RECEIVE_AES_KEY;
		m[AES_IV_COMMAND] = RECEIVE_AES_IV;
		return m;
	}

	std::map<std::string, int> commands = create_command_map();

	//m["hello"] = 23;
	// check if key is present
	//if (m.find("world") != m.end())
	//	std::cout << "map contains key world!\n";
	// retrieve
	//std::cout << m["hello"] << '\n';
	//std::map<std::string, int>::iterator i = m.find("hello");


	// a global instance of std::mutex to protect global variable
	std::mutex myMutex;

	std::vector<std::string> chat_messages;

	Server* server = nullptr;
	Client* client = nullptr;



	std::string message_to_send = "";
	bool send_message_flag = false;
	std::mutex mutex_2;


	boost::thread* read_thread_ = nullptr;
	boost::thread* send_thread_ = nullptr;


	bool is_server = false;
	bool is_client = false;


	FILE* output_file = NULL;

	//server - is running
	//client - is connected
	bool active = false;



	RSAObj* rsa_obj = nullptr;

	AESObj* aes_obj = nullptr;

	string iv = "";
	string key = "";

	string chat_name = "";


	string file_download_path = "D:/c++/output/";


	void set_file_download_path(const string& new_path);
	const string& get_file_download_path() const;





	RSA::PublicKey* other_public_key = nullptr;


	Chat(Server* _server, Client* _client);


	Chat(Server* _server);

	Chat(Client* _client);

	Chat(string SERVER_OR_CLIENT, string ip, string port);

	Chat();

	~Chat();
	


	void print(const string& msg);
	void send_message(const string& msg);

	void add_message(const string& str, const string& receive_or_send_string);
	void add_message(const string& str);

	vector<string> get_chat_messages();



	float last_set_ms = 0;


	//Thing(const Thing& g){
	//	name = g.get_name();
	//}


	////allows for
	////  const string& lol_cref = thingOne.get_name();
	////which uses the same memory as class for string
	////or just string lol = thingOne.get_name(); for a string copy
	//const string& get_name() const {
	//	return name;
	//}
	const string& get_name() const;




	void client_read_thread();


	void send_aes_encrypted_message(string unencrypted_msg);


	void send_sym_key();

	void send_sym_iv();


	bool is_at_end_of_string(const string& needle, const string& hay);

	int RECEIVE_STATE_COMMAND = RECEIVE_TEXT;


	//Create new message header
	// HEAD_<TEXT>_56:
	string create_msg_header(const string& command, int data_size);



	//Create new message header
	// HEAD_<TEXT>_56:
	string create_encrypted_msg_header(const string& command, const string& encrypted_text, const string& hmac_sig);


	string create_file_msg_header(string file_path);


	//string create_msg(const string& command, const string& data){
	//	string msg = create_msg_header(command, data.size());
	//	msg += data;
	//	return create_msg_header(command, data.size()) + data;
	//}


	void chat_read_thread();


	void send(const string& lol);
	void send(char* char_array, const int char_array_len);




	void chat_send_thread();


	void add_client(Client* c);

	int start_chat_thread();


	string hmac_sign(string key, string plain);


	string get_chat_type();

	string get_ip_and_port_string();

	string get_ip();

	string get_port();

	string get_from_string(string send_or_receive);


	void send_file(string file_path);


	void receive_file_2(string file_name, string file_ext, char* recvbuf, int recvbuflen, FILE* pFile);
	void receive_file(string file_name, string file_ext, char* recvbuf, int recvbuflen);



	int message_count();

	string get_message(int msg_index);


	const bool is_active() const;

	int start_client();
	int start_server();
};



#endif