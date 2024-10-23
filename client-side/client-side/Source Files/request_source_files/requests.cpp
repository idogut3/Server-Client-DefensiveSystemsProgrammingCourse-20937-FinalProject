#include "Header Files\utils.hpp"
#include "requests.hpp"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
	: Request(header), payload(payload) {}

const RegistrationPayload* RegisterRequest::getPayload() const {
	return &payload;  // Returning a pointer to payload
}


/*
	This method packs the header and payload for the registration request in a form of uint8_t vector
	All numeric fields are ordered by little endian order
*/
Bytes RegisterRequest::pack_request() const {
	Bytes packed_header = this->getHeader().pack_header();
	Bytes packed_payload = this->getPayload()->pack_payload();
	Bytes request = packed_header + packed_payload;
	return request;
}



int RegisterRequest::run(tcp::socket& sock) {
	// Pack request fields into vector and initialize parameter times_sent to 0
	int times_sent = 1;
	Bytes request = this->pack_request();

	while (times_sent <= MAX_REQUEST_FAILS) {
		try {
			// Send the request to the server via the provided socket
			boost::asio::write(sock, boost::asio::buffer(request));

			// Receiving the header from the server, extracting response code and header_extras_size
			Bytes response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = extractCodeFromResponseHeader(response_header);
			uint32_t response_payload_size = extractPayloadSizeFromResponseHeader(response_header);

			// Receiving the payload from the server and saving the num of bytes received from it
			Bytes response_payload(response_payload_size);
			size_t num_of_bytes_received_from_server = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// If the code is wrong or we didn't receive enough bytes or the payload size we got is wrong
			if (response_code != Codes::REGISTRATION_CODE || response_payload_size != PayloadSize::REGISTRATION_SUCCEEDED_PAYLOAD_SIZE || num_of_bytes_received_from_server != response_payload_size) {
				throw std::invalid_argument("server responded with an error");
			}
			// The Registration succeeded, set the uuid to the id the server responded with
			this->getHeaderReference().setUUIDFromRawBytes(response_payload);

			break; // Existing the loop Registration was successful
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}

		times_sent++; // Meaning we failed registering 1 time because we catched an exception 
	}

	// If the times_sent reached MAX_REQUEST_FAILS, returning false
	if (times_sent >= MAX_REQUEST_FAILS) {
		return FAILURE;
	}
	// If the the registration succeeded, return true
	return SUCCESS;
}


SendPublicKeyRequest::SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload)
	: Request(header), payload(payload) {}

const SendPublicKeyPayload* SendPublicKeyRequest::getPayload() const {
	return &payload;  // Returning a pointer to payload
}

void SendPublicKeyRequest::updateEncryptedAESKey(const Bytes& encrypted_aes_key) {
	this->payload.setEncryptedAESKey(reinterpret_cast<const char*>(encrypted_aes_key.data()), encrypted_aes_key.size());
}


Bytes SendPublicKeyRequest::pack_request() const {
	Bytes packed_header = this->getHeader().pack_header();
	Bytes packed_payload = this->getPayload()->pack_payload();
	Bytes request = packed_header + packed_payload;
	return request;
}


