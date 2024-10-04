class ClientMessageHeader:
    def __init__(self, client_id, client_version, code, payload_size):
        self.client_id = client_id
        self.client_version = client_version
        self.code = code
        self.payload_size = payload_size

    def get_payload_size(self):
        return self.payload_size


class Request:
    def __init__(self, header, payload=""):
        self.header = header
        self.payload = payload

    def pack_message_header_with_payload(self):
        return self.header.pack_header() + self.payload
