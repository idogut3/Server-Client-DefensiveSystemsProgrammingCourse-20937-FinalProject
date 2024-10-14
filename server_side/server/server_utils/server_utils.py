def get_header(conn):
    header_length = 23
    return conn.recv(header_length)  # Will receive the first 24 bytes meaning the header


def get_full_message(conn, header):
    payload_size = int.from_bytes(header[19:23], 'little')  # 4 bytes for Payload size (little-endian)
    message = header + conn.recv(payload_size)
    return message


# def unpack_message(message):
#     # Extract fields according to the given structure
#     client_id_in_bytes = message[:16]  # 16 bytes for Client ID
#     version_in_bytes = message[16:17]  # 1 byte for Version
#     code_in_bytes = message[17:19]  # 2 bytes for Code
#     payload = message[23:]
#     return {
#         "client_id": client_id_in_bytes.decode('utf-8', 'ignore'),
#         "version": int.from_bytes(version_in_bytes, 'little'),
#         "code": int.from_bytes(code_in_bytes, 'little'),
#         "payload": payload
#     }
