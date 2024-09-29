import socket

from server.database.UserDataBase import UserDatabase
from server.utils.protocols.ClientRequestProtocols.protocols import *
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes


class Server:
    def __init__(self, host):
        DEFAULT_PORT = "1256"
        port = DEFAULT_PORT
        try:
            port_info_file = open("port.info", "r")
            # assuming the port_info_file is correct in its form
            port = port_info_file.read()
        except OSError as error:
            print("Error: Error accessing port.info file")
            print(error)
            print("using default port: ", DEFAULT_PORT)
        finally:
            self.port = port
            self.host = host
            self.ADDR = (self.host, self.port)
            self.database = UserDatabase()
            self.client_reply_protocols = {
                ClientReplyCodes.REGISTER_REQUEST: RegisterRequestProtocol(self),
                ClientReplyCodes.SEND_PUBLIC_KEY_REQUEST: SendPublicKeyRequestProtocol(self),
                ClientReplyCodes.RECONNECT_TO_SERVER_REQUEST: ReconnectToServerRequestProtocol(self),
                ClientReplyCodes.SEND_FILE_REQUEST: SendFileRequest(self),
                ClientReplyCodes.ADEQUATE_CRC_VALUE: AdequateCrcValueProtocol(self),
                ClientReplyCodes.INADEQUATE_CRC_VALUE: InadequateCrcValueProtocol(self),
                ClientReplyCodes.INADEQUATE_CRC_VALUE_FOR_THE_FORTH_TIME: InadequateCrcValueForTheForthTimeProtocol(
                    self)
            }

    def get_database(self):
        return self.database

    def check_existing_database(self):  # Question 3
        pass

    def handle_client(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind(self.ADDR)
            s.listen()
            conn, addr = s.accept()
            with conn:
                print("connected by:", addr)
                self.handle_connection(conn, addr)
                # More code maybe here ...

    def handle_connection(self, conn, addr):
        pass

    def run(self):
        self.check_existing_database()
        while True:
            pass
