#include "client-side\Header Files\utils.hpp"

Bytes operator+(const Bytes& first, const Bytes& second) {
	Bytes result;
	result.reserve(first.size() + second.size());
	result.insert(result.end(), first.begin(), first.end());  // Insert first at the end
	result.insert(result.end(), second.begin(), second.end());  // Append second at the end
	return result;
}

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

uint32_t extractPayloadContentSize(Bytes response_payload) {
	// Extract the 4 bytes starting from index 16 and convert to uint32_t
	uint32_t content_size = (static_cast<uint32_t>(response_payload[16])) |
		(static_cast<uint32_t>(response_payload[17]) << 8) |
		(static_cast<uint32_t>(response_payload[18]) << 16) |
		(static_cast<uint32_t>(response_payload[19]) << 24);
	return content_size;
}

string extractSendFileResponseFileName(Bytes response_payload) {
	// The file name starts at index 20 and can be up to MAX_FILE_NAME_LENGTH bytes long
	size_t file_name_start = 20;
	size_t file_name_length = std::min(static_cast<size_t>(MAX_FILE_NAME_LENGTH), response_payload.size() - file_name_start);

	// Create a string from the response payload data starting at index 20
	std::string file_name(reinterpret_cast<const char*>(response_payload.data() + file_name_start), file_name_length);

	// Remove any null terminators from the end of the string
	file_name.erase(std::find(file_name.begin(), file_name.end(), '\0'), file_name.end());

	return file_name;
}
unsigned long extractSendFileResponseCksum(Bytes response_payload) {
	size_t cksum_start = 275;

	// The checksum is located at the end of the response payload (last 4 bytes)
	unsigned long response_cksum =
		static_cast<unsigned long>(response_payload[275]) << 24 | // 1st byte of checksum
		static_cast<unsigned long>(response_payload[276]) << 16 | // 2nd byte of checksum
		static_cast<unsigned long>(response_payload[277]) << 8 | // 3rd byte of checksum
		static_cast<unsigned long>(response_payload[278]);        // 4th byte of checksum

	return response_cksum;
}




bool are_uuids_equal(const Bytes first, const UUID second) {
	for (int i = 0; i < first.size(); i++) {
		if (first[i] != second.data[i]){
			return false;
		}
	}
	return true;
}


uint32_t htole32(uint32_t x) {
	return ((x >> 24) & 0x000000FF) |
		((x >> 8) & 0x0000FF00) |
		((x << 8) & 0x00FF0000) |
		((x << 24) & 0xFF000000);
}

uint16_t htole16(uint16_t x) {
	return ((x >> 8) & 0x00FF) |
		((x << 8) & 0xFF00);
}