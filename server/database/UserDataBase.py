from server.database.utils import database_utils
from server.database.utils.database_utils import compute_new_aes_key
from server.utils.encryption_decryption_utils.rsa_encrtption_decryption import encrypt_aes_key_with_public_key


class User:
    def __init__(self, uuid, name, directory_path, aes_key, public_key="", ):
        self.uuid = uuid
        self.name = name
        self.public_key = public_key
        self.aes_key = aes_key
        self.directory_path = directory_path

    def get_uuid(self):
        return self.uuid

    def get_name(self):
        return self.name

    def get_public_key(self):
        return self.public_key

    def set_public_key(self, public_key):
        self.public_key = public_key

    def set_aes_key(self, aes_key):
        self.aes_key = aes_key

    def get_aes_key(self):
        return self.aes_key

    def get_directory_path(self):
        return self.directory_path


class UserDatabase:
    def __init__(self, users_folders_directory_name="users"):
        self.users = dict()
        able_to_create_directory = database_utils.make_directory(users_folders_directory_name)
        if not able_to_create_directory:
            print("Error couldn't create users_folders directory")
        else:
            self.users_folders_directory_name = users_folders_directory_name

    def add_new_user_to_database(self, username) -> tuple:
        uuid = database_utils.compute_new_uuid()
        while uuid in self.users.keys():  # ensuring that the uuid is unique to the user - normally the code in this line would never be executed
            uuid = database_utils.compute_new_uuid()
        user_directory_path = self.users_folders_directory_name + "\\" + uuid
        user_aes_key = database_utils.compute_new_aes_key()

        able_to_create_directory = database_utils.make_directory(user_directory_path)
        if not able_to_create_directory:
            return False, ""

        self.users[uuid] = User(uuid=uuid, name=username, aes_key=user_aes_key,
                                directory_path=user_directory_path)
        return True, uuid

    def is_username_already_registered(self, username):
        for user in self.users.values():
            if user.get_name() == username:
                return True
        return False

    def set_new_user_public_key(self, username, public_key):
        for user in self.users.values():
            if user.get_name() == username:
                user.set_public_key(public_key)
                aes_key = compute_new_aes_key()
                user.set_aes_key(aes_key)
                encrypted_aes_key = encrypt_aes_key_with_public_key(aes_key, public_key)
                return encrypted_aes_key
        return ""  # In case of an error

    def get_aes_key_by_uuid(self, client_id):
        for user in self.users.values():
            if user.get_uuid() == client_id:
                return user.get_aes_key()
        return ""  # In case of an error

