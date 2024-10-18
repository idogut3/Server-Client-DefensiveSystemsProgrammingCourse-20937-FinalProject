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

void RequestHeader::setUUIDFromRawBytes(const Bytes& uuid_bytes) {
	// Assuming response_payload is already validated to be exactly 16 bytes
	std::copy(uuid_bytes.begin(), uuid_bytes.end(), uuid.data.begin());
}

RequestHeader Request::getHeader() {
	return this->header;
}

Bytes RequestHeader::pack_header() const {
	Bytes request(REQUEST_HEADER_SIZE + this->payload_size);

	// Saving the numeric type in little endian order
	uint16_t code_in_little_endian = native_to_little(this->code);
	uint32_t payload_size_in_little_endian = native_to_little(this->payload_size);

	// Saving the bytes in little endian order as a byte array.
	uint8_t* code_in_little_endian_ptr = reinterpret_cast<uint8_t*>(&code_in_little_endian);
	uint8_t* payload_size_in_little_endian_ptr = reinterpret_cast<uint8_t*>(&payload_size_in_little_endian);

	// Adding fields to the vector
	size_t postion = 0;

	std::copy(uuid.begin(), uuid.end(), request.begin()); // Copying the uuid to the beginning of request
	postion += sizeof(uuid); // Move the position forward by the size of UUID

	request[postion] = version; // after the uuid we insert the version
	postion += sizeof(version); // Move the position forward by the size of version

	std::copy(code_in_little_endian_ptr, code_in_little_endian_ptr + sizeof(code_in_little_endian), request.begin() + postion);
	postion += sizeof(code); // Move the position forward by the size of code

	std::copy(payload_size_in_little_endian_ptr, payload_size_in_little_endian_ptr + sizeof(payload_size_in_little_endian), request.begin() + postion);

	return request;
}


Request::Request(RequestHeader request_header)
	: header(request_header) {}

