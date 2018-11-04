#pragma once



#ifndef RSAOBJ_H
#define RSAOBJ_H

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



using namespace std;





class RSAObj {
public:
	RSA::PrivateKey* private_key;
	RSA::PublicKey* public_key;
	AutoSeededRandomPool rng;
	InvertibleRSAFunction parameters;

	bool new_key_pair = false;


	RSAObj(RSA::PublicKey* _public_key, RSA::PrivateKey* _private_key);

	RSAObj(const string& public_key_file_name, const string& private_key_file_name);

	RSAObj();


	~RSAObj();

	void save_private_key();

	void save_public_key();



	void set_public_key(RSA::PublicKey* _public_key);

	void set_private_key(RSA::PrivateKey* _private_key);


	string encrypt(string unencrypted_string);

	string encrypt_with(RSA::PublicKey* other_public_key, const string& unencrypted_string);

	string decrypt(string encrypted_string);





	void SaveHexPrivateKey(const string& filename, const RSA::PrivateKey& key);

	void SaveHexPublicKey(const string& filename, const RSA::PublicKey& key);


	void Save(const string& filename, const BufferedTransformation& bt);


	void SaveHex(const string& filename, const BufferedTransformation& bt);



	string public_key_to_string(RSA::PublicKey publicKey);

	string private_key_to_string(RSA::PrivateKey publicKey);

	RSA::PublicKey load_public_key(const string& file_name);

	RSA::PrivateKey load_private_key(const string& file_name);



	//RSA::PrivateKey rsaPrivate(params);
	//
	//std::string rsaPrivateMaterial;
	//StringSink stringSink(rsaPrivateMaterial);
	//rsaPrivate.DEREncode(stringSink);
	//
	//RSA::PrivateKey rsaPrivate2;
	//StringSource stringSource(rsaPrivateMaterial, true);
	//rsaPrivate2.BERDecode(stringSource);



	RSA::PrivateKey* string_to_private_key(const string& private_key_string);

	RSA::PublicKey* string_to_public_key(const string& public_key_string);




};



#endif