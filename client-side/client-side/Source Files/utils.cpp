#include "client-side\Header Files\utils.hpp"

UUID getUUIDFromString(string client_id)
{
	UUIDGenerator uuid_generator;  // Use the alias for boost::uuids::string_generator
	return uuid_generator(client_id);  // Generate and return a UUID from the string
}

bool is_integer(const std::string& num) {
	if (num.empty()) {
		return false; // Empty string is not a valid integer
	}

	std::size_t start = 0;

	// Check for a leading sign ('+' or '-')
	if (num[0] == '+' || num[0] == '-') {
		if (num.length() == 1) {
			return false;  // A single '+' or '-' is not a valid integer
		}
		start = 1; // Start checking digits after the sign
	}

	// Check if all remaining characters are digits
	for (std::size_t i = start; i < num.length(); ++i) {
		if (!std::isdigit(num[i])) {
			return false;  // Non-digit character found none 0-9 char
		}
	}

	return true;  // All characters are digits (or valid sign)
}


uint16_t getCodeFromResponseHeader(const std::vector<uint8_t>& header) {
	// The code starts right after the version field, which is 1 byte
	const size_t code_offset = 1;  // Version is 1 byte, so code starts at offset 1
	uint16_t code = 0;

	// Copy the 2 bytes of the code from the header and convert to little-endian
	std::memcpy(&code, &header[code_offset], sizeof(code));
	return native_to_little(code);
}

uint32_t getPayloadSizeFromResponseHeader(const std::vector<uint8_t>& header) {
	// The payload size starts after the version (1 byte) and the code (2 bytes)
	const size_t payload_size_offset = 1 + 2; // Version is 1 byte, code is 2 bytes
	uint32_t  payload_size = 0;

	// Copy the 4 bytes of the payload size from the header and convert to little-endian
	std::memcpy(&payload_size, &header[payload_size_offset], sizeof(payload_size));
	return native_to_little(payload_size);
}