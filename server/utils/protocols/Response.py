import struct


class Header:
    def __init__(self, server_version, response_code, payload_size=0):
        self.server_version = server_version
        self.response_code = response_code
        self.payload_size = payload_size

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
