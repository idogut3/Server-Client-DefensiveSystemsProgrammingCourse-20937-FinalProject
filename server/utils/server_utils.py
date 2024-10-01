from server.database.utils import checksum


def calculate_checksum_value(file):
    return checksum.readfile(file)


