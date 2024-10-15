//#ifndef RSA_HELPER_HPP
//#define RSA_HELPER_HPP
//
//#include <osrng.h>
//#include <rsa.h>
//#include <string>
//
//
//
//class RSAPublicKeyHelper
//{
//public:
//	static const unsigned int KEYSIZE = 160;
//	static const unsigned int BITS = 1024;
//
//private:
//	CryptoPP::AutoSeededRandomPool _rng;
//	CryptoPP::RSA::PublicKey _publicKey;
//
//	RSAPublicKeyHelper(const RSAPublicKeyHelper& rsapublic);
//	RSAPublicKeyHelper& operator=(const RSAPublicKeyHelper& rsapublic);
//public:
//
//	RSAPublicKeyHelper(const char* key, unsigned int length);
//	RSAPublicKeyHelper(const std::string& key);
//	~RSAPublicKeyHelper();
//
//	std::string getPublicKey() const;
//	char* getPublicKey(char* keyout, unsigned int length) const;
//
//	std::string encrypt(const std::string& plain);
//	std::string encrypt(const char* plain, unsigned int length);
//};
//
//
//class RSAPrivateKeyHelper
//{
//public:
//	static const unsigned int BITS = 1024;
//
//private:
//	CryptoPP::AutoSeededRandomPool _rng;
//	CryptoPP::RSA::PrivateKey _privateKey;
//
//	RSAPrivateKeyHelper(const RSAPrivateKeyHelper& rsaprivate);
//	RSAPrivateKeyHelper& operator=(const RSAPrivateKeyHelper& rsaprivate);
//public:
//	RSAPrivateKeyHelper();
//	RSAPrivateKeyHelper(const char* key, unsigned int length);
//	RSAPrivateKeyHelper(const std::string& key);
//	~RSAPrivateKeyHelper();
//
//	std::string getPrivateKey() const;
//	char* getPrivateKey(char* keyout, unsigned int length) const;
//
//	std::string getPublicKey() const;
//	char* getPublicKey(char* keyout, unsigned int length) const;
//
//	std::string decrypt(const std::string& cipher);
//	std::string decrypt(const char* cipher, unsigned int length);
//};
//
//#endif