from database.utils import checksum


def calculate_checksum_value(file):
    return checksum.readfile(file)


def is_checksum_correct(user_checksum, file) -> bool:
    return user_checksum == calculate_checksum_value(file)


