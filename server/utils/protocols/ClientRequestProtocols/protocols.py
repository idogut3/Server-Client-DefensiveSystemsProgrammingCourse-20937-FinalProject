from abc import abstractmethod

from server.utils.protocols.ClientRequestProtocols.Reply import Reply
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes


class Protocol:
    def __init__(self, server):
        self.server = server

    @abstractmethod
    def protocol(self, message) -> Reply:
        pass

    @abstractmethod
    def unpack_message(self, message):
        pass


class RegisterRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        if not self.server.get_database().is_username_already_registered():
            pass
            # server.get_database().
        else:
            pass


class SendPublicKeyRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass


class ReconnectToServerRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass


class SendFileRequest(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass


class AdequateCrcValueProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass


class InadequateCrcValueProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass


class InadequateCrcValueForTheForthTimeProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def unpack_message(self, message):
        pass

    def protocol(self, message) -> Reply:
        pass

# TODO<not really todo>: EXAMPLE FOR FUTURE REFERENCE: #reply = client_reply_protocols[ClientReplyCodes.REGISTER_REQUEST](server).protocol(message)
