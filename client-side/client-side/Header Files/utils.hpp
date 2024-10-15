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

using std::cout;
using std::cin;
using std::string;
using UUID = boost::uuids::uuid;
const UUID NIL_UUID = boost::uuids::nil_uuid();
using UUIDGenerator = boost::uuids::string_generator;  // Alias for the string generator

constexpr auto VERSION = 3;
constexpr auto NAME_LENGTH = 255;
constexpr auto PUBLIC_KEY_LENGTH = 160;
constexpr auto REQUEST_HEADER_SIZE = 23;
constexpr auto RESPONSE_HEADER_SIZE = 7;
constexpr auto MAX_NAME_LENGTH = 100;
constexpr auto HEX_ID_LENGTH = 32;
const std::string EXE_DIR = "client.cpp\\..\\..\\x64\\debug";
#define EXE_DIR_FILE_PATH(file_name) (EXE_DIR + "\\" + file_name)



UUID getUUIDFromString(string client_id);


#endif