import socket

from database.UserDataBase import UserDatabase


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

    def accept_register_request(self, username):
        if not self.database.is_username_already_registered(username):
            pass
            # user_public_key = get_user_public_key
            # self.database.add_new_user_to_database(username, user_public_key)
        else:
            pass
            # respond_with_error()


    def run(self):
        self.check_existing_database()
        while True:
            pass



