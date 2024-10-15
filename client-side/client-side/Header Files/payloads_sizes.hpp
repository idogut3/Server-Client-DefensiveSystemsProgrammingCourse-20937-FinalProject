#ifndef PAYLODS_SIZES_HPP
#define PAYLODS_SIZES_HPP
#include "Header Files\utils.hpp"

// 32 bits = 4 bytes
enum PayloadSize : uint32_t {
	REGISTRATION_PAYLOAD_SIZE = 255,
	SENDING_PUBLIC_KEY_PAYLOAD_SIZE = 415,
	RECONNECTION_PAYLOAD_SIZE = 255,
	SENDING_FILE_PAYLOAD_SIZE_WITHOUT_FILE_SIZE = 267,
	VALID_CRC_PAYLOAD_SIZE = 255,
	SENDING_CRC_AGAIN_PAYLOAD_SIZE = 255,
	INVALID_CRC_DONE_PAYLOAD_SIZE = 255,

	REGISTRATION_SUCCEEDED_PAYLOAD_SIZE = 16,
	REGISTRATION_FAILED_PAYLOAD_SIZE = 0,
	PUBLIC_KEY_RECEIVED_PAYLOAD_SIZE_WITHOUT_AES_KEY_SIZE = 16,
	FILE_RECEIVED_CRC_PAYLOAD_SIZE = 279,
	MESSAGE_RECEIVED_PAYLOAD_SIZE = 16,
	RECONNECTION_SUCCEEDED_PAYLOAD_SIZE_WITHOUT_AES_KEY_SIZE = 176,
	RECONNECTION_FAILED_PAYLOAD_SIZE = 16,
	GENERAL_ERROR_PAYLOAD_SIZE = 0
};

#endif