#pragma once

#include "basic_func.h"
#include "headers.h"
#include "values.h"

using namespace NWTAS_Values;
using namespace basic_func;

namespace NWTAS {
	using t_TokensP = pair<string, vector<string>>;
	using t_Tokens = vector<t_TokensP>;

	string removeComments(string str) {
		bool in_comment = false;
		string output;
		for (int i = 0; i < str.size(); i++) {
			char c = str[i];
			int p = i - 1;
			if (p < 0) { p = 0; }
			if (c == comment && str[p] != '\\') {
				in_comment = !in_comment;
				continue;
			}
			if (!in_comment) { output += c; }
		}
		return output;
	}

	t_Tokens Tokens(string SCRIPT) {
		// Define tolkens and lines
		t_Tokens tolkens;
		vector<string> lines = StrSplit(SCRIPT, "\n");

		// Loop for making the tolkens
		for (string line : lines) {
			// Get the line

			// Remove comments
			line = removeUSpaces(replaceQuotes(removeComments(line)));

			// Skip the line if is empty
			if (line == "") { continue; }

			// Define tolken elements
			vector<string> lineT = StrSplit(line, ",");
			vector<string> lineT0 = StrSplit(lineT[0], " ");

			// Assemble the tolken
			string command = lineT0[0];
			vector<string> args;
			if (lineT0.size() > 1) {
				args.push_back(lineT0[1]);
				args.insert(args.end(), lineT.begin() + 1, lineT.end());
				for (int a = 0; a < args.size(); a++) { args[a] = removeUSpaces(args[a]); }
			}
			else { args = { "" }; }
			tolkens.push_back({command, args});
		}

		return tolkens;
	}

	// convert tokens to binary
	vector<unsigned char> convertTtB(t_Tokens tokens, string eversion, int bytes){
		vector<unsigned char> output;
		
		// asm/bin version
		vector<string> eversion_ = StrSplit(eversion, "-");
		vector<int> ei = _versions.at(eversion_[0]);

		int com_ei = ei[0];
		int nf_ei = ei[1];
		int kw_ei = ei[2];
		bool bzm = eversion_[1] == "1";
		

		for (t_TokensP token : tokens) {
			string last_left_arg;
			string com = token.first;

			// pass the compiler op
			if (com[0] == compilerOp) { continue; }
			
			vector<unsigned char> com__ = commands[com_ei].at(com);
			vector<unsigned char> temp;
			if (bzm) {
				temp = {};
				while (com__.size() != bytes) { temp.push_back(0x00); }
				com__.insert(com__.begin(), temp.begin(), temp.end());
			}
			output.insert(output.end(),com__.begin(), com__.end());
			
			for (string arg : token.second) {
				if (arg.empty()) { continue; }
				auto nf = name_flags[nf_ei].find(arg[0]);
				vector<string> sepv;
				int sep_ = countChars(arg, ArgValueSep);
				if (nf != name_flags[nf_ei].end()) {
					output.push_back(name_flags[nf_ei].at(arg[0]));
					arg.erase(0, 1);
				}
				if (sep_ != 0) {
					sepv = StrSplitAtLeft(arg, string(1, ArgValueSep));
					arg = sepv[0];
					last_left_arg = sepv[0];
				}
				if (countChars(arg, '\'') == 0) {
					try {
						int value = stoi(arg);
						vector<unsigned char> int_bytes = IntToFullSetBytes(value, bytes);
						output.insert(output.end(), int_bytes.begin(), int_bytes.end());
					} catch (...) {
						try {
							vector<unsigned char> kw__ = keywords[kw_ei].at(arg);
							if (bzm) {
								temp = {};
								while (com__.size() != bytes) { temp.push_back(0x00); }
								com__.insert(com__.begin(), temp.begin(), temp.end());
							}
							output.insert(output.end(), kw__.begin(), kw__.end());

							arg = "";
						}
						catch (...) {}
						if (!sepv.empty()) { arg = sepv[1]; }
					}
				}
				if (!arg.empty()) {
					auto nf = name_flags[nf_ei].find(arg[0]);
					if (nf != name_flags[nf_ei].end()) {
						output.push_back(name_flags[nf_ei].at(arg[0]));
						arg.erase(0, 1);
						arg = StrReplace(arg, "'", "");
						copy(arg.begin(), arg.end(), back_inserter(output));
						output.push_back(0xff);
					}
					else {
						if (int_size.count(last_left_arg) > 0) {
							int value = stoi(arg);
							vector<unsigned char> int_bytes = IntToFullSetBytes(value, int_size.at(last_left_arg));
							output.insert(output.end(), int_bytes.begin(), int_bytes.end());
						}
						else if (last_left_arg == "hex") {
							vector<unsigned char> hexStrOut = hexStrToUnsChar(arg);
							vector<unsigned char> size_of_hex = IntToFullSetBytes(hexStrOut.size(), 2);
							output.insert(output.end(), size_of_hex.begin(), size_of_hex.end());
							output.insert(output.end(), hexStrOut.begin(), hexStrOut.end());
						}
						else if (last_left_arg == "int") {
							int value = stoi(arg);
							vector<unsigned char> int_bytes = IntToFullSetBytes(value, bytes);
							output.insert(output.end(), int_bytes.begin(), int_bytes.end());
						}
						else if (count(lists_types.begin(), lists_types.end(), last_left_arg) > 0) {
							vector<string> split_arg = StrSplit(last_left_arg, "-");
							last_left_arg = split_arg[0];
							int value = stoi(arg);
							vector<unsigned char> int_bytes = IntToFullSetBytes(value, bytes);
							output.insert(output.end(), int_bytes.begin(), int_bytes.end());
						}
						else {
							arg = StrReplace(arg, "'", "");
							copy(arg.begin(), arg.end(), back_inserter(output));
							output.push_back(0xff);
						}
					}
				}
			}
		}

		return output;
	}

