#ifndef CLIENT_H
#define CLIENT_H

#include "utils.hpp"


class Client {
	std::string address;
	std::string port;
	std::string name;
	std::string file_path;
	UUID uuid;

public:
	Client();
	void setAddress(std::string address);
	void setPort(std::string port);
	void setName(std::string name);
	void setFilePath(std::string file_path);
	void setUUID(UUID uuid);

	std::string getAddress() const;
	std::string getPort() const;
	std::string getName() const;
	std::string getFilePath() const;
	UUID getUuid() const;
	Client setupClient(Client client, const std::string& ip, int port, const std::string& name, const std::string& filePath);
};

#endif