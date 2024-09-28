class Reply:
    def __init__(self, server_version, reply_code, payload = ""):
        self.server_version = server_version
        self.reply_code = reply_code
        self.payload = payload
        self.payload_size = len(payload)


    def response(self):
        pass

