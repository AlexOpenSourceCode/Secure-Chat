#include "Chat.h"
#include "base.h"


Chat::Chat(Server* _server, Client* _client){
	server = _server;
	client = _client;

	boost::thread start_socket_server_thread(&Chat::start_chat_thread, this);
}

Chat::Chat(Server* _server){
	server = _server;

	boost::thread start_socket_server_thread(&Chat::start_chat_thread, this);
}

Chat::Chat(Client* _client){
	client = _client;

	boost::thread start_socket_server_thread(&Chat::start_chat_thread, this);
}


Chat::Chat(string SERVER_OR_CLIENT, string ip, string port){

	chat_name = Base::to_lower_case(SERVER_OR_CLIENT);

	if (Base::to_lower_case(SERVER_OR_CLIENT) == "server"){
		server = new Server(ip, port);
		//server->start_server();
	}
	else if (Base::to_lower_case(SERVER_OR_CLIENT) == "client"){
		client = new Client(ip, port);
		//client->Start();
	}
	

	boost::thread start_socket_server_thread(&Chat::start_chat_thread, this);
}

Chat::Chat(){

}

Chat::~Chat(){
	if (read_thread_){
		//calls detach in deconstructor supposedly
		delete read_thread_;
	}
	if (send_thread_){
		delete send_thread_;
	}
	if (server){
		delete server;
	}
	if (client){
		delete client;
	}

}


void Chat::add_message(const string& str){
	std::lock_guard<std::mutex> guard(myMutex);
	chat_messages.push_back(str);
}

void Chat::add_message(const string& str, const string& receive_or_send_string){
	this->add_message(this->get_from_string(receive_or_send_string) + ": " + str);
}



vector<string> Chat::get_chat_messages(){
	std::lock_guard<std::mutex> guard(myMutex);
	return chat_messages;
}



void Chat::client_read_thread(){
	if (!client){
		return;
	}

	while (client) {
		string msg = client->Recv();

		if (msg != "" && msg.find("error") == std::string::npos){
			std::cout << "Inside Chat::client_read_thread" << endl;
			std::cout << "recieved message from client " << endl;
			chat_messages.push_back("Client: " + msg);
		}


	}
}

void Chat::send_sym_key(){

}

void Chat::send_sym_iv(){

}

bool Chat::is_at_end_of_string(const string& needle, const string& hay){
	if (needle.size() > hay.size()){
		return false;
	}
	else if (needle.size() == hay.size()){
		if (needle == hay){
			return true;
		}
		else{
			return false;
		}
	}
	else {

		string end_subs = hay.substr(hay.size() - needle.size(), hay.size());

		if (end_subs == needle){
			return true;
		}
		else{
			return false;
		}
	}
}


vector<string> get_file_parts(const string& file_path){
	vector<string> path_parts;
	path_parts = Base::split(file_path, '/');
	if (path_parts.size() > 0){
		string file_name = path_parts[path_parts.size() - 1];

		vector<string> file_name_parts = Base::split(file_name, '.');
		return file_name_parts;
	}


	//invalid
	return path_parts;
}


string Chat::create_file_msg_header(string file_path){

	vector<string> file_parts = get_file_parts(file_path);
	string file_name = "";
	string ext = "";
	if (file_parts.size() == 2){
		file_name = file_parts[0];
		ext = file_parts[1];
	}

	return HEADER_STRING + HEADER_SEPERATOR + FILE_COMMAND + HEADER_SEPERATOR + file_name + HEADER_SEPERATOR + ext + HEADER_END;
}


string Chat::create_msg_header(const string& command, int data_size){
	return HEADER_STRING + HEADER_SEPERATOR + command + HEADER_SEPERATOR + to_string(data_size) + HEADER_END;
}





string Chat::create_encrypted_msg_header(const string& command, const string& encrypted_text, const string& hmac_sig){
	return HEADER_STRING + HEADER_SEPERATOR + command + HEADER_SEPERATOR + to_string(encrypted_text.size() + hmac_sig.size()) + HEADER_SEPERATOR + to_string(hmac_sig.size()) + HEADER_END;
}


