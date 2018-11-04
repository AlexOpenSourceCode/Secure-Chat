#pragma once



#ifndef SERVER_H
#define SERVER_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include "V2ctor.h"
#include "OBB2D.h"
#include "Object.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Eigen/Dense>


using namespace std;


namespace rmp {

	class Server {
	public:

		std::string ip = "127.0.0.1";
		std::string port = "9999";

		std::string name = "default server";
		std::string chat_type;
		bool server_started = false;

		SOCKET listen_socket;
		SOCKET client_socket;


		string Server::srecv();


		bool Send(const string& message_to_send);
		bool Send(char* char_array, const int len);


		Server(const std::string& _ip, const std::string& _port, const std::string& _name, const std::string& _chat_type);


		Server(const std::string& _ip, const std::string& _port);

		SOCKET initialize_socket();

		int read_thread(SOCKET listen_socket, SOCKET client_socket);

		SOCKET wait_for_client(SOCKET listen_socket);

		void FileReceive(char* recvbuf, int recvbuflen);

		void clean_socket(SOCKET listen_socket);


		bool start_server();

		int start_socket_server();


		vector<string> get_chat_messages();






	};

}


#endif



