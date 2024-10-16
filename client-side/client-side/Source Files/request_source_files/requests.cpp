#include "Header Files\utils.hpp"
#include "requests.hpp"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
	: Request(header), payload(payload) {}

Payload RegisterRequest::getPayload() const {
	return payload;
}

bool RegisterRequest::run(tcp::socket& sock) {
	// Pack request fields into vector and initialize parameter times_sent to 0.
	int times_sent = 0;
	vector<uint8_t> request = pack_registration_request();

	while (times_sent != MAX_FAILS) {
		try {
			// Send the request to the server via the provided socket.
			boost::asio::write(sock, boost::asio::buffer(request));

			// Receive header from the server, get response code and payload_size
			vector<uint8_t> response_header(RESPONSE_HEADER_SIZE);
			boost::asio::read(sock, boost::asio::buffer(response_header, RESPONSE_HEADER_SIZE));
			uint16_t response_code = get_response_code(response_header);
			uint32_t response_payload_size = get_response_payload_size(response_header);

			// Receive payload from the server, save it's length in a parameter length.
			vector<uint8_t> response_payload(response_payload_size);
			size_t length = boost::asio::read(sock, boost::asio::buffer(response_payload, response_payload_size));

			// If the code is not success, the payload_size for the code is not the same as the size received in the header, or the length of the payload is not the wanted length, print error.
			if (response_code != Codes::REGISTRATION_SUCCEEDED_C || response_payload_size != PayloadSize::REGISTRATION_SUCCEEDED_P || length != response_payload_size) {
				throw std::invalid_argument("server responded with an error.");
			}
			// The Registration succeeded, set the uuid to the id the server responded with.
			for (int i = 0; i < response_payload.size(); i++) {
				uuid.data[i] = (response_payload[i] >> 4);
				uuid.data[i + 1] = (response_payload[i] & 0xf);
			}
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



ReconnectRequest::ReconnectRequest(RequestHeader header, ReconnectionPayload payload)
	: Request(header), payload(payload) {}

Payload ReconnectRequest::getPayload() const {
	return payload;
}



ValidCrcRequest::ValidCrcRequest(RequestHeader header, ValidCrcPayload payload)
	: Request(header), payload(payload) {}

	Payload ValidCrcRequest::getPayload() const {
		return payload;
	}



InvalidCrcRequest::InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcRequest::getPayload() const {
	return payload;
}



InvalidCrcDoneRequest::InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload)
	: Request(header), payload(payload) {}

Payload InvalidCrcDoneRequest::getPayload() const {
	return payload;
}



SendFileRequest::SendFileRequest(RequestHeader header, SendFilePayload payload)
	: Request(header), payload(payload) {}

Payload SendFileRequest::getPayload() const {
	return payload;
}