template<typename T>
void remove_substring(basic_string<T>& s, const basic_string<T>& p) {
	basic_string<T>::size_type n = p.length();

	for (basic_string<T>::size_type i = s.find(p);
		i != basic_string<T>::npos;
		i = s.find(p))
		s.erase(i, n);
}




void Chat::send_aes_encrypted_message(string unencrypted_msg){

	if (unencrypted_msg.at(0) == '$'){
		return; // dont send chat commands as encrypted message
	}


	cout << "Sending " << unencrypted_msg << endl;
	if (aes_obj == NULL){
		cout << "aes_obj is NULL" << endl;
		return;
	}

	string encrypted_msg = base64_encode(aes_obj->encrypt(unencrypted_msg));


	//create_encrypted_msg_header(AES_ENCRYPTED_COMMAND, unencrypted_msg);
	//send(create_encrypted_msg_header(AES_ENCRYPTED_COMMAND, encrypted_msg));
	string hmac_sig = hmac_sign(aes_obj->key, unencrypted_msg);

	if (DEBUG){
		//cout << "SENDING HMAC SIG: " << hmac_sig << endl;
		//cout << "HMAC SIG LENGTH: " << hmac_sig.size() << endl;
	}


	send(create_encrypted_msg_header(AES_ENCRYPTED_COMMAND, encrypted_msg, hmac_sig));


	this->add_message(" ");
	this->add_message("Plaintext: " + unencrypted_msg);
	this->add_message("AES Ciphertext: " + encrypted_msg);
	//add hmac sig to front of message
	encrypted_msg.insert(0, hmac_sig);

	send(encrypted_msg);

	
	this->add_message("HMAC: " + hmac_sig);
	this->add_message(unencrypted_msg, "send");
}




void Chat::print(const string& msg){
	if (DEBUG){
		//cout << msg << endl;
	}	
}


