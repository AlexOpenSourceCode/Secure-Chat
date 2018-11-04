#pragma once
#ifndef AESOBJ_H
#define AESOBJ_H


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



using namespace std;

















class AESObj {
public:

	AutoSeededRandomPool rng;
	std::string key = "0123456789abcdef";
	std::string iv = "aaaaaaaaaaaaaaaa";

	// Generate a random IV
	byte key_b[AES::DEFAULT_KEYLENGTH];

	// Generate a random IV
	byte iv_b[AES::BLOCKSIZE];


	CryptoPP::AES::Encryption* aesEncryption;
	CryptoPP::CBC_Mode_ExternalCipher::Encryption* cbcEncryption;

	bool new_key_pair = false;


	AESObj(CryptoPP::AES::Encryption* _aesEncryption, CryptoPP::CBC_Mode_ExternalCipher::Encryption* _cbcEncryption, string _key, string _iv);

	AESObj(string _key, string _iv);

	AESObj();

	~AESObj();

	string encrypt(string& unencrypted_string);
	string encrypt(string& unencrypted_string, string& _iv);

	//HMAC with SHA-256 as hash function was chosen to
	//be solutions for authenticating messages.
	string decrypt(string& encrypted_string);
	string decrypt(string encrypted_string, string& _iv);




	string get_key_string();

	string get_iv_string();

};
#endif