	vector<unsigned char> BUILD(string SRC_Path) {
		vector<unsigned char> output;

		string next_file;
		string _eversion = "1.0.0";
		int bytes = 4; // 32 bits
		/*
		Due to using bytes instead of bytes for the "bytes" int, the compiler outputs a lot of 0x00.
		*/
		bool lock_n = false;
		bool global_n = false;

		// read the files
		map<string, t_Tokens> files_tokens;
		vector<string> files = listFiles(SRC_Path);
		for (string file_p : files) { if (file_p.size() >= 6 && file_p.substr(file_p.size() - 6) == ".nwtas") {
			t_Tokens tokens = Tokens(readFile(file_p));
			string f_name = StrReplace(file_p, SRC_Path, "");
			f_name = f_name.substr(0, f_name.length() - 6);
			if (f_name.empty()) { f_name = "."; }
			for (t_TokensP token : tokens) { if (token.first == ".main") { next_file = f_name; } }
			files_tokens.insert({ f_name, tokens });
		}}

		if (next_file == "") { next_file = files_tokens.begin()->first; }

		while (true)
		{
			if (next_file == "") { break; }

			string __eversion = _eversion;
			int _bytes = bytes;

			t_Tokens tokens = files_tokens.at(next_file);
			
			string start = "";
			string include_code = "";
			string type_code = "";
			bool skip = false;
			bool global_code = global_n;
			bool lock_ = lock_n;
			next_file = "";

			// seg
			vector<unsigned char> ds = { 0x00,0x00,0x00 };
			vector<unsigned char> cs = { 0x00,0x00,0x00 };
			vector<unsigned char> ss = { 0x00,0x00,0x00 };

			// set compiler values
			for (t_TokensP token : tokens) {
				string arg = token.second[0];
				try { switch (compilerOpMap.at(token.first)) {
				case 0: bytes = stoi(arg); break;
				case 1: next_file = arg; break;
				case 2: skip = true; break;
				case 3: start = arg; break;
				case 4: global_code = true; break;
				case 5: _eversion = arg; break;
				case 6: type_code = arg; break;
				case 7: lock_ = true; break;
				case 8: ds = IntToFullSetBytes(stoi(arg), 3); break;
				case 9: cs = IntToFullSetBytes(stoi(arg), 3); break;
				case 10: ss = IntToFullSetBytes(stoi(arg), 3); break;
				case 11:
					lock_n = true;
					lock_ = true;
					break;
				case 12: lock_ = false; break;
				case 13:
					lock_n = false;
					lock_ = false;
					break;
				case 14:
					global_n = true;
					global_code = true;
					break;
				case 15: global_code = false; break;
				case 16:
					global_n = false;
					global_code = false;
					break;
				}}
				catch (...) {}
			}

			if (skip) { continue; }

			// insert flags
			if (__eversion == _eversion) { output.push_back(0x00); }
			else{
				copy(_eversion.begin(), _eversion.end(), back_inserter(output));
				output.push_back(0xff);
			}
			if(_bytes == bytes){ output.push_back(0x00); }
			else {
				vector<unsigned char> bytes_out = IntToFullByte(bytes);
				output.insert(output.end(), bytes_out.begin(), bytes_out.end());
			}
			if (global_code) { output.push_back(0xff); }
			else { output.push_back(0x00); }
			if (lock_) { output.push_back(0xff); }
			else { output.push_back(0x00); }

			// insert segments size
			output.insert(output.end(), ds.begin(), ds.end());
			output.insert(output.end(), cs.begin(), cs.end());
			output.insert(output.end(), ss.begin(), ss.end());
			
			vector<unsigned char> bin_output = convertTtB(tokens, _eversion, bytes);
			vector<unsigned char> code_size = IntToFullSetBytes(static_cast<int>(bin_output.size()), bytes);
			
			output.insert(output.end(), code_size.begin(), code_size.end());
			output.insert(output.end(), bin_output.begin(), bin_output.end());
		}

		return output;
	}
}