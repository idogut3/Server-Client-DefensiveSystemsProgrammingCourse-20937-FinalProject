import database_utils


class User:
    def __init__(self, name, public_key):
        self.name = name
        self.public_key = public_key

    def get_name(self):
        return self.name

    def get_public_key(self):
        return self.public_key


class UserDatabase:
    def __init__(self):
        self.users = dict()

    # def add_new_user_to_database(self, user) -> bool:
    #     UUID = database_utils.compute_new_uuid()
    #     # try:
    #     #     user_folder = open("users\\"+UUID, 'x+')
    #     # except OSError as error:
    #     #     print("Unexpected error couldn't create a file for the user")
    #     #     print("Error description - ", error)
    #     #     return False
    #     # self.users[UUID] = []
