#pragma once

#ifdef NWTAS_EXPORTS
#define NWTAS_API __declspec(dllexport)
#else
#define NWTAS_API __declspec(dllimport)
#endif

#include "headers.h"

// Define DataBuffer struct
struct DataBuffer {
    unsigned char* data;
    int size;
};

#include "pch.h"
#include "export.h"
#include "NWTAS.h"

extern "C" NWTAS_API const char* removeComments(const char* str) {
    static string result;
    result = NWTAS::removeComments(string(str));
    return result.c_str();
}

extern "C" NWTAS_API const char* Tokens(const char* script) {
    static string result;
    vector<pair<string, vector<string>>> tokens = NWTAS::Tokens(script);

    ostringstream oss;

    oss << "[";
    for (const auto& pair : tokens) {
        oss << "[\"" << pair.first << "\", [";
        for (const auto& str : pair.second) {
            oss << "\"" << str << "\", ";
        }
        if (!pair.second.empty()) {
            oss.seekp(-2, ios_base::cur); // Remove the trailing comma and space
        }
        oss << "]], ";
    }
    if (!tokens.empty()) {
        oss.seekp(-2, ios_base::cur); // Remove the trailing comma and space
    }
    oss << "]";

    result = oss.str();
    return result.c_str();
}

extern "C" NWTAS_API DataBuffer Build(const char* src) {
    try {
        vector<unsigned char> result = NWTAS::BUILD(string(src));
        DataBuffer buffer;
        buffer.size = result.size();
        buffer.data = new unsigned char[buffer.size];
        copy(result.begin(), result.end(), buffer.data);
        return buffer;
    }
    catch (const exception& e) {
        cerr << "Exception in Build: " << e.what() << endl;
        return { nullptr, 0 };
    }
    catch (...) {
        cerr << "Unknown exception in Build." << endl;
        return { nullptr, 0 };
    }
}
