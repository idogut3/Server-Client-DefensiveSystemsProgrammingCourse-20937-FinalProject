#include "Header Files\request_header_files\requests_payloads.hpp"
#include "Header Files\utils.hpp"

RegistrationPayload::RegistrationPayload(const char* username) {
	// Attempt to copy the username
	errno_t result = strcpy_s(this->username, sizeof(this->username), username);
	if (result != 0) {
		std::cerr << "Error copying username in RegistrationPayload: source is too long!" << std::endl;
		// Handle the error appropriately, e.g., set username to an empty string
		this->username[0] = '\0'; // Ensure username is empty on error
	}
}

Bytes RegistrationPayload::pack_payload() const
{
	Bytes packed_payload(REGISTRATION_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the username string (up to 255)
	size_t username_length = std::strlen(this->username);

	// Copy the username into the vector
	std::copy(this->username, this->username + std::min(username_length, size_t(REGISTRATION_PAYLOAD_SIZE)), packed_payload.begin());
	return packed_payload;
}
	





SendPublicKeyPayload::SendPublicKeyPayload(const char* username, const char* public_key) {
	// Attempt to copy the username
	errno_t result = strcpy_s(this->username, sizeof(this->username), username);
	if (result != 0) {
		std::cerr << "Error copying username in SendPublicKeyPayload: source is too long!" << std::endl;
		this->username[0] = '\0'; // Ensure username is empty on error
	}

	// Attempt to copy the public key
	result = strcpy_s(this->public_key, sizeof(this->public_key), public_key);
	if (result != 0) {
		std::cerr << "Error copying public key in SendPublicKeyPayload: source is too long!" << std::endl;
		this->public_key[0] = '\0'; // Ensure public_key is empty on error
	}
}

const char* SendPublicKeyPayload::getUsername() const {
	return username;
}

const char* SendPublicKeyPayload::getPublicKey() const {
	return public_key;
}

void SendPublicKeyPayload::setEncryptedAESKey(const char* encrypted_aes_key, const size_t key_length) {
	if (key_length <= ENCRYPTED_AES_KEY_LENGTH) {
		std::copy(encrypted_aes_key, encrypted_aes_key + key_length, this->encrypted_aes_key);
	}
	else {
		throw std::length_error("Key length exceeds ENCRYPTED_AES_KEY_LENGTH");
	}
}


Bytes SendPublicKeyPayload::pack_payload() const {
	
	constexpr size_t NUM_OF_BYTES_OF_USERNAME = 255;
	constexpr size_t NUM_OF_BYTES_OF_PUBLIC_KEY = 160;

	// Initialize the packed payload with zeroes and in the correct size
	Bytes packed_payload(NUM_OF_BYTES_OF_USERNAME + NUM_OF_BYTES_OF_PUBLIC_KEY, 0);

	// Get the actual length of the username string
	size_t name_length = std::strlen(this->username);

	// Copy the username into the vector, limiting to the maximum size
	std::copy(this->username, this->username + std::min(name_length, size_t(NUM_OF_BYTES_OF_USERNAME)), packed_payload.begin());

	// Copy the public key into the vector (160 bytes)
	std::copy(this->public_key, this->public_key + NUM_OF_BYTES_OF_PUBLIC_KEY, packed_payload.begin() + NUM_OF_BYTES_OF_USERNAME);

	return packed_payload;
}




ReconnectionPayload::ReconnectionPayload(const char* username) {
	// Attempt to copy the username
	errno_t result = strcpy_s(this->username, sizeof(this->username), username);
	if (result != 0) {
		std::cerr << "Error copying username in ReconnectionPayload: source is too long!" << std::endl;
		this->username[0] = '\0'; // Ensure username is empty on error
	}
}
const string ReconnectionPayload::getEncryptedAESKey() const {
	return this->encrypted_aes_key;
}


const char* ReconnectionPayload::getUsername() const {
	return username;
}

void ReconnectionPayload::setEncryptedAESKey(const char* encrypted_aes_key, const size_t key_length) {
	if (key_length <= ENCRYPTED_AES_KEY_LENGTH) {
		std::copy(encrypted_aes_key, encrypted_aes_key + key_length, this->encrypted_aes_key);
	}
	else {
		throw std::length_error("Key length exceeds ENCRYPTED_AES_KEY_LENGTH");
	}
}


Bytes ReconnectionPayload::pack_payload() const {
	Bytes packed_payload(RECONNECTION_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the username string (up to 255)
	size_t username_length = std::strlen(this->username);

	// Copy the username into the vector
	std::copy(this->username, this->username + std::min(username_length, size_t(RECONNECTION_PAYLOAD_SIZE)), packed_payload.begin());

	return packed_payload;
}



ValidCrcPayload::ValidCrcPayload(const char* file_name) {
	// Attempt to copy the file name
	errno_t result = strcpy_s(this->file_name, sizeof(this->file_name), file_name);
	if (result != 0) {
		std::cerr << "Error copying file name in ValidCrcPayload: source is too long!" << std::endl;
		this->file_name[0] = '\0'; // Ensure file_name is empty on error
	}
}

const char* ValidCrcPayload::getFileName() const {
	return file_name;
}

Bytes ValidCrcPayload::pack_payload() const {
	Bytes packed_payload(VALID_CRC_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the file name string (up to VALID_CRC_PAYLOAD_SIZE)
	size_t file_name_length = std::strlen(this->file_name);

	// Copy the file_name into the vector
	std::copy(this->file_name, this->file_name + std::min(file_name_length, size_t(VALID_CRC_PAYLOAD_SIZE)), packed_payload.begin());
	return packed_payload;
}



InvalidCrcPayload::InvalidCrcPayload(const char* file_name) {
	// Attempt to copy the file name
	errno_t result = strcpy_s(this->file_name, sizeof(this->file_name), file_name);
	if (result != 0) {
		std::cerr << "Error copying file name in InvalidCrcPayload: source is too long!" << std::endl;
		this->file_name[0] = '\0'; // Ensure file_name is empty on error
	}
}

const char* InvalidCrcPayload::getFileName() const {
	return file_name;
}

Bytes InvalidCrcPayload::pack_payload() const {
	Bytes packed_payload(INVALID_CRC_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the file name string (up to INVALID_CRC_PAYLOAD_SIZE)
	size_t file_name_length = std::strlen(this->file_name);

	// Copy the file_name into the vector
	std::copy(this->file_name, this->file_name + std::min(file_name_length, size_t(INVALID_CRC_PAYLOAD_SIZE)), packed_payload.begin());

	return packed_payload;
}



InvalidCrcDonePayload::InvalidCrcDonePayload(const char* file_name) {
	strcpy_s(this->file_name, file_name); // Use strcpy_s for safer copying
}

const char* InvalidCrcDonePayload::getFileName() const {
	return file_name;
}

Bytes InvalidCrcDonePayload::pack_payload() const {
	Bytes packed_payload(INVALID_CRC_DONE_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the file name string (up to INVALID_CRC_DONE_PAYLOAD_SIZE)
	size_t file_name_length = std::strlen(this->file_name);

	// Copy the file_name into the vector using std::copy
	std::copy(this->file_name, this->file_name + std::min(file_name_length, size_t(INVALID_CRC_DONE_PAYLOAD_SIZE)), packed_payload.begin());

	return packed_payload;
}




SendFilePayload::SendFilePayload(uint32_t content_size, uint32_t orig_file_size, uint16_t packet_number, uint16_t total_packets, char file_name[])
	: content_size(content_size), orig_file_size(orig_file_size), packet_number(packet_number), total_packets(total_packets) {
	// Attempt to copy the file name
	errno_t result = strcpy_s(this->file_name, sizeof(this->file_name), file_name);
	if (result != 0) {
		std::cerr << "Error copying file name in InvalidCrcDonePayload: source is too long!" << std::endl;
		this->file_name[0] = '\0'; // Ensure file_name is empty on error
	}
}


uint32_t SendFilePayload::get_content_size() const {
	return content_size;
}

uint32_t SendFilePayload::get_orig_file_size() const {
	return orig_file_size;
}

uint16_t SendFilePayload::get_packet_number() const {
	return packet_number;
}

uint16_t SendFilePayload::get_total_packets() const {
	return total_packets;
}

const char* SendFilePayload::get_file_name() const {
	return file_name;
}

const string& SendFilePayload::get_encrypted_file_content() const {
	return encrypted_file_content;
}

Bytes SendFilePayload::pack_payload() const {
	Bytes packed_payload(SENDING_FILE_PAYLOAD_SIZE_WITHOUT_FILE_SIZE + sizeof(this->encrypted_file_content), 0);

	// Iterator for filling the vector
	auto it = packed_payload.begin();

	// Convert and copy content_size (4 bytes) in little-endian
	uint32_t little_endian_content_size = htole32(this->content_size);

	it = std::copy(reinterpret_cast<const uint8_t*>(&little_endian_content_size),
		reinterpret_cast<const uint8_t*>(&little_endian_content_size) + sizeof(little_endian_content_size),it);

	// Convert and copy orig_file_size (4 bytes) in little-endian
	uint32_t little_endian_orig_file_size = htole32(this->orig_file_size);
	it = std::copy(reinterpret_cast<const uint8_t*>(&little_endian_orig_file_size),
		reinterpret_cast<const uint8_t*>(&little_endian_orig_file_size) + sizeof(little_endian_orig_file_size),it);

	// Convert and copy packet_number (2 bytes) in little-endian
	uint16_t little_endian_packet_number = htole16(this->packet_number);
	it = std::copy(reinterpret_cast<const uint8_t*>(&little_endian_packet_number),
		reinterpret_cast<const uint8_t*>(&little_endian_packet_number) + sizeof(little_endian_packet_number),it);

	// Convert and copy total_packets (2 bytes) in little-endian
	uint16_t little_endian_total_packets = htole16(this->total_packets);
	it = std::copy(reinterpret_cast<const uint8_t*>(&little_endian_total_packets),
		reinterpret_cast<const uint8_t*>(&little_endian_total_packets) + sizeof(little_endian_total_packets),it);

	// Copy the file_name (MAX_USERNAME_LENGTH bytes)
	it = std::copy(reinterpret_cast<const uint8_t*>(this->file_name),
		reinterpret_cast<const uint8_t*>(this->file_name) + MAX_USERNAME_LENGTH,it);

	// Copy the encrypted file content (its dynamic size)
	it = std::copy(encrypted_file_content.begin(), encrypted_file_content.end(), it);

	return packed_payload;
}