import struct
from abc import abstractmethod

from server.utils.protocols.ClientRequestProtocols.Reply import Reply
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes


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
    def protocol(self, message) -> Reply:
        pass


class RegisterRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        message_dict = unpack_message(message)
        payload = message_dict["payload"]
        username = payload[:255].strip()

        if not self.server.get_database().is_username_already_registered(username):
            pass
            # server.get_database().
        else:
            pass


class SendPublicKeyRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass


class ReconnectToServerRequestProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass


class SendFileRequest(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass


class AdequateCrcValueProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass


class InadequateCrcValueProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass


class InadequateCrcValueForTheForthTimeProtocol(Protocol):
    def __init__(self, message, conn):
        super().__init__(message, conn)

    def protocol(self, message) -> Reply:
        pass

# TODO<not really todo>: EXAMPLE FOR FUTURE REFERENCE: #reply = client_reply_protocols[ClientReplyCodes.REGISTER_REQUEST](server).protocol(message)
