// dynamic_encrypt.hpp : Defines the entry point for the console application.
#ifndef _dynamic_encrypt_hpp_
#define _dynamic_encrypt_hpp_

#include <string>
#include <vector>

/*namespace svticket*/
//{

bool is_num(std::string str);

bool is_unreserved_char(const char c);

// to hex
std::string to_hex(std::string const& s);

// escape string to %hex
std::string escape_string(const std::string& s);

/**
 *	hex to bin
 */ 
std::string hex2bin(std::string stx);

/**
 *	bin216
 */
std::string bin216(std::string& str);

/**
 *	encode32
 */
std::string encode32(std::string& input);

/**
 *	Base32.encrypt
 */
// base32.longArrayToString
std::string longArrayToString(std::vector<long> data);
// base32.stringToLongArray
std::vector<long> stringToLongArray(const char* str, bool includeLength);

// base32.encrypt
std::string base32_encrypt(std::string val, std::string key);

/**
 *	external...
 */
// ×Ö·û´®´óÐ¡Ð´×ª»»
void string_to_lower(std::string &str);
void string_to_upper(std::string &str);

// ×Ö·ûÌæ»»
std::string& string_replace(std::string& str,const std::string& old_value, const std::string& new_value);

// ansi Óë unicode ×Ö·û±àÂë×ª»»
wchar_t* ansi_to_unicode(const char *szAnsi);

char * unicode_to_ansi(const wchar_t* szUnicode);

// unicode×Ö·û<\u0000> ±àÂë/½âÂë
std::string unicode_encode(std::string& data);
std::string unicode_decode(std::string stx);

//} // @end namespace svticket.

#endif

// unit_test.
//#include <iostream>
//int main(int argc, char* argv[])
//{
//    std::cout << encode32(bin216(base32_encrypt("1111", "NDQzNDg4"))) << std::endl;
//
//    return 0;
//}
