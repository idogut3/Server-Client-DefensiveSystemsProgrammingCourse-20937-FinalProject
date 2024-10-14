#include "Header Files\request_header_files\request.h"

RequestHeader::RequestHeader(UUID uuid, uint16_t code, uint32_t payload_size) {
	uuid(uuid);
	version(VERSION);
	code(code);
	payload_size(payload_size);
}


Request::Request() {
	uuid(uuid),
		version(VERSION),
		code(code),
		payload_size(payload_size)
}

UUID Request::getUuid() const {
	return this->uuid;
}
uint8_t Request::getVersion() const {
	return this->version;
}
uint16_t Request::getCode() const {
	return this->code;
}
uint32_t Request::getPayloadSize() const {
	return this->payload_size;
}