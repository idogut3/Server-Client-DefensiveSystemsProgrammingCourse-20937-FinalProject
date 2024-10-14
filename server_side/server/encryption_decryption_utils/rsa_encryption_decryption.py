from Crypto.Cipher import PKCS1_OAEP, AES
from Crypto.Util.Padding import unpad

def encrypt_aes_key_with_public_key(aes_key, public_key):
    rsa_cipher = PKCS1_OAEP.new(public_key)
    encrypted_key = rsa_cipher.encrypt(aes_key)
    return encrypted_key


def decrypt_file_with_aes_key(encrypted_file, aes_key):
    iv = bytes(16)  # Reset IV to zeros for decryption
    cipher = AES.new(aes_key, AES.MODE_CBC, iv)
    decrypted_data = unpad(cipher.decrypt(encrypted_file), AES.block_size)
    return decrypted_data
