#include "RSAObj.h"



RSAObj::RSAObj(RSA::PublicKey* _public_key, RSA::PrivateKey* _private_key) :public_key(_public_key), private_key(_private_key){

}

RSAObj::RSAObj(const string& public_key_file_name, const string& private_key_file_name) {
	public_key = &load_public_key(public_key_file_name);
	private_key = &load_private_key(private_key_file_name);
}

RSAObj::RSAObj() {
	parameters.GenerateRandomWithKeySize(rng, 3072);
	private_key = new RSA::PrivateKey(parameters);
	public_key = new RSA::PublicKey(parameters);
	new_key_pair = true;
}

RSAObj::~RSAObj(){
	if (new_key_pair){
		delete private_key;
		private_key = nullptr;
		delete public_key;
		public_key = nullptr;
	}
}

void RSAObj::save_private_key() {
	FileSink private_file_output("rsaprivate.dat");
	private_key->DEREncode(private_file_output);
}

void RSAObj::save_public_key() {
	FileSink public_file_output("rsapublic.dat");
	public_key->DEREncode(public_file_output);
}

void RSAObj::set_public_key(RSA::PublicKey* _public_key){
	public_key = _public_key;
}

void RSAObj::set_private_key(RSA::PrivateKey* _private_key){
	private_key = _private_key;
}

string RSAObj::encrypt(string unencrypted_string){
	string cipher;
	RSAES_OAEP_SHA_Encryptor e(*public_key);

	StringSource ss1(unencrypted_string, true,
		new PK_EncryptorFilter(rng, e, new StringSink(cipher)) // PK_EncryptorFilter
		); // StringSource

	return cipher;
}

string RSAObj::encrypt_with(RSA::PublicKey* other_public_key, const string& unencrypted_string){
	string cipher;
	RSAES_OAEP_SHA_Encryptor e(*other_public_key);

	StringSource ss1(unencrypted_string, true,
		new PK_EncryptorFilter(rng, e, new StringSink(cipher)) // PK_EncryptorFilter
		); // StringSource

	return cipher;
}

string RSAObj::decrypt(string encrypted_string){
	// Decrypt
	RSAES_OAEP_SHA_Decryptor d(*private_key);
	string recovered;

	StringSource ss2(encrypted_string, true,
		new PK_DecryptorFilter(rng, d, new StringSink(recovered)) // PK_DecryptorFilter
		); // StringSource

	return recovered;
}

void RSAObj::SaveHexPrivateKey(const string& filename, const RSA::PrivateKey& key) {
	ByteQueue queue;
	key.Save(queue);

	SaveHex(filename, queue);
}

void RSAObj::SaveHexPublicKey(const string& filename, const RSA::PublicKey& key) {
	ByteQueue queue;
	key.Save(queue);

	SaveHex(filename, queue);
}

void RSAObj::Save(const string& filename, const BufferedTransformation& bt) {
	FileSink file(filename.c_str());

	bt.CopyTo(file);
	file.MessageEnd();
}

void RSAObj::SaveHex(const string& filename, const BufferedTransformation& bt) {
	HexEncoder encoder;

	bt.CopyTo(encoder);
	encoder.MessageEnd();

	Save(filename, encoder);
}

string RSAObj::public_key_to_string(RSA::PublicKey publicKey){
	ByteQueue q2;
	publicKey.DEREncode(q2);

	string key_2;
	StringSink ss2(key_2);
	q2.TransferTo(ss2);

	return key_2;
}

string RSAObj::private_key_to_string(RSA::PrivateKey publicKey){
	ByteQueue q2;
	publicKey.DEREncode(q2);

	string key_2;
	StringSink ss2(key_2);
	q2.TransferTo(ss2);

	return key_2;
}

RSA::PublicKey RSAObj::load_public_key(const string& file_name){
	RSA::PublicKey rsaPublic;
	FileSource input2(file_name.c_str(), true);
	rsaPublic.BERDecode(input2);
	return rsaPublic;
}

RSA::PrivateKey RSAObj::load_private_key(const string& file_name){
	// Read keys from file into new objects
	RSA::PrivateKey rsaPrivate;


	FileSource input(file_name.c_str(), true);
	rsaPrivate.BERDecode(input);
	return rsaPrivate;

}

RSA::PrivateKey* RSAObj::string_to_private_key(const string& private_key_string){


	RSA::PrivateKey* rsa_private = new RSA::PrivateKey();
	StringSource stringSource(private_key_string, true);
	rsa_private->BERDecode(stringSource);

	return rsa_private;
}

RSA::PublicKey* RSAObj::string_to_public_key(const string& public_key_string){


	RSA::PublicKey* rsa_public = new RSA::PublicKey();
	StringSource stringSource(public_key_string, true);
	rsa_public->BERDecode(stringSource);

	return rsa_public;
}

