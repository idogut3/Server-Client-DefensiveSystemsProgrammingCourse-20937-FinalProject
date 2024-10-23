#include "Header Files\request_header_files\request.hpp"
#include "Header Files\request_header_files\requests.hpp"
#include "Header Files\request_header_files\requests_payloads.hpp"
#include "../Header Files/utils.hpp"
#include "../Header Files/clients/client.hpp"

static bool transferValidation(Client& client, string ip_port, string name, string file_path) {
	size_t colon_postion = ip_port.find(':');

	if (colon_postion == string::npos || name.length() > MAX_USERNAME_LENGTH || name.length() == 0 || file_path.length() == 0) {
		return false;
	}

	string ip = ip_port.substr(0, colon_postion);
	string port = ip_port.substr(colon_postion + 1);

	bool is_port_valid = is_integer(port);
	if (!is_port_valid) {
		return false;
	}

	client.setupClient(ip, port, name, file_path);

	return true;
}

static Client createClient() {
	string transfer_path = EXE_DIR_FILE_PATH("transfer.info");
	string line, ip_port, client_name, client_file_path;
	ifstream transfer_info_file(transfer_path);

	int lines = 1;
	Client client;

	if (!transfer_info_file.is_open()) {
		throw std::runtime_error("Error opening 'transfer.info' - exiting");
	}

	while (getline(transfer_info_file, line)) {
		cout << "'" << line << "' " << line.length() << "\n";
		switch (lines) {
		case 1:
			ip_port = line;
			break;
		case 2:
			client_name = line;
			break;
		case 3:
			client_file_path = line;
			break;
		default:
			break;
		}
		lines++;
	}

	if (lines != 4) {
		throw std::invalid_argument("Error: transfer.info contains too many lines / not enough lines");
	}

	if (!transferValidation(client, ip_port, client_name, client_file_path)) {
		throw std::invalid_argument("Error: transfer.info contains invalid data");
	}

	transfer_info_file.close();
	return client;

}

static string read_me_info_file(Client& client) {
	string me_info_path = EXE_DIR_FILE_PATH("me.info");
	string line, client_name, client_id, private_key;
	int lines = 1;
	ifstream info_file(me_info_path);

	if (!info_file.is_open()) {
		throw std::runtime_error("Error opening 'me.info' - exiting");
	}

	while (getline(info_file, line)) {
		cout << "'" << line << "' " << line.length() << "\n";
		switch (lines) {
		case 1:
			client_name = line;
			break;
		case 2:
			client_id = line;
			break;
		case 3:
			private_key = line;
			break;
		default:
			break;
		}
		lines++;
	}

	if (lines != 4 || client_name.length() > MAX_USERNAME_LENGTH || client_name.length() == 0 || client_id.length() != HEX_ID_LENGTH || private_key.length() == 0) {
		throw std::invalid_argument("Error: me.info contains invalid data.");
	}

	UUID id = getUUIDFromString(client_id);
	client.setName(client_name);
	client.setUUID(id);

	info_file.close();
	return private_key;
}

static void save_me_info(string name, UUID uuid, string private_key) {
	string my_uuid = uuids::to_string(uuid);
	my_uuid.erase(remove(my_uuid.begin(), my_uuid.end(), '-'), my_uuid.end()); // Remove '-' from the string
	string base64_private_key = Base64Wrapper::encode(private_key);

	string path_info = EXE_DIR_FILE_PATH("me.info");

	ofstream info_file(path_info);

	if (!info_file.is_open()) {
		throw std::runtime_error("Error opening the 'me.info' - exiting");
	}

	// Writing to info file
	info_file << name << "\n" << my_uuid << "\n" << base64_private_key << "\n";

	info_file.close();
}
static void save_priv_key_file(string private_key) {
	// Encode the private key to base64 and open files
	string base64_private_key = Base64Wrapper::encode(private_key);
	string path_key = EXE_DIR_FILE_PATH("priv.key");

	ofstream private_key_file(path_key);

	if (!private_key_file.is_open()) {
		throw std::runtime_error("Error opening the 'priv.key' file, aborting program.");
	}
	// Writing to priv.key file
	private_key_file << base64_private_key << "\n";
	private_key_file.close();
}


