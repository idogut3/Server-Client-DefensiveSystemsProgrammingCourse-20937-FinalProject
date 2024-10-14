#include "Header Files\request_header_files\request.hpp"
#include "Header Files\utils.hpp"

RequestHeader::RequestHeader(UUID user_id, uint16_t request_code, uint32_t request_payload_size)
	: uuid(user_id), version(VERSION), code(request_code), payload_size(request_payload_size) {}

UUID RequestHeader::getUUID() const {
	return this->uuid;
}
uint8_t RequestHeader::getVersion() const {
	return this->version;
}
uint16_t RequestHeader::getCode() const {
	return this->code;
}
uint32_t RequestHeader::getPayloadSize() const {
	return this->payload_size;
}

Request::Request(RequestHeader request_header)
	: header(request_header) {}

