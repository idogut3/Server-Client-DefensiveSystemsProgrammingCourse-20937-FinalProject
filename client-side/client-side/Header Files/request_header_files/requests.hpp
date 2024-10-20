#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include "request.hpp"
#include "requests.hpp"
#include "requests_payloads.hpp"


class RegisterRequest : public Request {
protected:
	RegistrationPayload payload;

public:
	RegisterRequest(RequestHeader header, RegistrationPayload payload);
	const RegistrationPayload* getPayload() const override;

	Bytes pack_request() const;
	bool run(tcp::socket& sock);
};


class SendPublicKeyRequest : public Request {
private:
	SendPublicKeyPayload payload;

public:
	SendPublicKeyRequest(RequestHeader header, SendPublicKeyPayload payload);
	const SendPublicKeyPayload* getPayload() const override;

	Bytes pack_request();
	bool run(tcp::socket& sock);
};



class ReconnectRequest : public Request {
private:
	ReconnectionPayload payload;

public:
	ReconnectRequest(RequestHeader header, ReconnectionPayload payload);
	const ReconnectionPayload* getPayload() const override;

	bool run(tcp::socket& sock);
};



class ValidCrcRequest : public Request {
private:
	ValidCrcPayload payload;

public:
	ValidCrcRequest(RequestHeader header, ValidCrcPayload payload);
	const ValidCrcPayload* getPayload() const override;

	bool run(tcp::socket& sock);
};



class InvalidCrcRequest : public Request {
private:
	InvalidCrcPayload payload;

public:
	InvalidCrcRequest(RequestHeader header, InvalidCrcPayload payload);
	const InvalidCrcPayload* getPayload() const override;

	bool run(tcp::socket& sock);
};



class InvalidCrcDoneRequest : public Request {
private:
	InvalidCrcDonePayload payload;

public:
	InvalidCrcDoneRequest(RequestHeader header, InvalidCrcDonePayload payload);
	const InvalidCrcDonePayload* getPayload() const override;

	bool run(tcp::socket& sock);
};



class SendFileRequest : public Request {
private:
	SendFilePayload payload;

public:
	SendFileRequest(RequestHeader header, SendFilePayload payload);
	const SendFilePayload* getPayload() const override;

	bool run(tcp::socket& sock);
};

#endif