static void run_client(tcp::socket& sock, Client& client) {
	int operation_success;
	string private_key, decrypted_aes_key;

	// if me.info does not exist, send registration request.
	if (!(std::filesystem::exists(EXE_DIR_FILE_PATH("me.info")))) {
		RequestHeader request_header(client.getUuid(), Codes::REGISTRATION_CODE, PayloadSize::REGISTRATION_PAYLOAD_SIZE);
		RegistrationPayload registration_payload(client.getName().c_str());
		RegisterRequest register_request(request_header, registration_payload);
		operation_success = register_request.run(sock);

		if (!operation_success) {
			FATAL_MESSAGE_RETURN("registration");
		}

		// create rsa pair, save fields data into me.info and prev.key files, and send a sendingpublickey request.
		RSAPrivateWrapper privatekeywrapper;

		string public_key = privatekeywrapper.getPublicKey();
		private_key = privatekeywrapper.getPrivateKey();

		// saving files as required for future (reconnection etc)
		save_me_info(client.getName(), client.getUuid(), private_key);
		save_priv_key_file(private_key);

		sendingpublickey sending_pub_key(client.getUuid(), Codes::SENDING_PUBLIC_KEY_CODE, PayloadSize::SENDING_PUBLIC_KEY_PAYLOAD_SIZE, client.getName().c_str(), public_key.c_str());
		operation_success = sending_pub_key.run(sock);

		if (!operation_success) {
			FATAL_MESSAGE_RETURN("sending public key");
		}

		// get the encrypted aes key and decrypt it.
		string encrypted_aes_key = sending_pub_key.getencryptedaeskey();
		decrypted_aes_key = privatekeywrapper.decrypt(encrypted_aes_key);
	}

	else 
	{ // if me.info does exist, read id and send reconnection request.
		// read the fields from the client.
		string key_base64 = read_from_files(client);

		// send reconnection request to the server.
		reconnection reconnection(client.getUuid(), Codes::RECONNECTION_CODE, PayloadSize::RECONNECTION_PAYLOAD_SIZE, client.getName().c_str());
		operation_success = reconnection.run(sock);

		if (!operation_success) {
			FATAL_MESSAGE_RETURN("reconnection");
		}

		// decode the private key and create the decryptor.
		private_key = Base64Wrapper::decode(key_base64);
		RSAPrivateWrapper privatekeywrapper(private_key);

		// get the encrypted aes key and decrypt it.
		string encrypted_aes_key = reconnection.getencryptedaeskey();
		decrypted_aes_key = privatekeywrapper.decrypt(encrypted_aes_key);
	}
	AESWrapper aeskeywrapper(reinterpret_cast<const unsigned char*>(decrypted_aes_key.c_str()), decrypted_aes_key.size());
	int file_error_cnt = 0, times_crc_sent = 0;
	while (file_error_cnt != MAX_REQUEST_FAILS && times_crc_sent != MAX_REQUEST_FAILS) {
		// get the file's content, save the encrypted content and save the sizes of both.
		std::string content = filetostring(client.getFilePath());
		std::string encrypted_content = aeskeywrapper.encrypt(content.c_str(), content.length());
		uint32_t content_size = encrypted_content.length();
		uint32_t orig_size = content.length();

		// save the total packets and send the sending file request to the server.
		uint16_t total_packs = total_packets(content_size);
		sendingfile sendingfile(client.getUuid(), Codes::SENDING_FILE_CODE, PayloadSize::sending_file_p, content_size, orig_size, total_packs, client.getfilepath().c_str(), encrypted_content);
		operation_success = sendingfile.run(sock);
		// if the sending file request did not succeed, add 1 to sending file error counter and continue the loop.
		if (!operation_success) {
			file_error_cnt++;
			continue;
		}

		// get the cksum the server responded with.
		std::string response_cksum = sendingfile.getcksum();
		if (response_cksum == readfile(EXE_DIR_FILE_PATH(client.getFilePath()))) {
			break;
		}

		// if the crc given by the server is incorrect, send sending crc again request - 901.
		sendingcrcagain sendingcrcagain(client.getUuid(), Codes::SENDING_CRC_AGAIN_CODE, PayloadSize::INVALID_CRC_PAYLOAD_SIZE, client.getFilePath().c_str());

		// if the sending crc request did not succeed, add 1 to times crc sent counter.
		times_crc_sent++;
	}
	if (file_error_cnt == MAX_REQUEST_FAILS) {
		FATAL_MESSAGE_RETURN("sending file");
	}
	else if (times_crc_sent == MAX_REQUEST_FAILS) {
		invalidcrcdone invalid_crc_done(client.getUuid(), Codes::INVALID_CRC_DONE_CODE, PayloadSize::INVALID_CRC_DONE_PAYLOAD_SIZE, client.getFilePath().c_str());
		invalid_crc_done.run(sock);
	}
	else {
		validcrc valid_crc(client.getUuid(), Codes::VALID_CRC_CODE, PayloadSize::VALID_CRC_PAYLOAD_SIZE, client.getFilePath().c_str());
	}
}


int main()
{
	std::cout << "Hello World!\n";
}

