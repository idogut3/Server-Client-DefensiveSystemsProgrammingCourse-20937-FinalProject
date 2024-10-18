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

Bytes RegistrationPayload::pack_payload() const {
	Bytes packed_payload();

	// TODO:: Add implementation

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

Bytes SendPublicKeyPayload::pack_payload() const {
	Bytes packed_payload();

	// TODO:: Add implementation

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
	Bytes packed_payload();

	// TODO:: Add implementation

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
	Bytes packed_payload();

	// TODO:: Add implementation

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
	Bytes packed_payload();

	// TODO:: Add implementation

	return packed_payload;
}



InvalidCrcDonePayload::InvalidCrcDonePayload(const char* file_name) {
	strcpy_s(this->file_name, file_name); // Use strcpy_s for safer copying
}

const char* InvalidCrcDonePayload::getFileName() const {
	return file_name;
}

Bytes InvalidCrcDonePayload::pack_payload() const {
	Bytes packed_payload();

	// TODO:: Add implementation

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

Bytes SendFilePayload::pack_payload() const {
	Bytes packed_payload();

	// TODO:: Add implementation

	return packed_payload;
}