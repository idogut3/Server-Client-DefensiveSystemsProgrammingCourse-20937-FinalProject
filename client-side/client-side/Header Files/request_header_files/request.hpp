#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <cstdint> // Include for uint8_t, uint16_t, uint32_t

using UUID = boost::uuids::uuid;
class RequestHeader {
protected:
	UUID uuid;
	uint8_t version;
	uint16_t code;
	uint32_t payload_size;
	
public:
	RequestHeader(UUID uuid, uint16_t code, uint32_t payload_size);
	UUID getUUID() const;
	uint8_t getVersion() const;
	uint16_t getCode() const;
	uint32_t getPayloadSize() const

};

class Payload{};

class Request {
protected:
	RequestHeader header;

public:
	Request(RequestHeader header);

    RequestHeader getHeader() const { return header;}
	virtual Payload getPayload() const = 0;

};

#endif 