from abc import abstractmethod

from server.utils.protocols.ClientRequestProtocols.Reply import Reply
from server.utils.protocols.codes.client_reply_codes_enum import ClientReplyCodes


class Protocol:
    def __init__(self, message):
        self.message = message

    @abstractmethod
    def protocol(self) -> Reply:
        pass


class RegisterRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class SendPublicKeyRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class ReconnectToServerRequestProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class SendFileRequest(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class AdequateCrcValueProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class InadequateCrcValueProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


class InadequateCrcValueForTheForthTimeProtocol(Protocol):
    def __init__(self, message):
        super().__init__(message)

    def protocol(self) -> Reply:
        pass


client_reply_protocols = {
    ClientReplyCodes.REGISTER_REQUEST: RegisterRequestProtocol,
    ClientReplyCodes.SEND_PUBLIC_KEY_REQUEST: SendPublicKeyRequestProtocol,
    ClientReplyCodes.RECONNECT_TO_SERVER_REQUEST: ReconnectToServerRequestProtocol,
    ClientReplyCodes.SEND_FILE_REQUEST: SendFileRequest,
    ClientReplyCodes.ADEQUATE_CRC_VALUE: AdequateCrcValueProtocol,
    ClientReplyCodes.INADEQUATE_CRC_VALUE: InadequateCrcValueProtocol,
    ClientReplyCodes.INADEQUATE_CRC_VALUE_FOR_THE_FORTH_TIME: InadequateCrcValueForTheForthTimeProtocol
}

# TODO<not really todo>: EXAMPLE FOR FUTURE REFERENCE: #reply = client_reply_protocols[ClientReplyCodes.REGISTER_REQUEST]("message RARARARARARAR").protocol()
