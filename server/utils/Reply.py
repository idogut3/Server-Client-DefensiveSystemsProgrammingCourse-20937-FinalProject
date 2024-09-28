import struct

class Reply:
    def __init__(self, server_version, reply_code, payload=""):
        self.server_version = server_version
        self.reply_code = reply_code
        self.payload = payload
        self.payload_size = len(payload)

    def pack_message_header(self):
        #struct format (That we pack the message header with):
        # '<' represents we are using a Little-Endian Format
        # 'B' represents an unsigned char (1 byte), 'H' represents unsigned short (2 bytes)
        # 'H' represents Unsigned short (2 bytes)
        # 'I' represents an unsigned int (4 bytes)
        return struct.pack('<B H I', self.server_version, self.reply_code, self.payload_size)

    def response(self, conn):
        message_header = self.pack_message_header()
        message = pack_message_header_with_payload(message_header, self.payload)
        conn.sendall(message)


def pack_message_header_with_payload(message_header, message_payload):
    return message_header + message_payload