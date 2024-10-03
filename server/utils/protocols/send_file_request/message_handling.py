from server.utils.server_utils import unpack_message


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


def extract_relevant_values_from_message(message):
    message_dict = unpack_message(message)
    client_id = message_dict["client_id"]
    payload = message_dict["payload"]
    encrypted_content_size, original_file_size, packet_number, total_packets, file_name, encrypted_message_content = \
        unpack_send_file_payload(payload)
    return client_id, encrypted_content_size, original_file_size, packet_number, total_packets, file_name, encrypted_message_content

