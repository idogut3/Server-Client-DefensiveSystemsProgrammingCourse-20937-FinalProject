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


def encrypt_aes_key_with_public_key(aes_key, public_key):
    rsa_cipher = PKCS1_OAEP.new(public_key)
    encrypted_key = rsa_cipher.encrypt(aes_key)
    return encrypted_key


def decrypt_file_with_aes_key(encrypted_file, aes_key):
    iv = bytes(16)  # Reset IV to zeros for decryption
    cipher = AES.new(aes_key, AES.MODE_CBC, iv)
    decrypted_data = unpad(cipher.decrypt(encrypted_file), AES.block_size)
    return decrypted_data
