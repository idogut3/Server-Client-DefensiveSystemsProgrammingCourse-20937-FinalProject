#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include "request.hpp"
#include "requests_payloads.hpp"


class RegisterRequest : public Request {
protected:
	RegistrationPayload payload;

public:
	RegisterRequest(RequestHeader header, RegistrationPayload payload);

};


class SendPublicKeyRequest : public Request {
private:
	SendPublicKeyPayload payload;

public:
	SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload)
}

class ReconnectRequest : public Request {
private:
	ReconnectionPayload payload;

public:
	ReconnectRequest(RequestHeader header, ReconnectionPayload payload)

};

class ValidCrcRequest : public Request {
private:
	ValidCrcPayload payload;

public:
	ValidCrcRequest(RequestHeader header, ValidCrcPayload payload);
};

class InvalidCrcRequest : public Request {
private:
	InvalidCrcPayload payload;

public:
	InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload);
};

class InvalidCrcDoneRequest : public Request {
private:
	InvalidCrcDonePayload payload;

public:
	InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload);
};

class SendFileRequest : public Request {
private:
	SendFilePayload payload;

public:
	SendFileRequest(RequestHeader header, SendFilePayload payload);
};

#endif