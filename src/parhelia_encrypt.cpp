#include "parhelia_encrypt.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

string ParheliaEncrypt::encrypt(const string & passphrase, const string & unencrypted)
{
	string ret;
	string key = passphrase;
	key.resize(32, 'X');
	string iv = "0123456789123456";
CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte *)key.c_str(), key.length(), (byte *)iv.c_str());
	CryptoPP::StringSource encryptor(unencrypted, true, new CryptoPP::StreamTransformationFilter(encryption, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(ret), false)));
	return ret;
}

string ParheliaEncrypt::decrypt(const string & passphrase, const string & encrypted)
{
	 string ret;
	 string key = passphrase;
	 key.resize(32, 'X');
	 string iv = "0123456789123456";
	 CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
	 CryptoPP::StringSource decryptor(encrypted, true, new CryptoPP::Base64Decoder(new CryptoPP::StreamTransformationFilter(decryption, new CryptoPP::StringSink(ret))));
	 return ret;
}


