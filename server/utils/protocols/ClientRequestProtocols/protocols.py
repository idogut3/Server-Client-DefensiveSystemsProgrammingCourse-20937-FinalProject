import struct
from abc import abstractmethod

from Crypto.SelfTest.Protocol.test_ecdh import public_key

from server.utils.protocols.ClientRequestProtocols.Response import Response, Header
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes
from server.utils.protocols.codes.server_reply_codes_enum import ServerReplyCodes


def unpack_message(message):
    # Extract fields according to the given structure
    client_id = message[:16]  # 16 bytes for Client ID
    version = message[16:17]  # 1 byte for Version
    code = message[17:19]  # 2 bytes for Code
    payload_size = struct.unpack("<I", message[19:23])[0]  # 4 bytes for Payload size (little-endian)
    payload = message[23:23 + payload_size]  # Extract the payload of given size
    return {
        "client_id": client_id.decode('utf-8', 'ignore'),
        "version": version.decode('utf-8', 'ignore'),
        "code": int.from_bytes(code, 'little'),
        "payload_size": payload_size,
        "payload": payload.decode('utf-8', 'ignore')
    }


class Protocol:
    def __init__(self, server, conn):
        self.server = server
        self.conn = conn

    @abstractmethod
    def protocol(self, message):
        pass


class RegisterRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        message_dict = unpack_message(message)
        payload = message_dict["payload"]
        username = payload[:255].strip()

        if not self.server.get_database().is_username_already_registered(username):
            accepted_register_request = self.accept_register_request(username)
            if accepted_register_request:
                user_public_key = self.receive_public_key()
                # Todo: more code here
        else:
            self.send_register_failed_reply()

    def build_accept_register_request_reply(self, uuid) -> Response:
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.REGISTERED_SUCCESSFULLY)
        payload_format = '<16s'
        # The format string '<' indicates little-endian format.
        # '16s' means a string of 16 bytes
        payload = struct.pack(payload_format, uuid.encode('utf-8'))
        reply = Response(header=reply_header, payload=payload)
        return reply

    def accept_register_request(self, username) -> bool:
        was_able_to_add_user, uuid = self.server.get_database().add_new_user_to_database(username=username)
        if not was_able_to_add_user:
            self.send_register_failed_reply()
            return False
        reply = self.build_accept_register_request_reply(uuid)
        reply.response(self.conn)
        return True

    def build_register_failed_reply(self) -> Response:
        reply_header = Header(server_version=self.server.get_version(),
                              response_code=ServerReplyCodes.REGISTRATION_FAILED)
        reply = Response(reply_header)
        return reply

    def send_register_failed_reply(self):
        reply = self.build_register_failed_reply()
        reply.response(self.conn)

    def receive_public_key(self):
        try:
            # Receive the fixed-size header (16 + 1 + 2 + 4 = 23 bytes)
            header_size = 23
            header_data = self.conn.recv(header_size)

            # Unpack the header
            header_format = '<16sBHI'  # Little-endian: 16s (client_id), B (version), H (code), I (payload_size)
            client_id, version, code, payload_size = struct.unpack(header_format, header_data)
            # TODO: check that the code we got is the right code for get public key else throw an error I guess / Return False
        except Exception as e:
            pass
        return public_key


class SendPublicKeyRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass


class ReconnectToServerRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass


class SendFileRequest(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass


class AdequateCrcValueProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass


class InadequateCrcValueProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass


class InadequateCrcValueForTheForthTimeProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message):
        pass

# TODO<not really todo>: EXAMPLE FOR FUTURE REFERENCE: #reply = client_reply_protocols[ClientReplyCodes.REGISTER_REQUEST](server).protocol(message)
