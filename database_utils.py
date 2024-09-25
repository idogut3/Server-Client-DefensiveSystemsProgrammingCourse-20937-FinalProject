import uuid
import os


def compute_new_uuid() -> str:
    return str(uuid.uuid4())


def make_directory(directory_name) -> bool:
    try:  # Was able to create users directory
        os.makedirs(directory_name)
        return True
    except OSError:  # Error couldn't create directory
        return False


def compute_aes_key(public_key):
    pass
