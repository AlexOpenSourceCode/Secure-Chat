#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;




// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFFER_LENGTH	512
#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>

#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/thread.hpp>;
#include <boost/date_time.hpp>;



#include "base.h"
#include "Server.h";

using namespace std;











namespace rmp {




	Server::Server(const std::string& _ip, const std::string& _port, const std::string& _name, const std::string& _chat_type) :
		ip(_ip),
		port(_port),
		name(_name),
		chat_type(_chat_type)
	{


	
	}


	Server::Server(const std::string& _ip, const std::string& _port) :
		ip(_ip),
		port(_port)
	{

		ip = _ip;
		port = _port;


	}




	bool Server::start_server(){
		server_started = true;


		listen_socket = initialize_socket();
		if (listen_socket == 1){
			return false;
		}
		else{
			return true;
		}

	}

	

	


		void Server::clean_socket(SOCKET listen_socket){
			server_started = false;
			// Free the resouces
			closesocket(listen_socket);
			WSACleanup();
		}


		void Server::FileReceive(char* recvbuf, int recvbuflen) {

			ofstream out("C:\\code\\c++\\output\\out.png", ios::binary);
			if (out.is_open()) {
				out.write(recvbuf, strlen(recvbuf));
				ZeroMemory(&recvbuf, recvbuflen);
			}
		}

		




		SOCKET Server::wait_for_client(SOCKET listen_socket){



			SOCKET ClientSocket;

			ClientSocket = INVALID_SOCKET;

			// Accept a client socket
			//cout << "Waiting for client..." << endl;
			ClientSocket = accept(listen_socket, NULL, NULL);


			if (ClientSocket == INVALID_SOCKET) {
				printf("accept failed: %d\n", WSAGetLastError());
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}


			client_socket = ClientSocket;
		
			return ClientSocket;
		}


		string Server::srecv() {

			int result_len = 1;
			char recvbuf[DEFAULT_BUFFER_LENGTH];
			do {
				result_len = recv(client_socket, recvbuf, DEFAULT_BUFFER_LENGTH, 0);

				if (result_len > 0) {
					try {

						string str(recvbuf, result_len);

						return str;
					}
					catch (...){
						return "";
					}
				}
				else if (result_len == 0){
					printf("Connection closed\n");
				}
				else {
					printf("SERVER recv failed: %d\n", WSAGetLastError());
					closesocket(client_socket);
					WSACleanup();
					//return 1;
				}


				return "<error>";
			} while (result_len > 1);

			return "<error>";
		}



		bool Server::Send(char* char_array, const int len){
			int iSendResult = send(client_socket, char_array, len, 0);

			if (iSendResult == SOCKET_ERROR) {

				closesocket(client_socket);
				WSACleanup();
				return false;
			}


			return true;
		}


		bool Server::Send(const string& message_to_send){
			return this->Send((char*)message_to_send.c_str(), message_to_send.length());
		}


		int string_to_int(string str){
			stringstream ss(str);
			int x = 0;
			ss >> x;

			return x;
		}


		SOCKET Server::initialize_socket() {
			WSADATA WsData;
			int ret = WSAStartup(MAKEWORD(2, 2), &WsData);
			if (ret != 0) {
				printf("Can't initialize Winsock! Error: %d", ret);
				return 1;
			}

			//Create a socket
			SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (listening == INVALID_SOCKET) {
				printf("Can't create a socket! Error: %d", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			//Bind the ip and port to a socket
			sockaddr_in hint = {};
			hint.sin_family = AF_INET;


			port = "9999";
			hint.sin_port = htons(string_to_int(port.c_str()));
			hint.sin_addr.s_addr = inet_addr(ip.c_str()); //Could also use inet_pton



			//iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
			ret = ::bind(listening, (sockaddr*)&hint, sizeof(hint));
			if (ret == SOCKET_ERROR) {
				printf("Can't bind socket! Error: %d", WSAGetLastError());
				closesocket(listening);
				WSACleanup();
				return 1;
			}

			//Tell winsock the socket is for listening
			ret = listen(listening, SOMAXCONN);
			if (ret == SOCKET_ERROR) {
				printf("Can't listen on socket! Error: %d", WSAGetLastError());
				closesocket(listening);
				WSACleanup();
				return 1;
			}


			return listening;		
		}


}