// TODO::::::::
int SendPublicKeyRequest::run(tcp::socket& sock) {
	// Pack request fields into vector and initialize parameter times_sent to 0
	int times_sent = 1;
	Bytes request = pack_request();

	while (times_sent <= MAX_REQUEST_FAILS) {
		try {
			// Send the request to the server via the provided socket
			boost::asio::write(sock, boost::asio::buffer(request));

			// Receive header from the server, get response code and header_extras_size
			Bytes response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = extractCodeFromResponseHeader(response_header);
			uint32_t response_payload_size = extractPayloadSizeFromResponseHeader(response_header);

			// Receive payload from the server, save it's length in parameter length
			Bytes response_payload(response_payload_size);
			size_t length = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// If the code is not success, the header_extras_size for the code is not the same as the size received in the header, or the length of the payload is not the wanted length, print error
			if (response_code != Codes::PUBLIC_KEY_RECEIVED_CODE || length != response_payload_size) {
				throw std::invalid_argument("server responded with an error");
			}

			// Copy the id from the payload, and check if it's the correct client id
			Bytes payload_uuid(sizeof(UUID_SIZE));
			std::copy(response_payload.begin(), response_payload.begin() + sizeof(UUID_SIZE), payload_uuid.begin());

			if (!are_uuids_equal(payload_uuid, this->getHeader().getUUID())) {
				throw std::invalid_argument("server responded with an error.");
			}

			updateEncryptedAESKey(response_payload);

			break; // Existing the loop SendPublicKeyRequest::run was successful
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		// Increment the i by 1 each iteration.
		times_sent++;
	}
	// If the times_sent reached MAX_REQUEST_FAILS, returning false
	if (times_sent >= MAX_REQUEST_FAILS) {
		return FAILURE;
	}
	// If the the SendPublicKeyRequest::run succeeded, return true
	return SUCCESS;
}



ReconnectRequest::ReconnectRequest(RequestHeader header, ReconnectionPayload payload)
	: Request(header), payload(payload) {}

const ReconnectionPayload* ReconnectRequest::getPayload() const {
	return &payload;  // Returning a pointer to payload
}

Bytes ReconnectRequest::pack_request() const { //todo :not done
	Bytes packed_header = this->getHeader().pack_header();
	Bytes packed_payload = this->getPayload()->pack_payload();
	Bytes request = packed_header + packed_payload;
	return request;
}


void ReconnectRequest::updateEncryptedAESKey(const Bytes& encrypted_aes_key) {
	this->payload.setEncryptedAESKey(reinterpret_cast<const char*>(encrypted_aes_key.data()), encrypted_aes_key.size());
}

int ReconnectRequest::run(tcp::socket& sock) {
	// Pack request fields into vector and initialize parameter times_sent to 1
	int times_sent = 1;
	Bytes request = pack_request();

	while (times_sent <= MAX_REQUEST_FAILS) {
		try {
			// Send the request to the server via the provided socket
			boost::asio::write(sock, boost::asio::buffer(request));

			// Receive header from the server, get response code and header_extras_size
			Bytes response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = extractCodeFromResponseHeader(response_header);
			uint32_t response_payload_size = extractPayloadSizeFromResponseHeader(response_header);

			// Receive payload from the server, save it's length in a parameter length
			Bytes response_payload(response_payload_size);
			size_t length = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// The reconnection FAILED, so it registerd a new user and returned us a new uuid
			if (response_code == Codes::RECONNECTION_FAILED_CODE && response_payload_size == PayloadSize::RECONNECTION_FAILED_PAYLOAD_SIZE && length == response_payload_size) {
				this->getHeaderReference().setUUIDFromRawBytes(response_payload);
				return SPECIAL;
			}

			// If the code is not success, or other problem occurred
			else if (response_code != Codes::RECONNECTION_SUCCEEDED_CODE || response_payload_size != PayloadSize::RECONNECTION_SUCCEEDED_PAYLOAD_SIZE_WITHOUT_AES_KEY_SIZE || length != response_payload_size) {
				throw std::invalid_argument("server responded with an error");
			}

			Bytes payload_uuid(sizeof(UUID_SIZE));
			std::copy(response_payload.begin(), response_payload.begin() + sizeof(UUID_SIZE), payload_uuid.begin());

			if (!are_uuids_equal(payload_uuid, this->getHeader().getUUID())) {
				throw std::invalid_argument("server responded with an error.");
			}

			// Copy the encrypted aes key content from the response_payload vector into the parameter encrypted_aes_key, then break from the loop.
			updateEncryptedAESKey(response_payload);
			break;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		// Increment the i by 1 each iteration.
		times_sent++;
	}
	// If the times_sent reached MAX_REQUEST_FAILS, returning FAILURE
	if (times_sent >= MAX_REQUEST_FAILS) {
		return FAILURE;
	}
	// If the the SendPublicKeyRequest::run succeeded, return SUCCESS
	return SUCCESS;
}



ValidCrcRequest::ValidCrcRequest(RequestHeader header, ValidCrcPayload payload)
	: Request(header), payload(payload) {}


const ValidCrcPayload* ValidCrcRequest::getPayload() const {
	return &payload;
}

//Bytes ValidCrcRequest::pack_request() { //TODO :NOT DONE
//	Bytes ret(1);
//	return ret(1)
//}


InvalidCrcRequest::InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload)
	: Request(header), payload(payload) {}

const InvalidCrcPayload* InvalidCrcRequest::getPayload() const {
	return &payload;
}

//Bytes InvalidCrcRequest::pack_request() { //TODO :NOT DONE
//	Bytes ret(1);
//	return ret(1)
//}

InvalidCrcDoneRequest::InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload)
	: Request(header), payload(payload) {}

const InvalidCrcDonePayload* InvalidCrcDoneRequest::getPayload() const {
	return &payload;
}

//Bytes InvalidCrcDoneRequest::pack_request() { //TODO :NOT DONE
//	Bytes ret(1);
//	return ret(1)
//}


SendFileRequest::SendFileRequest(RequestHeader header, SendFilePayload payload)
	: Request(header), payload(payload) {}

const SendFilePayload* SendFileRequest::getPayload() const {
	return &payload;
}

SendFilePayload& SendFileRequest::getPayloadReference() {
	return payload; // Return a reference to the payload
}


