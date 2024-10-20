#ifndef REQUEST_PAYLOADS_HPP
#define REQUEST_PAYLOADS_HPP
#include "request.hpp"

class RegistrationPayload : public Payload {
protected:
	char username[255];
public:
	RegistrationPayload(const char* username);

	const char* getUsername() const { return username;}

    Bytes pack_payload() const override;
};


class SendPublicKeyPayload : public Payload {
protected:
    char username[255];
    char public_key[160];

public:

    SendPublicKeyPayload(const char* username, const char* public_key);

    const char* getUsername() const;
    const char* getPublicKey() const;

    Bytes pack_payload() const override;
};



class ReconnectionPayload : public Payload {
private:
    char username[255];

public:
    ReconnectionPayload(const char* username);

    const char* getUsername() const;

    Bytes pack_payload() const override;
};



class ValidCrcPayload : public Payload {
protected:
    char file_name[255];

public:
    ValidCrcPayload(const char* file_name);

    const char* getFileName() const;

    Bytes pack_payload() const override;
};




class InvalidCrcPayload : public Payload {
protected:
    char file_name[255];

public:
    InvalidCrcPayload(const char* file_name);
    const char* getFileName() const;

    Bytes pack_payload() const override;
};



class InvalidCrcDonePayload : public Payload {
protected:
    char file_name[255];

public:
    InvalidCrcDonePayload(const char* file_name);
    const char* getFileName() const;

    Bytes pack_payload() const;
};



class SendFilePayload : public Payload {
protected:
    uint32_t content_size; // 4 bytes = 32 bits
    uint32_t orig_file_size; // 4 bytes = 32 bits
    uint16_t packet_number; // 2 bytes = 16 bits
    uint16_t total_packets; // 2 bytes = 16 bits
    char file_name[NAME_LENGTH];
    string encrypted_file_content; 
public:
    SendFilePayload(uint32_t content_size, uint32_t orig_file_size, uint16_t packet_number, uint16_t total_packets, char file_name[]);
    uint32_t get_content_size() const;
    uint32_t get_orig_file_size() const;
    uint16_t get_packet_number() const;
    uint16_t get_total_packets() const;
    const char* get_file_name() const;
    const string& get_encrypted_file_content() const;

    Bytes pack_payload() const override;
};



#endif