from ....database.database_utils.checksum import readfile


def calculate_checksum_value(file):
    return readfile(file)
