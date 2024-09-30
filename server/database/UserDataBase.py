from server.database.utils import database_utils


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

    def get_aes_key_by_username(self, username) -> tuple:
        if not self.is_username_already_registered:
            return False, ""
        for user in self.users.values():
            if user.get_name() == username:
                return True, user.get_aes_key()
