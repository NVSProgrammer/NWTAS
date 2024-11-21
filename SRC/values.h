#pragma once
#include "headers.h"

namespace NWTAS_Values {
    // flags
    const char compilerOp = '.';
    const char comment = ';';
    const char ArgValueSep = ':';

    const map<string, int> compilerOpMap = {
        {".bytes", 0},
        {".next", 1},
        {".skip", 2},
        {".start", 3},
        {".global", 4},
        {".eversion", 5},
        {".type", 6},
        {".lock", 7},
        {".data", 8},
        {".code", 9},
        {".stack", 10},
        {".lock-all", 11},
        {".unlock", 12},
        {".unlock-all", 13},
        {".global-all", 14},
        {".local", 15},
        {".local-all", 16}
    };

    // version map
    const map<string, vector<int>> _versions = {
        // v.com.nf.kw.bzm
        // emulator
        {"1.0.0", {0, 0, 0, 0}}
    };

    // command
    const vector<map<string, vector<unsigned char>>> commands = {
        {
            {"NOP", {0x00}},
            {"Reboot", {0x01}},
            {"Shutdown", {0xFE}},
            {"Set", {0xCE}},
            {"New", {0xAE}},
            {"And", {0xAA}},
            {"Or", {0xA0}},
            {"XOR", {0xA9}},
            {"Not", {0x09}},
            {"Random", {0xD3}},
            {"Add", {0xAD}},
            {"Sub", {0xCD}},
            {"Mul", {0xA7}},
            {"Div", {0xDA}},
            {"Mod", {0xA8}},
            {"Shift", {0xCF } },
            {"Draw", {0xD0}},
            {"Get", {0xD9}},
            {"Use", {0xE0}},
            {"End", {0xED}},
            {"Refresh", {0xAC}},
            {"Pause", {0xBA}},
            {"Goto", {0x02}},
            {"Size", {0xCA}},
            {"Run", {0x0A}},
            {"ROM", {0x0F}},
            {"RAM", {0x1F}},
            {"Send", {0x0C}},
            {"Math", {0x0B}},
            {"SL", {0xC7}},
            {"SR", {0xC8}},
            {"Move", {0xC9}},
            {"Copy", {0xD0}},
            {"SPoB", {0xFB}}
        }
    };

    // name flags
    const vector<map<char, unsigned char>> name_flags = {
        {
            {'~', 0x00}, // system
            {'$', 0x01}, // device
            {'#', 0x02}, // value
            {'@', 0x03}, // function
            {'*', 0x04}  // different
        }
    };

    // keywords
    const vector < map < string, vector<unsigned char>>> keywords = {
        {
            {"vmode", {0xEA}},
            {"dfcolor", {0xDC}},
            {"dbcolor", {0xDB}},
            {"text", {0xE0}},
            {"img", {0xAA}},
            {"cursorp", {0xCD}},
            {"hex", {0x00}},
            {"newline", {0xE7}},
            {"int", {0x01}},
            {"str", {0x02}},
            {"hex-list", {0x03}},
            {"int-list", {0x04}},
            {"srt-list", {0x05}},
            {"map-list", {0x06} },
            {"int4", {0x07}},
            {"int8", {0x08}},
            {"int4-list", {0x09}},
            {"int8-list", {0x0A}},
            {"bin", {0xFF}},
            {"read", {0x0F} },
            {"write", {0x1F}},
            {"ins", {0xCC}},
            {"req", {0xCE}},
            {"data", {0xCD}},
            {"file", {0xFE} }
        }
    };

    const map<string, int> int_size = {
        {"int4", 4},
        {"int8", 8}
    };
    
    const vector<string> lists_types = { "int32_list", "int64-list", "hex-list", "str-list", "int-list" };
}

namespace TestWV {
    string RC = "hello p ;753c; 7534;9";
}

namespace TestCV {
    string RC = "hello p 7534";
}