//This is a special request where I need to send the request in chunks of data because
// the file could be too big
Bytes SendFileRequest::pack_request() const {
	Bytes packed_header = this->getHeader().pack_header();
	Bytes packed_payload = this->getPayload()->pack_payload();

	// Create a request that combines the header and the first 267 bytes of the payload
	size_t header_extras_size = std::min(packed_payload.size(), size_t(SEND_FILE_REQUEST_HEADER_EXTRAS_SIZE)); // Ensure we don't exceed the payload size
	Bytes request;

	request.reserve(packed_header.size() + header_extras_size); // Reserve space for efficiency
	request.insert(request.end(), packed_header.begin(), packed_header.end()); // Add header to request
	request.insert(request.end(), packed_payload.begin(), packed_payload.begin() + header_extras_size); // Add first 267 bytes of payload

	return request;
}

void SendFileRequest::sendFileData(tcp::socket& sock) {
	Bytes request_payload = this->getPayload()->pack_payload();
	size_t total_bytes_to_send = request_payload.size() - SEND_FILE_REQUEST_HEADER_EXTRAS_SIZE;
	size_t bytes_sent = SEND_FILE_REQUEST_HEADER_EXTRAS_SIZE; // We continue from the point where its only the file bytes we need to send

	while (bytes_sent < total_bytes_to_send) {
		// Determine the remaining bytes to send and how much to send in this chunk
		size_t bytes_remaining = total_bytes_to_send - bytes_sent;
		size_t bytes_to_send = std::min(bytes_remaining, CONTENT_SIZE_PER_PACKET);

		// Create a buffer for the current chunk
		boost::asio::const_buffer buffer(request_payload.data() + bytes_sent, bytes_to_send);

		// Send the chunk
		boost::system::error_code error;
		size_t sent = boost::asio::write(sock, buffer, error);

		if (error) {
			std::cerr << "Error sending file data: " << error.message() << std::endl;
			break; // Break if there's an error
		}

		// Update the number of bytes sent
		bytes_sent += sent;
	}

	if (bytes_sent == total_bytes_to_send) {
		std::cout << "Successfully sent the entire request payload." << std::endl;
	}
	else {
		std::cerr << "Only sent " << (bytes_sent - SEND_FILE_REQUEST_HEADER_EXTRAS_SIZE) << " bytes out of " << total_bytes_to_send << " bytes." << std::endl;
	}
}


int SendFileRequest::run(tcp::socket& sock) 
{
	// Pack request fields into vector and initialize parameter times_sent to 1
	int times_sent = 1;
	Bytes request_header_with_extras = pack_request();

	while (times_sent <= MAX_REQUEST_FAILS) {
		try {
			// Send the request header to the server via the provided socket
			boost::asio::write(sock, boost::asio::buffer(request_header_with_extras));
			// Sending the payload
			sendFileData(sock);

			// Receive header from the server, get response code and payload_size
			Bytes response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = extractCodeFromResponseHeader(response_header);
			uint32_t response_payload_size = extractPayloadSizeFromResponseHeader(response_header);

			// Receive payload from the server, save it's length in a parameter length.
			Bytes response_payload(response_payload_size);
			size_t length = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// If the code is not success, the payload_size for the code is not the same as the size received in the header, or the length of the payload is not the wanted length, print error.
			if (response_code != Codes::FILE_RECEIVED_CRC_CODE || response_payload_size != PayloadSize::FILE_RECEIVED_CRC_PAYLOAD_SIZE || length != response_payload_size) {
				throw std::invalid_argument("server responded with an error");
			}

			Bytes payload_uuid(sizeof(UUID_SIZE));
			std::copy(response_payload.begin(), response_payload.begin() + sizeof(UUID_SIZE), payload_uuid.begin());

			if (!are_uuids_equal(payload_uuid, this->getHeader().getUUID())) {
				throw std::invalid_argument("server responded with an error");
			}


			uint32_t response_content_size = extractPayloadContentSize(response_payload);
			if (this->getPayload()->get_content_size() != response_content_size) {
				throw std::invalid_argument("server responded with an error");
			}

			// std::string response_file_name(response_payload.begin() + sizeof(uuid) + sizeof(content_size), response_payload.begin() + sizeof(uuid) + sizeof(content_size) + sizeof(file_name));
			string response_file_name = extractSendFileResponseFileName(response_payload);
			if (response_file_name != string(this->getPayload()->get_file_name())) {
				throw std::invalid_argument("server responded with an error");
			}

			// Copy the cksum content from the response_payload vector into the parameter cksum.
			unsigned long response_cksum = extractSendFileResponseCksum(response_payload);

			this->getPayloadReference().setCksum(response_cksum);

			// continue here to get the responses
			break;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		// Increment the i by 1 each iteration.
		times_sent++;
	}

	// If the times_sent reached MAX_REQUEST_FAILS, returning FAILURE
	if (times_sent >= MAX_REQUEST_FAILS) {
		return FAILURE;
	}
	// If the the SendPublicKeyRequest::run succeeded, return SUCCESS
	return SUCCESS;
}