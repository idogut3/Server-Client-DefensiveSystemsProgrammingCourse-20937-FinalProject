import uuid
import os
from Crypto.Random import get_random_bytes
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad, pad

from server.utils.server_utils import calculate_checksum_value


def compute_new_uuid() -> str:
    return str(uuid.uuid4())


def make_directory(directory_name) -> bool:
    try:  # Was able to create users directory
        os.makedirs(directory_name)
        return True
    except OSError:  # Error couldn't create directory
        return False


def compute_new_aes_key(key_size=256):
    return get_random_bytes(key_size // 8)


def save_file_in_path(path, file):
    pass