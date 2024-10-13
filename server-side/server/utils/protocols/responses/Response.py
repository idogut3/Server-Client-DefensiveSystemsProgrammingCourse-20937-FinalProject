import struct

from server.utils.protocols.codes.server_reply_codes_enum import ServerReplyCodes


class ServerMessageHeader:
    def __init__(self, server_version, response_code, payload_size=0):
        self.server_version = server_version
        self.response_code = response_code
        self.payload_size = payload_size

    def get_payload_size(self):
        return self.payload_size

    def pack_header(self):
        # struct format (That we pack the message header with):
        # '<' represents we are using a Little-Endian Format
        # 'B' represents an unsigned char (1 byte), 'H' represents unsigned short (2 bytes)
        # 'H' represents Unsigned short (2 bytes)
        # 'I' represents an unsigned int (4 bytes)
        return struct.pack('<B H I', self.server_version, self.response_code, self.payload_size)


class Response:
    def __init__(self, header, payload=""):
        self.header = header
        self.payload = payload

    def pack_message_header_with_payload(self):
        return self.header.pack_header() + self.payload

    def response(self, conn):
        message = self.pack_message_header_with_payload()
        conn.sendall(message)


def send_general_server_error(conn, server_version):
    reply = build_send_general_server_error_response(server_version=server_version)
    reply.response(conn)


def build_send_general_server_error_response(server_version) -> Response:
    header = ServerMessageHeader(server_version=server_version, response_code=ServerReplyCodes.GENERAL_SERVER_ERROR)
    reply = Response(header)
    return reply
