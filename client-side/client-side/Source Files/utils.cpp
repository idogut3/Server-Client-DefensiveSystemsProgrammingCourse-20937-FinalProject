#include "client-side\Header Files\utils.hpp"

UUID getUUIDFromString(string client_id)
{
    UUIDGenerator uuid_generator;  // Use the alias for boost::uuids::string_generator
    return uuid_generator(client_id);  // Generate and return a UUID from the string
}
