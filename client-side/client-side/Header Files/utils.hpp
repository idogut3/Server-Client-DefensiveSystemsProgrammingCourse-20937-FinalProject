#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>    

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/endian/conversion.hpp>

#include "cipher_utils/AESWrapper.hpp"
#include "cipher_utils/Base64Wrapper.hpp"
#include "cipher_utils/RSAWrapper.hpp"

#include "codes.hpp"
#include "payloads_sizes.hpp"


namespace uuids = boost::uuids;

using std::cout;
using std::cin;

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;

using boost::asio::ip::tcp;
using UUID = boost::uuids::uuid;
const UUID NIL_UUID = boost::uuids::nil_uuid();
using UUIDGenerator = boost::uuids::string_generator;  
using boost::uuids::to_string;
using boost::endian::native_to_little;

constexpr auto VERSION = 3;
constexpr auto NAME_LENGTH = 255;
constexpr auto PUBLIC_KEY_LENGTH = 160;
constexpr auto REQUEST_HEADER_SIZE = 23;
constexpr auto RESPONSE_HEADER_SIZE = 7;
constexpr auto MAX_NAME_LENGTH = 100;
constexpr auto HEX_ID_LENGTH = 32;
constexpr auto CONTENT_SIZE_PER_PACKET = 1024;
constexpr auto MAX_FAILS = 3;

const std::string EXE_DIR = "client.cpp\\..\\..\\x64\\debug"; //Todo: change later cuz folders
#define EXE_DIR_FILE_PATH(file_name) (EXE_DIR + "\\" + file_name)
#define FATAL_MESSAGE_RETURN(type) \
	cerr << "Fatal: " << type << " request failed.\n"; \
	return;

#define TOTAL_PACKETS(content_size) \
	((content_size % CONTENT_SIZE_PER_PACKET) ? (content_size/CONTENT_SIZE_PER_PACKET + 1) : content_size/CONTENT_SIZE_PER_PACKET)




UUID getUUIDFromString(string client_id);
bool is_integer(const std::string& num);



#endif