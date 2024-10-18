#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "utils.hpp"

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
	uint32_t getPayloadSize() const;

	void setUUIDFromRawBytes(const std::vector<uint8_t>& uuid_bytes);

	vector<uint8_t> pack_header() const;
};

class Payload {};

class Request {
protected:
	RequestHeader header;

public:
	Request(RequestHeader header);

	RequestHeader getHeader() const;
	virtual Payload getPayload() const = 0;

	// Pure Virtual function, each request derived class will implement this function.
	vitrual vector<uint8_t> pack_request();
	virtual bool run(tcp::socket& sock) = 0;
};

#endif 