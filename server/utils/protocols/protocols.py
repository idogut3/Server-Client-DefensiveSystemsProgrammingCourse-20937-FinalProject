import struct
from abc import abstractmethod

from server.utils.protocols.Response import Response, Header
from server.utils.protocols.send_file_request.message_handling import \
    extract_relevant_values_from_send_file_request_message, \
    extract_relevant_values_from_crc_conformation_message_as_dict
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes
from server.utils.protocols.codes.server_reply_codes_enum import ServerReplyCodes
from server.utils.protocols.send_file_request.send_file_protocol_utils import calculate_checksum_value
from server.utils.server_utils import unpack_message
from server.utils.encryption_decryption_utils.rsa_encrtption_decryption import decrypt_file_with_aes_key


class Protocol:
    def __init__(self, server, conn):
        self.server = server
        self.conn = conn

    @abstractmethod
    def protocol(self, message):
        pass


class RegisterRequestProtocol(Protocol):
    def __init__(self, server, conn):
        super().__init__(server, conn)

    def protocol(self, message) -> bool:
        message_dict = unpack_message(message)
        payload = message_dict["payload"]
        username = payload[:255].decode('utf-8')

        if not self.server.get_database().is_username_already_registered(username):
            accepted_register_request, uuid = self.accept_register_request(username)
            if accepted_register_request:
                received_correctly, user_public_key = self.receive_public_key(uuid, username)
                if received_correctly:
                    encrypted_aes_key = self.server.get_database().set_new_user_public_key(username=username,
                                                                                           public_key=user_public_key)
                    if encrypted_aes_key != "":
                        self.send_encrypted_aes_key_reply(uuid, encrypted_aes_key)
                        return True
                    else:
                        self.send_register_failed_reply()
                        return False
                else:
                    self.send_register_failed_reply()
                    return False
            else:
                self.send_register_failed_reply()
                return False
        else:
            self.send_register_failed_reply()
            return False

    def build_accept_register_request_reply(self, uuid) -> Response:
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.REGISTERED_SUCCESSFULLY)
        payload_format = '<16s'
        # The format string '<' indicates little-endian format.
        # '16s' means a string of 16 bytes
        payload = struct.pack(payload_format, uuid.encode('utf-8'))
        reply = Response(header=reply_header, payload=payload)
        return reply

    def accept_register_request(self, username):
        was_able_to_add_user, uuid = self.server.get_database().add_new_user_to_database(username=username)
        if not was_able_to_add_user:
            self.send_register_failed_reply()
            return False, ""
        reply = self.build_accept_register_request_reply(uuid)
        reply.response(self.conn)
        return True, uuid

    def build_register_failed_reply(self) -> Response:
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.REGISTRATION_FAILED)
        reply = Response(reply_header)
        return reply

    def send_register_failed_reply(self):
        reply = self.build_register_failed_reply()
        reply.response(self.conn)

    def receive_public_key(self, uuid, username):
        # Receive the fixed-size header (16 + 1 + 2 + 4 = 23 bytes)
        header_size = 23
        header_data = self.conn.recv(header_size)

        # Unpack the header
        header_format = '<16sBHI'  # Little-endian: 16s (client_id), B (version), H (code), I (payload_size)
        client_id, version, code, payload_size = struct.unpack(header_format, header_data)
        client_id_str = client_id.decode('utf-8').rstrip('\x00')  # Convert bytes to string and strip null bytes

        if code != ClientReplyCodes.SEND_PUBLIC_KEY_REQUEST or client_id_str != uuid:
            return False, ""

        payload_data = self.conn.recv(payload_size)
        payload_format = "255s 160s"
        received_username, public_key = struct.unpack(payload_format, payload_data)

        received_username = received_username.decode('utf-8').rstrip(
            '\x00')  # Convert bytes to string and strip null bytes
        if received_username != username:
            return False, ""

        return True, public_key

    def send_encrypted_aes_key_reply(self, uuid, encrypted_aes_key):
        reply = self.build_send_encrypted_aes_key_reply(uuid, encrypted_aes_key)
        reply.response(self.conn)

    def build_send_encrypted_aes_key_reply(self, uuid, encrypted_aes_key) -> Response:
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.PUBLIC_KEY_RECEIVED_SENDING_ENCRYPTED_AES_KEY,
                              payload_size=len(encrypted_aes_key))
        payload_format = f'16s{len(encrypted_aes_key)}s'  # Create the format string based 16 byte uuid and the length of the encrypted_aes_key
        packed_payload = struct.pack(payload_format, uuid.encode('utf-8'), encrypted_aes_key.encode('utf-8'))
        return Response(reply_header, packed_payload)


