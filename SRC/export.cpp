#include "pch.h"
#include "export.h"
#include "NWTAS.h"

extern "C" NWTAS_API const char* removeComments(const char* str) {
    static std::string result;
    result = NWTAS::removeComments(std::string(str));
    return result.c_str();
}

extern "C" NWTAS_API const char* Tokens(const char* script) {
    static std::string result;
    std::vector<std::pair<std::string, std::vector<std::string>>> tokens = NWTAS::Tokens(script);

    std::ostringstream oss;

    oss << "[";
    for (const auto& pair : tokens) {
        oss << "[\"" << pair.first << "\", [";
        for (const auto& str : pair.second) {
            oss << "\"" << str << "\", ";
        }
        if (!pair.second.empty()) {
            oss.seekp(-2, std::ios_base::cur); // Remove the trailing comma and space
        }
        oss << "]], ";
    }
    if (!tokens.empty()) {
        oss.seekp(-2, std::ios_base::cur); // Remove the trailing comma and space
    }
    oss << "]";

    result = oss.str();
    return result.c_str();
}

extern "C" NWTAS_API DataBuffer Build(const char* src) {
    std::vector<unsigned char> result = NWTAS::BUILD(std::string(src));
    DataBuffer buffer;
    buffer.size = result.size();
    buffer.data = new unsigned char[buffer.size];
    std::copy(result.begin(), result.end(), buffer.data);
    return buffer;
}