void Chat::chat_read_thread(){
	int iResult = 1;
	char recvbuf[DEFAULT_BUFFER_LENGTH];


	//char recvbuf[65936];

	//Holds pieces of message
	//until all are received
	string msg_stack = "";


	//holds extra data for received for next msg
	string extra_msg_stack = "";


	//Get in the msg header over tcp
	//to know how many bytes to accept
	int expecting_data_size = 0;
	int bytes_received = 0;

	bool process_message_stack = false;
	bool last_file_part = false;


	vector<string> header_parts;

	while (true){
		

		try{

		string msg_str = "";

		if (server){
			msg_str = server->srecv();
			//cout << "CHAT READ THREAD SERVER !!!" << endl;
		}
		else if (client){
			msg_str = client->Recv();
		}


		if (msg_str.size() == 0){
			continue;
		}

		
		if (client){
			////cout << "CLIENT CHAT RECEIVED " << endl;
			////cout << msg_str << endl;
		}


		if (server){
			////cout << "chat server receive thread: len(msg) = " << msg_str.size() << endl;
			////cout << msg_str << endl;


			print("Server Read: " + msg_str);
		}
		else if (client){

			print("Client Read: " + msg_str);
			
		}



		if (msg_str.substr(0, HEADER_STRING.size()) == HEADER_STRING){
			bool error_parsing_header = false;
			size_t header_end_index = msg_str.find(HEADER_END);
			if (header_end_index != std::string::npos){
				string header_string = msg_str.substr(0, header_end_index);
				if (DEBUG){
					//cout << "HEADER STRING: " << header_string << endl;
				}
				

				vector<string> _header_parts = Base::split(header_string, HEADER_SEPERATOR.at(0));



				if (_header_parts.size() >= 2){
					header_parts = _header_parts;

					string command_ = header_parts[1];
					string msg_length_ = "";

					if (_header_parts.size() >= 3){

						if (command_ == FILE_COMMAND){
							last_set_ms = Base::current_time_ms();
						}
						else{
							msg_length_ = header_parts[2];

							//Make sure the msg_length can be parsed
							try{
								expecting_data_size = stoi(msg_length_);
							}
							catch (...){
								//cout << "Error parsing msg_length from header " << endl;
								error_parsing_header = true;

							}
						}
					}

					//Make sure command is valid
					if (commands.find(command_) != commands.end()){
						if (true || DEBUG){
							cout << "RECEIVING COMMAND: " + command_ << endl;
						}

						cout << msg_str << endl;

						RECEIVE_STATE_COMMAND = commands[command_];
					    
						remove_substring(msg_str, header_string + HEADER_END);	
					}
					else{
						//cout << "command: " << command_ << "not in command list " << endl;
						error_parsing_header = true;
					}
				}
				else{
					//cout << "HEADER ISNT 2 parts" << endl;
					error_parsing_header = true;
				}
			}
			else{
				//cout << "Couldnt find the end of msg header" << endl;
				error_parsing_header = true;
			}

			if (error_parsing_header){
				//cout << "ERROR PARSING MSG HEADER. CLEARING MSG_STR and MSG_STACK" << endl;
				msg_str = "";
				msg_stack.clear();
			}

		}
		else{
			if (RECEIVE_STATE_COMMAND == commands[FILE_COMMAND]){
				//Since files are sent part by part, we dont know its overall size
				//therefore we must send a end receive file terminator ex "</FILE>", or the program would
				//continue receiving file forever
				//Check if end of file command is in this string
				size_t found = msg_str.find(END_FILE_COMMAND);
				if (found != std::string::npos){
					//remove END_FILE_COMMAND from this msg
					msg_str = msg_str.substr(0, found);
					last_file_part = true;
				}
					
			}
		}



		//Files are written chunk by chunk so dont add to msg_stack
		if (RECEIVE_STATE_COMMAND == commands[FILE_COMMAND]){
			process_message_stack = false;
		}
		else if (RECEIVE_STATE_COMMAND == commands[TEXT_COMMAND]){
			process_message_stack = false;
		}
		else{
			//Header is cleaned from msg_str at this point if there was one
			if (msg_stack.size() >= expecting_data_size){
				process_message_stack = true;
			}
			else if (msg_stack.size() < expecting_data_size){
				//15 + 6 = 21 > 20
				if (msg_stack.size() + msg_str.size() > expecting_data_size){
					int split_index = expecting_data_size - msg_stack.size();
					string msg_sub_str = msg_str.substr(0, split_index);
					


					print("CURRENT MSG STACK");

					cout << msg_stack << endl;
					if (DEBUG){
						//cout << "msg stack size: " << msg_stack.size() << endl;
						//cout << "expecting_data_size: " << expecting_data_size << endl;
					}

					msg_stack += msg_sub_str;

					if (DEBUG){
						//cout << "ADDING substr to MSG STACK TO GET EXPECTED DATA SIZE: " << msg_str.substr(0, split_index) << endl;
						//cout << "MSG STACK: " << msg_stack.size() << " == EXPECTED BYTES: " << expecting_data_size << endl;
					}

					process_message_stack = true;

					extra_msg_stack += msg_str.substr(split_index, msg_str.size());
					

					if (DEBUG){
						//cout << "ADDING data to extra_msg_stack: " << msg_str.substr(split_index, msg_str.size()) << endl;

					}
				}
				else {
					msg_stack += msg_str;

					if (msg_stack.size() == expecting_data_size){
						process_message_stack = true;
					}
				}

			}
		}
		



		if (process_message_stack){
			print(" FULLY RECEIVED MSG: " + RECEIVE_STATE_COMMAND); 
			print(msg_stack);



			if (RECEIVE_STATE_COMMAND == RECEIVE_PUBLIC_KEY){
				//The public key is done being sent

				//other_public_key = string_to_public_key(base64_decode(msg_stack));
				try{
					other_public_key = rsa_obj->string_to_public_key(base64_decode(msg_stack));

					if (server){
						//cout << "RECEIVED CLIENT'S PUBLIC KEY COMPLETELY. CREATE AES SESSION KEY TO SEND TO CLIENT" << endl;

						//Send symmetric key and IV encrypted with the clients public key
						string encrypted_aes_key = base64_encode(rsa_obj->encrypt_with(other_public_key, aes_obj->get_key_string()));
						string encrypted_iv_key = base64_encode(rsa_obj->encrypt_with(other_public_key, aes_obj->get_iv_string()));


						send(create_msg_header(AES_KEY_COMMAND, encrypted_aes_key.size()));
						//cout << endl;

						if (DEBUG){
							cout << "SENDING FULL AES KEY" << endl;
							cout << encrypted_aes_key << endl;
						}

						send(encrypted_aes_key);

						//cout << endl;

						if (DEBUG){
							cout << "SENDING FULL AES IV" << endl;
							cout << encrypted_iv_key << endl;
						}


						send(create_msg_header(AES_IV_COMMAND, encrypted_iv_key.size()));
						send(encrypted_iv_key);
						//cout << endl;

					}
				}
				catch (...){
					//cout << "ERROR CREATING PUBLIC KEY FROM STRING" << endl;
					//cout << msg_stack;
				}

			}
			else if (RECEIVE_STATE_COMMAND == RECEIVE_AES_KEY){
				print("RECEIVED AES KEY");

				key = rsa_obj->decrypt(base64_decode(msg_stack));
			}
			else if (RECEIVE_STATE_COMMAND == RECEIVE_AES_IV){
				if (DEBUG){
					cout << "RECEIVED AES IV" << endl;
				}
				
				iv = rsa_obj->decrypt(base64_decode(msg_stack));
			}
			else if (RECEIVE_STATE_COMMAND == RECEIVE_AES_ENCRYPTED){


				if (DEBUG){
					//cout << " RECEIVED AES ENCRYPTED " << chat_name << endl;
					//cout << msg_stack << endl;
				}
				

				
				
				if (aes_obj == NULL){
					//cout << "AES OBJ ON " + chat_name + " IS NULL " << endl;
					continue;
				}
				
				
				string received_hmac_sig = msg_stack.substr(0, stoi(header_parts[3]));
				


				string aes_encrypted_msg_part = msg_stack.substr(stoi(header_parts[3]), msg_stack.size());
				string decrypted_msg = aes_obj->decrypt(base64_decode(aes_encrypted_msg_part));

				string hmac_sig = hmac_sign(aes_obj->key, decrypted_msg);

				//Check if this hmac_sig matches the one sent in the header
				if (received_hmac_sig == hmac_sig){
					if (DEBUG){
						//cout << "HMAC SIGNATURE VERIFIED" << endl;
					}
					
				}


				this->add_message("Encrypted AES string: " + aes_encrypted_msg_part, "receive");
				this->add_message("HMAC Signature: " + hmac_sig);

				if (DEBUG){
					//cout << "DECRYPTED MESSAGE: " << decrypted_msg << endl;
				}
				
				this->add_message(decrypted_msg, "receive");
			}


			if (key != "" && iv != "" && aes_obj == NULL){
				cout << "CREATED NEW AES OBJ WITH KEY AND IV";
				aes_obj = new AESObj(key, iv);

				
			}

			//clear msg stack
			msg_stack.clear();
			msg_stack = "";

			if (extra_msg_stack.size() > 0){
				msg_stack = extra_msg_stack;


				extra_msg_stack.clear();
				extra_msg_stack = "";
			}

			//Reset the receive state back to text
			RECEIVE_STATE_COMMAND = RECEIVE_TEXT;

			//Done with this message so clear header
			header_parts.clear();

			process_message_stack = false;
		}//end process message
		else{

			if (RECEIVE_STATE_COMMAND == RECEIVE_TEXT){
				// the access to this function is mutually exclusive
				//std::lock_guard<std::mutex> guard(myMutex);

				this->add_message(msg_str, "receive");

			}
			else if (RECEIVE_STATE_COMMAND == RECEIVE_FILE){
				
				
				if (msg_str.length() > 0){
					if (output_file == NULL){
						output_file = fopen("C:\\code\\c++\\output\\jew.mp4", "wb");
					}

					string file_name = header_parts[2];
					string file_ext = header_parts[3];

					this->receive_file_2(file_name, file_ext, (char *)msg_str.c_str(), msg_str.length(), output_file);

				}

				if (last_file_part){


					this->add_message("Finished receiving file");
					this->add_message("File took " + to_string(Base::ms_span(last_set_ms)) + "ms to receive");
					//file done sending, set receive state back to text
					last_file_part = false;
					process_message_stack = false;
					RECEIVE_STATE_COMMAND = commands[TEXT_COMMAND];

					fclose(output_file);
				}
			}

		}



		}
		catch (const std::runtime_error& re)
		{
			// speciffic handling for runtime_error
			std::cerr << "Runtime error: " << re.what() << std::endl;
		}
		catch (const std::exception& ex)
		{
			// speciffic handling for all exceptions extending std::exception, except
			// std::runtime_error which is handled explicitly
			std::cerr << "Error occurred: " << ex.what() << std::endl;
		}
		catch (...)
		{
			// catch any other errors (that we have no information about)
			std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;
		}

	}

}