class SendFileRequestProtocol(Protocol):
    def __init__(self, server, conn):
        super().__init__(server, conn)

    def protocol(self, message):
        message_dict = self.handle_send_file_request_message(message)
        message_file_name = message_dict["file_name"]
        decrypted_file_content = message_dict["decrypted_file_content"]
        # TODO: more code here .........
        client_crc_conformation_message = self.receive_client_crc_conformation_message()

    def receive_client_crc_conformation_message(self):
        client_crc_conformation_message_length = 278  # client_id -> 16 bytes + version -> 1 byte  + Code -> 2 bytes + payload size -> 4 bytes + file_name -> 255 bytes
        client_crc_conformation_message = self.conn.recv(client_crc_conformation_message_length)
        return client_crc_conformation_message

    def handle_crc_conformation_reply(self, client_crc_conformation_message):
        client_crc_conformation_message_dict = extract_relevant_values_from_crc_conformation_message_as_dict(
            client_crc_conformation_message)

        client_id = client_crc_conformation_message_dict["conformation_reply_client_id"]
        code = client_crc_conformation_message_dict["conformation_reply_code"]
        conformation_reply_file_name = client_crc_conformation_message_dict["conformation_reply_file_name"]
        #TODO: checks for client id, file name ect ect ...
        if code == ClientReplyCodes.ADEQUATE_CRC_VALUE:
            pass
            #TODO: save the file


    def handle_send_file_request_message(self, message):
        client_id, encrypted_content_size, original_file_size, packet_number, total_packets, message_file_name, encrypted_message_content = \
            extract_relevant_values_from_send_file_request_message(message)

        user_aes_key = self.server.get_database.get_aes_key_by_uuid(client_id)
        decrypted_file_content = decrypt_file_with_aes_key(encrypted_message_content, user_aes_key)
        file_checksum_value = calculate_checksum_value(decrypted_file_content)
        self.send_user_file_received_message(client_id, encrypted_content_size, message_file_name, file_checksum_value)
        return {"file_name": message_file_name, "decrypted_file_content": decrypted_file_content}

    def send_user_file_received_message(self, client_id, encrypted_content_size, message_file_name,
                                        file_checksum_value):
        reply = self.build_user_file_received_message_reply(client_id, encrypted_content_size, message_file_name,
                                                            file_checksum_value)
        reply.response(self.conn)

    def build_user_file_received_message_reply(self, client_id, encrypted_content_size, message_file_name,
                                               file_checksum_value) -> Response:
        # 16 bytes (client_id) + 4 bytes (encrypted_content_size) + 255 bytes (file_name) + 4 bytes (checksum_value) = 279 (payload size)
        payload_size = 279
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.FILE_RECEIVED_SUCCESSFULLY_WITH_CRC,
                              payload_size=payload_size)
        # Format string: 16 bytes for Client ID, 4 bytes for encrypted content Size, 255 bytes for File Name, 4 bytes for Checksum
        message_format = '<16s I 255s I'
        client_id_bytes = client_id.encode("utf-8")
        file_name_bytes = message_file_name.encode("utf-8")
        packed_payload = struct.pack(message_format, client_id_bytes, encrypted_content_size, file_name_bytes,
                                     file_checksum_value)
        return Response(reply_header, packed_payload)


class ReconnectToServerRequestProtocol(Protocol):
    def __init__(self, server, conn):
        super().__init__(server, conn)

    def protocol(self, message):
        pass
