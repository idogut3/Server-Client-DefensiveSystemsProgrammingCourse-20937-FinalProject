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


uint16_t extractCodeFromResponseHeader(const Bytes& header) {
	uint8_t high = header[1], low = header[2];
	uint16_t combined_bytes = (static_cast<uint16_t>(high) << 8) | low;

	uint16_t native_code = little_to_native(combined_bytes);
	return native_code;
}

uint32_t extractPayloadSizeFromResponseHeader(const Bytes& header) {
	uint8_t first = header[3], second = header[4];
	uint8_t third = header[5],  forth = header[6];

	uint32_t combined = (static_cast<uint32_t>(first) << 24) | (static_cast<uint32_t>(second) << 16) | (static_cast<uint32_t>(third) << 8) | forth;

	uint32_t native_payload_size = little_to_native(combined);
	return native_payload_size;
}