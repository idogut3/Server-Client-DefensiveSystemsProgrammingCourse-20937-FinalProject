#include "Header Files\utils.hpp"
#include "requests.hpp"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
	: Request(header), payload(payload) {}

Payload RegisterRequest::getPayload() const {
	return payload;
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
			/*for (int i = 0; i < response_payload.size(); i++) {
				uuid.data[i] = (response_payload[i] >> 4);
				uuid.data[i + 1] = (response_payload[i] & 0xf);
			}*/
			std::memcpy(this->uuid.data, response_payload, 16);
			// If this code is reached, there was no error and the Registration was successful, so we break from the loop.
			break;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		// Increment the i by 1 each iteration.
		times_sent++;
	}

	// If the i reached 3, return false.
	if (times_sent == MAX_FAILS) {
		return false;
	}
	// If the client succeeded, return true.
	return true;
}


SendPublicKeyRequest::SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload)
	: Request(header), payload(payload) {}

Payload SendPublicKeyRequest::getPayload() const {
	return payload;
}

SendPublicKeyRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}


ReconnectRequest::ReconnectRequest(RequestHeader header, ReconnectionPayload payload)
	: Request(header), payload(payload) {}

Payload ReconnectRequest::getPayload() const {
	return payload;
}

ReconnectRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}



ValidCrcRequest::ValidCrcRequest(RequestHeader header, ValidCrcPayload payload)
	: Request(header), payload(payload) {}

Payload ValidCrcRequest::getPayload() const {
	return payload;
}

ValidCrcRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}


InvalidCrcRequest::InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcRequest::getPayload() const {
	return payload;
}

InvalidCrcRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}

InvalidCrcDoneRequest::InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcDoneRequest::getPayload() const {
	return payload;
}
InvalidCrcDoneRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}


SendFileRequest::SendFileRequest(RequestHeader header, SendFilePayload payload)
	: Request(header), payload(payload) {}

Payload SendFileRequest::getPayload() const {
	return payload;
}

SendFileRequest::vector<uint8_t> pack_request() { //TODO :NOT DONE
	vector<uint8_t> ret(1);
	return ret(1)
}
