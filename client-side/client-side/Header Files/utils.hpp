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


using UUID = boost::uuids::uuid;
const UUID NIL_UUID = boost::uuids::nil_uuid();



constexpr auto VERSION = 3;

#endif