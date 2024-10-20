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
	std::memcpy(packed_payload.data(), this->username, std::min(username_length, size_t(REGISTRATION_PAYLOAD_SIZE)));

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

void SendPublicKeyPayload::setEncryptedAESKey(const char* encrypted_aes_key, size_t key_length) {
	if (key_length <= ENCRYPTED_AES_KEY_LENGTH) {
		std::memcpy(this->encrypted_aes_key, encrypted_aes_key, key_length);
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
	std::memcpy(packed_payload.data(), this->username, std::min(name_length, NUM_OF_BYTES_OF_USERNAME));

	// Copy the public key into the vector (160 bytes)
	std::memcpy(packed_payload.data() + NUM_OF_BYTES_OF_USERNAME, this->public_key, NUM_OF_BYTES_OF_PUBLIC_KEY);

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

const char* ReconnectionPayload::getUsername() const {
	return username;
}


Bytes ReconnectionPayload::pack_payload() const {
	Bytes packed_payload(RECONNECTION_PAYLOAD_SIZE, 0); // Initialize with zeroes the packed_payload

	// Get the actual length of the username string (up to 255)
	size_t username_length = std::strlen(this->username);

	// Copy the username into the vector
	std::memcpy(packed_payload.data(), this->username, std::min(username_length, size_t(RECONNECTION_PAYLOAD_SIZE)));

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
	std::memcpy(packed_payload.data(), this->file_name, std::min(file_name_length, size_t(VALID_CRC_PAYLOAD_SIZE)));

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
	std::memcpy(packed_payload.data(), this->file_name, std::min(file_name_length, size_t(INVALID_CRC_PAYLOAD_SIZE)));

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

	// Copy the file_name into the vector
	std::memcpy(packed_payload.data(), this->file_name, std::min(file_name_length, size_t(INVALID_CRC_DONE_PAYLOAD_SIZE)));

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
	Bytes packed_payload();

	// TODO:: Add implementation

	return packed_payload;
}