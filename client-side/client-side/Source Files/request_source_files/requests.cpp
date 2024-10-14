#include "Header Files\request_header_files\requests.h"

RegisterRequest::RegisterRequest(RequestHeader header, RegistrationPayload payload)
    : Request(header), payload(payload) {}

Payload RegisterRequest::getPayload() const {
    return payload; 
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

SendFileRequset::SendFileRequset(RequestHeader header, SendFilePayload payload)
    : Request(header), payload(payload) {}
Payload SendFileRequset::getPayload() const {
    return payload; // Return the payload
}