void Chat::send(const string& txt){
	if (server){
		if (DEBUG){
			print("Server Sending: " + txt);
		}
		

		server->Send(txt);
	}
	else if (client){
		if (DEBUG){
			print("Client Sending: " + txt);
		}
		
		client->Send(txt);
	}
}

void Chat::send(char* char_array, const int char_array_len){
	if (server){
		print("Server Sending: ");
		print(char_array);

		server->Send(char_array, char_array_len);
	}
	
	
	if (client){
		print("Client Sending: ");
		print(char_array);

		client->Send(char_array, char_array_len);
	}
}


void Chat::send_message(const string& msg){
	this->add_message(msg, "send");

	this->send(msg);
}



void Chat::chat_send_thread(){
	int send_result_int = 1;
	int iSendResult;


	if (chat_name == "client"){
		print("CLIENT SENDING ITS PUBLIC KEY TO SERVER: ");

		string p_key_str = rsa_obj->public_key_to_string(*rsa_obj->public_key);

		std::string b64_encoded_str = base64_encode(p_key_str);

		message_to_send = create_msg_header(PUBLIC_KEY_COMMAND, b64_encoded_str.size());
		message_to_send += b64_encoded_str;

		send_message_flag = true;
	}



	while (true){
		continue;

		if (send_message_flag){


			SOCKET this_socket;

			if (server){
				this_socket = server->client_socket;
			}
			else if (client){
				this_socket = client->ConnectSocket;
			}

			std::lock_guard<std::mutex> guard(mutex_2);

			if (message_to_send.at(0) == '$'){
				send_file("C:/code/c++/input/test.mp4");
			}
			else{
				if (server){
					print("Server sends: ");
					print(message_to_send.c_str());
					server->Send(message_to_send);
				}
				else if (client){
					print("Client sends: ");
					print(message_to_send.c_str());


					client->Send(message_to_send);
				}
			}

			

			//iSendResult = send(this_socket, (char*)message_to_send.c_str(), message_to_send.length(), 0);

			//if (iSendResult == SOCKET_ERROR) {
			//	//cout << "send failed: %d\n" << WSAGetLastError() << endl;
			//	closesocket(this_socket);
			//	WSACleanup();
			//	//return 1;
			//}




			//if (server && client){
			//	std::lock_guard<std::mutex> guard(mutex_2);
			//	//cout << "MESSAGE_TO_SEND: " << message_to_send << endl;
			//	//cout << "SENDING CHAT INPUT TEXT TO CLIENT" << endl;


			//	client->Send(message_to_send);

			//}



			send_message_flag = false;
			message_to_send = "";
		}


		

		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	}//end while(true)




	server->clean_socket(server->listen_socket);

}

