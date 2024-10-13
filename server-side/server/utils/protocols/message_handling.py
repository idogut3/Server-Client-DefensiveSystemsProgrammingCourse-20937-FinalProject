import struct
from server.utils.protocols.Request import ClientMessageHeader


def unpack_send_file_payload(payload):
    # < is for little-endian format
    # I: Unsigned 4-byte integer (Content Size, Original File Size)
    # H: Unsigned 2-byte integer (Packet Number, Total Packets)
    # 255s: File Name (255 bytes)
    message_format = '<I I H H'

    # Unpack first 12 bytes
    encrypted_content_size, original_file_size, packet_number, total_packets = struct.unpack(message_format,
                                                                                             payload[:12])
    # file_name is 255 bytes 255 + previous 12 bytes = 267
    file_name = payload[12:267].decode('utf-8', 'ignore').rstrip('\x00')
    encrypted_message_content = payload[267:].decode('utf-8', 'ignore')

    return encrypted_content_size, original_file_size, packet_number, total_packets, file_name, encrypted_message_content


# def extract_relevant_values_from_crc_conformation_message_as_dict(client_crc_conformation_message):
#     message_dict = unpack_message(client_crc_conformation_message)
#     conformation_reply_client_id = message_dict["client_id"]
#     conformation_reply_code = message_dict["code"]
#     conformation_reply_payload = message_dict["payload"]
#     conformation_reply_file_name = conformation_reply_payload.decode("utf-8")
#     return {
#         "conformation_reply_client_id": conformation_reply_client_id,
#         "conformation_reply_code": conformation_reply_code,
#         "conformation_reply_file_name": conformation_reply_file_name
#     }


def receive_request_header(conn):
    client_id = conn.recv(16)
    client_version = conn.recv(1)
    code = conn.recv(4)
    payload_size = conn.recv(4)
    return ClientMessageHeader(client_id, client_version, code, payload_size)


def receive_register_request_protocol_payload(conn):
    user_name_bytes_size = 255
    payload = conn.recv(user_name_bytes_size)
    return payload
