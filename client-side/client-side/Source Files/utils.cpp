#include "client-side\Header Files\utils.hpp"

UUID getUUIDFromString(string client_id)
{
    UUIDGenerator uuid_generator;  // Use the alias for boost::uuids::string_generator
    return uuid_generator(client_id);  // Generate and return a UUID from the string
}

bool is_integer(const std::string& num) {
    if (num.empty()) {
        return false; // Empty string is not a valid integer
    }

    std::size_t start = 0;

    // Check for a leading sign ('+' or '-')
    if (num[0] == '+' || num[0] == '-') {
        if (num.length() == 1) {
            return false;  // A single '+' or '-' is not a valid integer
        }
        start = 1; // Start checking digits after the sign
    }

    // Check if all remaining characters are digits
    for (std::size_t i = start; i < num.length(); ++i) {
        if (!std::isdigit(num[i])) {
            return false;  // Non-digit character found none 0-9 char
        }
    }

    return true;  // All characters are digits (or valid sign)
}