#pragma once



#ifndef CLIENT_H
#define CLIENT_H

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

//#include <thread>         // std::thread



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

using namespace rmp;

using namespace Shaders;

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <Eigen/Dense>

#include <math.h>       /* ceil */


#include "base.h"

//using namespace Eigen;
//using namespace std;

using Eigen::Vector3f;



#include <WinSock2.h>
namespace rmp {


class Client {
public:

	char* szServerName;
	SOCKET ConnectSocket;


	string ip = "127.0.0.1";
	string port = "";

	bool started = false;

	Client(string _ip, string _port);
	Client(char* servername);
	Client(SOCKET _connect_socket);

	SOCKET get_socket();
	void wait_for_messages(SOCKET connect_socket);

	bool Start();

	// Free the resouces
	void Stop();

	// Send message to server
	bool Send(char* char_array, const int len);
	bool Send(const string& str);

	// Receive message from server
	string Recv();

};

}


#endif