void Chat::add_client(Client* c){
	std::cout << "add_new_client()" << endl;
	//active_client = new Client("127.0.0.1");

	//if (!active_client->Start()){
	//	std::cout << "ERROR STARTING CLIENT";
	//}


	//boost::thread client_wait_for_messages_thread(wait_for_messages, active_client);
	client = c;
	//read_thread_ = new boost::thread(&Chat::client_read_thread, this);
}

int Chat::start_server(){

	
	chat_messages.push_back("Starting server send thread");
	read_thread_ = new boost::thread(&Chat::chat_read_thread, this);
	send_thread_ = new boost::thread(&Chat::chat_send_thread, this);
	active = true;

	return 1;
}

int Chat::start_client(){
	//if (!client->started){
	//	this->add_message("Client error connecting to server < " + client->ip + ":" + client->port + " >");
	//	return 1;
	//}

	client->Start();

	this->add_message("Successfully connected to < " + client->ip + ":" + client->port + " >");

	active = true;
	//start read thread that waits for incoming messages
	read_thread_ = new boost::thread(&Chat::chat_read_thread, this);
	send_thread_ = new boost::thread(&Chat::chat_send_thread, this);



	string p_key_str = rsa_obj->public_key_to_string(*rsa_obj->public_key);

	std::string b64_encoded_str = base64_encode(p_key_str);

	message_to_send = create_msg_header(PUBLIC_KEY_COMMAND, b64_encoded_str.size());
	//message_to_send += b64_encoded_str;

	this->send(create_msg_header(PUBLIC_KEY_COMMAND, b64_encoded_str.size()));
	this->send(b64_encoded_str);


	////cout << "SENDING FULL MSG" << endl;
	////cout << b64_encoded_str << endl;
	return 1;
}


