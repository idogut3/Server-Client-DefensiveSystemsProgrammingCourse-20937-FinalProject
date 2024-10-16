#include "client-side\Header Files\utils.hpp"
#include "Header Files\request_header_files\request.hpp"
#include "Header Files\request_header_files\requests.hpp"
#include "Header Files\request_header_files\requests_payloads.hpp"


static bool transferValidation(client& client, string ip_port, string name, string file_path) {
	size_t colon_postion = ip_port.find(':');

	if (colon_postion == string::npos || name.length() > MAX_NAME_LENGTH || name.length() == 0 || file_path.length() == 0) {
		return false;
	}

	string ip = ip_port.substr(0, colon_postion);
	string port = ip_port.substr(colon_postion + 1);

	bool is_port_valid = is_integer(port);
	if (!is_port_valid) {
		return false;
	}

	setupClient(client, ip, port, name, file_path);

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
		cout << "'" << line << "' " << line.length() << endl;
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
		cout << "'" << line << "' " << line.length() << endl;
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

	if (lines != 4 || client_name.length() > MAX_NAME_LENGTH || client_name.length() == 0 || client_id.length() != HEX_ID_LENGTH || private_key.length() == 0) {
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
	string path_key = EXE_DIR_FILE_PATH("priv.key");
	ofstream info_file(path_info), key_file(path_key);

	if (!info_file.is_open()) {
		throw std::runtime_error("Error opening the 'me.info' - exiting");
	}
	if (!key_file.is_open()) {
		throw std::runtime_error("Error opening the 'priv.key' - exiting");
	}

	// Writing to both files.
	info_file << name << endl << id << endl << base64_private_key << endl;
	key_file << base64_private_key << endl;

	info_file.close();
	key_file.close();
}

//static void run_client(tcp::socket& sock, Client& client) {
//	bool operation_success;
//	string private_key, decrypted_aes_key;
//
//	// If me.info does not exist, send Registration request.
//	if (!filesystem::exists(EXE_DIR_FILE_PATH("me.info"))) {
//
//		RequestHeader request_header(client.getUuid(), Codes::REGISTRATION_CODE, PayloadSize::REGISTRATION_PAYLOAD_SIZE);
//		RegistrationPayload registration_payload(client.getName().c_str());
//		RegisterRequest register_request(request_header, registration_payload)
//		// operation_success = register_request.run(sock);
//
//		if (!operation_success) {
//			FATAL_MESSAGE_RETURN("Registration");
//		}
//
//		// Create RSA pair, save fields data into me.info and prev.key files, and send a SendingPublicKey request.
//		RSAPrivateWrapper prevKeyWrapper;
//		string public_key = prevKeyWrapper.getPublicKey();
//		private_key = prevKeyWrapper.getPrivateKey();
//		save_to_files(client.getName(), client.getUuid(), private_key);
//		SendingPublicKey sending_pub_key(client.getUuid(), Codes::SENDING_PUBLIC_KEY_C, PayloadSize::SENDING_PUBLIC_KEY_P, client.getName().c_str(), public_key.c_str());
//		operation_success = sending_pub_key.run(sock);
//
//		if (!operation_success) {
//			FATAL_MESSAGE_RETURN("Sending Public Key");
//		}
//
//		// Get the encrypted AES key and decrypt it.
//		string encrypted_aes_key = sending_pub_key.getEncryptedAesKey();
//		decrypted_aes_key = prevKeyWrapper.decrypt(encrypted_aes_key);
//	}
//	else { // If me.info does exist, read id and send reconnection request.
//		// Read the fields from the client.
//		string key_base64 = read_from_files(client);
//
//		// Send Reconnection request to the server.
//		Reconnection reconnection(client.getUuid(), Codes::RECONNECTION_C, PayloadSize::RECONNECTION_P, client.getName().c_str());
//		operation_success = reconnection.run(sock);
//
//		if (!operation_success) {
//			FATAL_MESSAGE_RETURN("Reconnection");
//		}
//
//		// Decode the private key and create the decryptor.
//		private_key = Base64Wrapper::decode(key_base64);
//		RSAPrivateWrapper prevKeyWrapper(private_key);
//
//		// Get the encrypted AES key and decrypt it.
//		string encrypted_aes_key = reconnection.getEncryptedAesKey();
//		decrypted_aes_key = prevKeyWrapper.decrypt(encrypted_aes_key);
//	}
//	AESWrapper aesKeyWrapper(reinterpret_cast<const unsigned char*>(decrypted_aes_key.c_str()), decrypted_aes_key.size());
//	int file_error_cnt = 0, times_crc_sent = 0;
//	while (file_error_cnt != MAX_FAILS && times_crc_sent != MAX_FAILS) {
//		// Get the file's content, save the encrypted content and save the sizes of both.
//		std::string content = fileToString(client.getFilePath());
//		std::string encrypted_content = aesKeyWrapper.encrypt(content.c_str(), content.length());
//		uint32_t content_size = encrypted_content.length();
//		uint32_t orig_size = content.length();
//
//		// Save the total packets and send the Sending File request to the server.
//		uint16_t total_packs = TOTAL_PACKETS(content_size);
//		SendingFile sendingFile(client.getUuid(), Codes::SENDING_FILE_C, PayloadSize::SENDING_FILE_P, content_size, orig_size, total_packs, client.getFilePath().c_str(), encrypted_content);
//		operation_success = sendingFile.run(sock);
//		// If the sending file request did not succeed, add 1 to sending file error counter and continue the loop.
//		if (!operation_success) {
//			file_error_cnt++;
//			continue;
//		}
//
//		// Get the cksum the server responded with.
//		std::string response_cksum = sendingFile.getCksum();
//		if (response_cksum == readfile(EXE_DIR_FILE_PATH(client.getFilePath()))) {
//			break;
//		}
//
//		// If the crc given by the server is incorrect, send Sending Crc Again request - 901.
//		SendingCrcAgain sendingCrcAgain(client.getUuid(), Codes::SENDING_CRC_AGAIN_C, PayloadSize::SENDING_CRC_AGAIN_P, client.getFilePath().c_str());
//
//		// If the sending crc request did not succeed, add 1 to times crc sent counter.
//		times_crc_sent++;
//	}
//	if (file_error_cnt == MAX_FAILS) {
//		FATAL_MESSAGE_RETURN("Sending File");
//	}
//	else if (times_crc_sent == MAX_FAILS) {
//		InvalidCrcDone invalid_crc_done(client.getUuid(), Codes::INVALID_CRC_DONE_C, PayloadSize::INVALID_CRC_DONE_P, client.getFilePath().c_str());
//		invalid_crc_done.run(sock);
//	}
//	else {
//		ValidCrc valid_crc(client.getUuid(), Codes::VALID_CRC_C, PayloadSize::VALID_CRC_P, client.getFilePath().c_str());
//	}
//}

//int main()
//{
//	std::cout << "Hello World!\n";
//}
//
