#ifndef REQUEST_PAYLOADS_H
#define REQUEST_PAYLOADS_H

class RegistrationPayload : Payload {
protected:
	char username[255];
public:
	RegistrationPayload(const char* username);
	const char* getUsername() const { return username; }
};


class SendPublicKeyPayload : Payload {
protected:
    char username[255];
    char public_key[160];

public:

    SendPublicKeyPayload(const char* username, const char* public_key);
    const char* getUsername() const;
    const char* getPublicKey() const;
};

class ReconnectionPayload : Payload {
private:
    char username[255];

public:
    ReconnectionPayload(const char* username);

    const char* getUsername() const;
};

class ValidCrcPayload : Payload {
protected:
    char file_name[255];

public:
    ValidCrcPayload(const char* file_name);
    const char* getFileName() const;
};


class InvalidCrcPayload : Payload {
protected:
    char file_name[255];

public:
    InvalidCrcPayload(const char* file_name);
    const char* getFileName() const;
};

class InvalidCrcDonePayload : Payload {
protected:
    char file_name[255];

public:
    InvalidCrcDonePayload(const char* file_name);
    const char* getFileName() const;
};

class SendFilePayload {
protected:
    uint32_t content_size; // 4 bytes = 32 bits
    uint32_t orig_file_size; // 4 bytes = 32 bits
    uint16_t packet_number; // 2 bytes = 16 bits
    uint16_t total_packets; // 2 bytes = 16 bits
    char file_name[255];
    //TODO: message conntent how to save it idkkkkkAAAA 
public:
    SendFilePayload(uint32_t content_size, uint32_t orig_file_size, uint16_t packet_number, uint16_t total_packets, char file_name[]);
};



#endif