#pragma once
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

// Functions
namespace basic_func {
    using namespace std;
    namespace fs = filesystem;
    vector<string> StrSplit( string str, string Sep) {
    	vector<string> output;
    	size_t start = 0;
    	size_t end = str.find(Sep);
    	while (end != string::npos) {
    		output.push_back(str.substr(start, end - start));
    		start = end + Sep.length();
    		end = str.find(Sep, start);
    	}
    	output.push_back(str.substr(start));
    	return output;
    }
    string StrReplace(string str, string toReplace, string Replacement) {
    	size_t pos = str.find(toReplace);
    	while (pos != string::npos) {
    		str.replace(pos, toReplace.length(), Replacement);
    		pos = str.find(toReplace, pos + Replacement.length());
    	}
    	return str;
    }
    
    string readFile(const string filePath) {
    	ifstream file(filePath);
    	if (!file.is_open()) {
    		cerr << "Unable to open file" << endl;
    		return "";
    	}
    
    	ostringstream ss;
    	ss << file.rdbuf();
    	return ss.str();
    }
    
    string removeUSpaces(const string& str) {
        stringstream ss(str);
        string word, result;
        bool insideQuotes = false;
        bool escapeNext = false;
    
        for (char ch : str) {
            if (escapeNext) {
                result += ch;
                escapeNext = false;
                continue;
            }
            if (ch == '\\') {
                escapeNext = true;
                result += ch;
                continue;
            }
            if (ch == '"' or ch == '\'') {
                insideQuotes = !insideQuotes;
            }
            if (insideQuotes || !isspace(ch)) {
                result += ch;
            }
            else if (!result.empty() && result.back() != ' ') {
                result += ' ';
            }
        }
    
        if (!result.empty() && result.front() == ' ') result.erase(result.begin());
        if (!result.empty() && result.back() == ' ') result.erase(result.end() - 1);
    
        return result;
    }
    
    string replaceQuotes(const string& str) {
        string result;
        bool escape = false;
        for (char c : str) {
            if (c == '\\' && !escape) {
                escape = true;
                result += c;
            }
            else if (c == '"' && !escape) {
                result += '\'';
            }
            else {
                result += c;
                escape = false;
            }
        }
        return result;
    }
    
    int countChars(const string str, char ch) {
        int count = 0;
        for (char c : str) {
            if (c == ch) {
                count++;
            }
        }
        return count;
    }
    
    vector<string> listFiles(const string& path) {
        vector<string> files;
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry.status())) {
                files.push_back(entry.path().string());
            }
        }
        return files;
    }
    
    vector<unsigned char> IntToFullByte(int value) {
        vector<unsigned char> bytes;
        while (value > 0) {
            bytes.insert(bytes.begin(), value & 0xFF);
            value >>= 8;
        }
        return bytes;
    }
    
    vector<unsigned char> IntToFullSetBytes(int value, int len_) {
        vector<unsigned char> output = IntToFullByte(value);
        while (output.size() != len_) { output.push_back(0x00); }
        return output;
    }
    
    vector<unsigned char> hexStrToUnsChar(const string& hex) {
        vector<unsigned char> result;
        for (size_t i = 0; i < hex.length(); i += 2) {
            string byteString = hex.substr(i, 2);
            unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
            result.push_back(byte);
        }
        return result;
    }
    
    vector<string> StrSplitAtLeft(const string& str, const string& Sep) {
        vector<string> output;
        size_t end = str.find(Sep);
    
        if (end != string::npos) {
            output.push_back(str.substr(0, end));
            output.push_back(str.substr(end + Sep.length()));
        }
        else {
            output.push_back(str);
        }
    
        return output;
    }
    
}