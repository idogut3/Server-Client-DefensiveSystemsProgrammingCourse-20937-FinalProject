#include "Header Files\utils.hpp"
#include "requests.hpp"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
	: Request(header), payload(payload) {}

Payload RegisterRequest::getPayload() const {
	return this->payload;
}


/*
	This method packs the header and payload for the registration request in a form of uint8_t vector.
	All numeric fields are ordered by little endian order.
*/
vector<uint8_t> RegisterRequest::pack_request() const {
	vector<uint8_t> request = pack_header();

	std::copy(name, name + sizeof(name), request.begin() + REQUEST_HEADER_SIZE);

	return request;
}



bool RegisterRequest::run(tcp::socket& sock) {
	// Pack request fields into vector and initialize parameter times_sent to 0.
	int times_sent = 1;
	vector<uint8_t> request = this->pack_request();

	while (times_sent <= MAX_FAILS) {
		try {
			// Send the request to the server via the provided socket.
			boost::asio::write(sock, boost::asio::buffer(request));

			// Receiving the header from the server, extracting response code and payload_size
			vector<uint8_t> response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = extractCodeFromResponseHeader(response_header);
			uint32_t response_payload_size = extractPayloadSizeFromResponseHeader(response_header);

			// Receiving the payload from the server and saving the num of bytes received from it
			vector<uint8_t> response_payload(response_payload_size);
			size_t num_of_bytes_received_from_server = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// If the code is wrong or we didn't receive enough bytes or the payload size we got is wrong
			if (response_code != Codes::REGISTRATION_CODE || response_payload_size != PayloadSize::REGISTRATION_SUCCEEDED_PAYLOAD_SIZE || num_of_bytes_received_from_server != response_payload_size) {
				throw std::invalid_argument("server responded with an error");
			}
			// The Registration succeeded, set the uuid to the id the server responded with.
			this->getHeader().setUUIDFromRawBytes(response_payload);

			break; // Existing the loop Registration was successful
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}

		times_sent++; // Meaning we failed registering 1 time because we catched an exception 
	}

	// If the times_sent reached MAX_FAILS, returning false
	if (times_sent == MAX_FAILS) {
		return false;
	}
	// If the the registration succeeded, return true.
	return true;
}


SendPublicKeyRequest::SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload)
	: Request(header), payload(payload) {}

Payload SendPublicKeyRequest::getPayload() const {
	return this->payload;
}

vector<uint8_t> SendPublicKeyRequest::pack_request() { //TODO :NOT DONE
	vector<uint8_t> request(REQUEST_HEADER_SIZE + this->getHeader()->getPayloadSize());

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


ReconnectRequest::ReconnectRequest(RequestHeader header, ReconnectionPayload payload)
	: Request(header), payload(payload) {}

Payload ReconnectRequest::getPayload() const {
	return this->payload;
}

ReconnectRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}



ValidCrcRequest::ValidCrcRequest(RequestHeader header, ValidCrcPayload payload)
	: Request(header), payload(payload) {}

Payload ValidCrcRequest::getPayload() const {
	return this->payload;
}

ValidCrcRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}


InvalidCrcRequest::InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcRequest::getPayload() const {
	return this->payload;
}

InvalidCrcRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}

InvalidCrcDoneRequest::InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcDoneRequest::getPayload() const {
	return this->payload;
}

InvalidCrcDoneRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}


SendFileRequest::SendFileRequest(RequestHeader header, SendFilePayload payload)
	: Request(header), payload(payload) {}

Payload SendFileRequest::getPayload() const {
	return this->payload;
}
SendFileRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}