int Chat::start_chat_thread(){

	if (!server && !client){
		//cout << "Neither server nor client is initialized" << endl;
		return 1;
	}

	if (server){
		aes_obj = new AESObj();
		if (!server->start_server()){
			this->add_message("Error starting server");
			return 1;
		}

		this->add_message("Started server < " + server->ip + ":" + server->port + " >");
	}

	rsa_obj = new RSAObj();

	if (!client) {
		this->add_message("Waiting for client...");
		server->wait_for_client(server->listen_socket);
		
		this->add_message("Client successfully connected");
		active = true;
	}

	if (server){
		this->start_server();
	}

	if (client){
		this->start_client();
	}
}



string Chat::hmac_sign(string key, string plain) {
	string mac, encoded;
	try
	{
		HMAC< SHA256 > hmac((byte*)key.c_str(), key.length());

		StringSource(plain, true,
			new HashFilter(hmac,
			new StringSink(mac)
			) // HashFilter      
			); // StringSource
	}
	catch (const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
	}

	encoded.clear();

	encoded = base64_encode(mac);

	return encoded;
}



string Chat::get_chat_type(){
	if (server){
		return "server";
	}

	if (client){
		return "client";
	}

	return "blank chat";
}




string Chat::get_from_string(string send_or_receive){
	string from_string = "";
	if (server){

		if (send_or_receive == "send"){
			
			from_string = "server";
		}
		else{
			from_string = "client";
		}
		
	}
	else if (client){

		if (send_or_receive == "send"){
			from_string = "client";
		}
		else{
			from_string = "server";
		}
	}

	return from_string;

}




int last_new_line_index(char* char_array, const int char_array_len){

	for (int x = char_array_len - 1; x >= 0; x--){
		if (char_array[x] == '\n'){
			return x;
		}
	}

	return -1;
}


void print_char(char* char_array, const int char_array_len){

	for (int x = 0; x < char_array_len; x++){
		//cout << "char[" << x << "] = <" << char_array[x] << "> : space ?= " << (char_array[x] == ' ') << ", newline ?= " << (char_array[x] == '\n') << ", eof ?= " << (char_array[x] == std::ifstream::traits_type::eof()) << endl;
	}

}



