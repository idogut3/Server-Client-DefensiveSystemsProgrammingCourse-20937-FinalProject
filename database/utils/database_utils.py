import uuid
import os
from Crypto.Random import get_random_bytes
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad


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

def decrypt_file_with_aes_key(file, aes_key, output_file_destination_path):
    AES_BLOCK_SIZE = 16
    cipher = AES.new(aes_key, AES.MODE_CBC) # Create a new AES cipher object

    # Read the initialization vector (IV) from the input file
    with open(file, 'rb') as f:
        iv = f.read(AES_BLOCK_SIZE)
        cipher = AES.new(aes_key, AES.MODE_CBC, iv)

        # Read the encrypted data
        encrypted_data = f.read()

    # Decrypt the data
    decrypted_data = unpad(cipher.decrypt(encrypted_data), AES.block_size)

    # Write the decrypted data to the t file
    with open(output_file_destination_path, 'wb') as f:
        f.write(decrypted_data)

