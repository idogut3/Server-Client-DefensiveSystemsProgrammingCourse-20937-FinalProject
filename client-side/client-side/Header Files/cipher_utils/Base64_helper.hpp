#ifndef BASE64_HELPER_HPP
#define BASE64_HELPER_HPP
#include <base64.h>


class Base64Helper
{
public:
	static std::string encode(const std::string& str);
	static std::string decode(const std::string& str);
};

#endif