void Chat::send_file(string file_path) {

	if (!Base::file_exists(file_path)){

		this->add_message("File " + file_path + " doesnt exist");
		return;
	}




	//std::streampos filesize = 0;
	std::ifstream in(file_path, std::ios::binary);
	//default buffer = 512
	//cout << "DEFAULT_BUFFER_LENGTH: " << DEFAULT_BUFFER_LENGTH << endl;

	// 5100ms at 5000 size
	const int sendbuflen = DEFAULT_BUFFER_LENGTH;
	//const int sendbuflen = 500000;
	char sendbuf[sendbuflen];


	//Mbps = Megabit/sec
	//1 Mbps = 0.125 MB/s (megabytes)
	//1 Mbps / 8 = 0.125 MB/s 

	std::ZeroMemory(&sendbuf, sendbuflen);

	last_set_ms = Base::current_time_ms();

	//send(FileSendSocket, (char*)file_command.c_str(), file_command.size(), 0);
	send(create_file_msg_header(file_path));

	//Bandwidth-Delay-Product - Bandwidth*Delay Products (BDP)
	//Optimal Buffer size formula :
	//• buffer size = 20 % * (bandwidth * RTT)
	//	•(assuming your target is 20 % of the narrow link)
	//Example :
	//		• ping time(RTT) = 50 ms
	//		• Narrow link = 1000 Mbps(125 MBytes / sec)
	//		• TCP buffers should be :
	//-.05 sec * 125 * 20 % = 1.25 MBytes


	if (in.is_open()) {

		this->add_message("Sending file " + file_path);
		while (1) {
			in.read(sendbuf, sendbuflen);

			this->send(sendbuf, in.gcount());

			ZeroMemory(&sendbuf, sendbuflen);

			if (in.eof()) {
				//std::cout << "End of File sending from Client" << std::endl;
				in.close();
				break;
			}

			//else
			//{
			//	send(FileSendSocket, sendbuf, sendbuflen, 0);
			//	ZeroMemory(&sendbuf, sendbuflen);
			//}
		}

		send(END_FILE_COMMAND);

		this->add_message("Finished sending file " + file_path);


		double ms_span = Base::ms_span(last_set_ms);

		this->add_message("Send file took " + to_string(ms_span) + "ms ; " + to_string(ms_span / 1000) + " seconds");
	}
	else{
		this->add_message("Couldnt open file " + file_path);
	}

}



void Chat::receive_file_2(string file_name, string file_ext, char* recvbuf, int recvbuflen, FILE* pFile) {
	fwrite(recvbuf, sizeof(char), recvbuflen, pFile);
}


void Chat::receive_file(string file_name, string file_ext, char* recvbuf, int recvbuflen) {

	//cout << "recvbuf: " << recvbuf << endl;
	// | ios::app    appends to file

	//ofstream out(file_download_path + file_name + "." + file_ext, ios::binary | ios::app);

	ofstream out("C:\\code\\c++\\output\\test.mp4", ios::binary | ios::app);
	if (out.is_open()) {
		out.write(recvbuf, recvbuflen);
		ZeroMemory(&recvbuf, recvbuflen);
	}
}




void Chat::set_file_download_path(const string& new_path) {
	file_download_path = new_path;
}



const string& Chat::get_file_download_path() const {
	return file_download_path;
}



int Chat::message_count() {
	return chat_messages.size();
}


string Chat::get_message(int msg_index) {
	if (msg_index >= chat_messages.size() || msg_index < 0){
		//cout << "invalid msg_index" << endl;
		return "";
	}
	else{
		return chat_messages[msg_index];
	}
}


string Chat::get_ip(){
	if (server){
		return server->ip;
	}
	else if (client){
		return client->ip;
	}

	return "0.0.0.0";
}


string Chat::get_port(){
	if (server){
		return server->port;
	}
	else if (client){
		return client->port;
	}
	return "0";
}


string Chat::get_ip_and_port_string(){
	return this->get_ip() + ":" + this->get_port();
}


const bool Chat::is_active() const {
	return active;
}