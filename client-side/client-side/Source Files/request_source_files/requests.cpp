#include "Header Files\request_header_files\requests.hpp"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
	: Request(header), payload(payload) {}

Payload RegisterRequest::getPayload() const {
	return payload;
}

SendPublicKeyRequest::SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload)
	: Request(header), SendPublicKeyPayload(payload) {}

Payload SendPublicKeyRequest::getPayload() const {
	return payload;
}

ReconnectRequest::ReconnectRequest(RequestHeader header, ReconnectionPayload payload)
	: Request(header), ReconnectionPayload(payload) {}

Payload ReconnectRequest::getPayload() const {
	return payload;
}

ValidCrcRequest::ValidCrcRequest(RequestHeader header, ValidCrcPayload payload)
	: Request(header), ValidCrcPayload(payload) {}

	Payload ValidCrcRequest::getPayload() const {
		return payload;
	}

	InvalidCrcRequest::InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload)
		: Request(header), InvalidCrcPayload(payload) {}

	Payload InvalidCrcRequest::getPayload() const {
		return payload;
	}

	InvalidCrcDoneRequest::InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload)
		: Request(header), InvalidCrcDonePayload(payload) {}

	Payload InvalidCrcDoneRequest::getPayload() const {
		return payload;
	}

	SendFileRequest::SendFileRequest(RequestHeader header, SendFilePayload payload)
		: Request(header), SendFilePayload(payload) {}

	Payload SendFileRequest::getPayload() const {
		return payload; // Return the payload
	}

