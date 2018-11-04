#include "AESObj.h"


AESObj::AESObj(CryptoPP::AES::Encryption* _aesEncryption, CryptoPP::CBC_Mode_ExternalCipher::Encryption* _cbcEncryption, string _key, string _iv){
	key = _key;
	iv = _iv;
	aesEncryption = _aesEncryption;
	cbcEncryption = _cbcEncryption;
}

AESObj::AESObj(string _key, string _iv){
	key = _key;
	iv = _iv;
	//aesEncryption = new CryptoPP::AES::Encryption((byte *)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
	//cbcEncryption = new CryptoPP::CBC_Mode_ExternalCipher::Encryption(*aesEncryption, (byte *)iv.c_str());
	new_key_pair = true;
}

AESObj::AESObj(){



	rng.GenerateBlock(key_b, AES::DEFAULT_KEYLENGTH);
	rng.GenerateBlock(iv_b, AES::BLOCKSIZE);

	key = (char *)key_b;
	iv = (char *)iv_b;

	//aesEncryption = new CryptoPP::AES::Encryption((byte *)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
	//cbcEncryption = new CryptoPP::CBC_Mode_ExternalCipher::Encryption(*aesEncryption, (byte *)iv.c_str());
	new_key_pair = true;
}

AESObj::~AESObj(){
	if (new_key_pair){
		delete aesEncryption;
		delete cbcEncryption;
		aesEncryption = nullptr;
		cbcEncryption = nullptr;

	}
}

string AESObj::encrypt(string& unencrypted_string){
	return encrypt(unencrypted_string, iv);
}

string AESObj::encrypt(string& unencrypted_string, string& _iv){

	string encrypted_string = "";

	try {
		//e.SetKeyWithIV((const byte *)key.c_str(), key.size(), (const byte *)iv.c_str());
		//rng.GenerateBlock(iv_b, AES::BLOCKSIZE);
		//key = (char *)key_b;
		//_iv = (char *)iv_b;

		CBC_Mode< AES >::Encryption e;
		e.SetKeyWithIV((const byte *)key.c_str(), key.size(), (const byte *)_iv.c_str());

		//  The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(unencrypted_string, true,
			new StreamTransformationFilter(e,
			new StringSink(encrypted_string)
			) // StreamTransformationFilter
			); // StringSource


#if 0
		StreamTransformationFilter filter(e);
		filter.Put((const byte*)plain.data(), plain.size());
		filter.MessageEnd();

		const size_t ret = filter.MaxRetrievable();
		cipher.resize(ret);
		filter.Get((byte*)cipher.data(), cipher.size());
#endif
	}
	catch (const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/



	//Convert bytes to hex string


	// Pretty print
	string hex_encoded_encrypted_string;

	//hex_encoded_encrypted_string.clear();
	StringSource(encrypted_string, true,
		new HexEncoder(
		new StringSink(hex_encoded_encrypted_string)
		) // HexEncoder
		); // StringSource


	return hex_encoded_encrypted_string;
}

string AESObj::decrypt(string& encrypted_string){
	return decrypt(encrypted_string, iv);
}

string AESObj::decrypt(string encrypted_string, string& _iv){

	string decryptedtext = "";



	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV((const byte *)key.c_str(), key.size(), (const byte *)_iv.c_str());
	// The StreamTransformationFilter removes
	//  padding as required.


	//If message is hex encoded, convert to bytes

	string encoded = encrypted_string;
	string hex_decoded;


	StringSource(encrypted_string, true,
		new HexDecoder(
		new StringSink(hex_decoded)
		) // HexEncoder
		); // StringSource


	StringSource s(hex_decoded, true,
		new StreamTransformationFilter(d,
		new StringSink(decryptedtext)
		) // StreamTransformationFilter
		); // StringSource


	return decryptedtext;
}

string AESObj::get_key_string(){
	string s(key);
	return s;
}

string AESObj::get_iv_string(){
	string s(iv);
	return s;
}












//string plain = "CBC Mode Test";
//string cipher, encoded, recovered;


//std::string plaintext = "name macmilan age 24 ciy bonn country germany";
//std::string ciphertext;
//std::string decryptedtext;

//std::cout << "Plain Text (" << plaintext.size() << " bytes)" << std::endl;
//std::cout << plaintext;
//std::cout << std::endl << std::endl;

//

//CryptoPP::StreamTransformationFilter stfEncryptor(*cbcEncryption, new CryptoPP::StringSink(ciphertext));
//stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length() + 1);
//stfEncryptor.MessageEnd();
////cout << "cipher text plain: " << ciphertext << endl;
//std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
////cout << endl;
////cout << endl;




//std::cout << "cipher text In HEX FORM:: ";
//for (int i = 0; i < ciphertext.size(); i++) {

//	std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
//}
////cout << endl;
////cout << endl;
///*********************************\
					//\*********************************/

//// Pretty print
//encoded.clear();
//StringSource(ciphertext, true,
//	new HexEncoder(
//	new StringSink(encoded)
//	) // HexEncoder
//	); // StringSource
////cout << "cipher text In HEX FORM (Modified):: " << encoded << endl;
////cout << endl;
////cout << endl;
//char *name2;
//name2 = (char*)malloc(encoded.length() + 1); // don't forget to free!!!!
////s2 = Database_row_count; // I forget if the string class can implicitly be converted to char*
////s2[0] = '1';
//std::strcpy(name2, encoded.c_str());

//const char* hex_str = name2;

//std::string result_string;
//unsigned int ch;
//for (; std::sscanf(hex_str, "%2x", &ch) == 1; hex_str += 2)
//	result_string += ch;
////cout << "HEX FORM to cipher text :: ";
//std::cout << result_string << '\n';
////cout << endl;
////cout << endl;








//This works as well
//CryptoPP::AES::Decryption aesDecryption((byte *)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
//CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, (byte *)iv.c_str());

//CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
//stfDecryptor.Put(reinterpret_cast<const unsigned char*>(encrypted_string.c_str()), encrypted_string.size());
//stfDecryptor.MessageEnd();

//std::cout << "Decrypted Text: " << std::endl;
//std::cout << decryptedtext;
//std::cout << std::endl << std::endl;

