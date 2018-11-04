//#define WIN32_LEAN_AND_MEAN


#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;

#include <cstdint>
#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>

#include "shader_loader.h";

#include "Object.h";
#include "Player.h";
#include "Spell.h";

//#include <thread>         // std::thread



#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>



#include <string>


#include <iostream>
#include <fstream>
#include <cstring> // for std::strlen
#include <cstddef> // for std::size_t -> is a typedef on an unsinged int

#include <iostream>;


#include <array>;


// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>

using namespace rmp;

using namespace Shaders;

#include <boost/thread.hpp>;
#include <boost/date_time.hpp>;

#include <Eigen/Dense>

#include <math.h>       /* ceil */



//using namespace Eigen;
//using namespace std;

using Eigen::Vector3f;

#include "Client.h"

#include <WinSock2.h>



    Client::Client(char* servername) {
		szServerName = servername;
		ConnectSocket = INVALID_SOCKET;
	}

	Client::Client(string _ip, string _port) {
		ip = _ip;
		port = _port;
		szServerName = (char *)_ip.c_str();
		ConnectSocket = INVALID_SOCKET;
	}


	Client::Client(SOCKET _connect_socket){
		ConnectSocket = _connect_socket;
	}

	SOCKET Client::get_socket() {
		return ConnectSocket;
	}


	bool Client::Start() {
		WSADATA wsaData;

		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			return false;
		}

		struct addrinfo	*result = NULL,
			*ptr = NULL,
			hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
		//iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return false;
		}

		ptr = result;

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Error at socket(): %d\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}



		//int sendbuff = 256000;

		//// re-use already bound address/port (if possible)
		//if (setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sendbuff, sizeof(sendbuff)) < 0)
		//	//cout << "Cannot set SO_REUSEADDR option on listen socket (%s)\n" << endl;

		//// re-use already bound address/port (if possible)
		//if (setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVBUF, (char *)&sendbuff, sizeof(sendbuff)) < 0)
		//	//cout << "Cannot set SO_RCVBUF option on listen socket (%s)\n" << endl;




		// Connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return false;
		}


		//boost::thread client_wait_for_messages_thread(&Client::wait_for_messages, this, ConnectSocket);
		started = true;
		return true;
	};

	// Free the resouces
	void Client::Stop() {
		int iResult = shutdown(ConnectSocket, SD_SEND);

		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
		}

		closesocket(ConnectSocket);
		WSACleanup();

		started = false;
	};




	// Send message to server
	bool Client::Send(char* char_array, const int len) {

		int iResult = send(ConnectSocket, char_array, len, 0);

		if (iResult == SOCKET_ERROR) {
			printf("CLIENT: send failed: %d\n", WSAGetLastError());
			Stop();
			return false;
		}

		return true;
	};




	// Send message to server
	bool Client::Send(const string& str) {
		return this->Send((char*)str.c_str(), str.length());
	};

	// Receive message from server
	string Client::Recv() {

		//const int buffer_size = 200000;


		char recvbuf[DEFAULT_BUFFER_LENGTH];
		//char recvbuf[buffer_size];

		//int result_len = recv(ConnectSocket, recvbuf, buffer_size, 0);
		int result_len = recv(ConnectSocket, recvbuf, DEFAULT_BUFFER_LENGTH, 0);

		if (result_len > 0) {
			//char msg[DEFAULT_BUFFER_LENGTH];
			//memset(&msg, 0, sizeof(msg));
			//strncpy_s(msg, recvbuf, result_len);

			//printf("Client received: %s\n", msg);
			string str(recvbuf, result_len);


			return str;
		}


		return "";
	}




	



