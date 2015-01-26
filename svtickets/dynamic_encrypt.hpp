// dynamic_encrypt.hpp : Defines the entry point for the console application.
#ifndef _dynamic_encrypt_hpp_
#define _dynamic_encrypt_hpp_

#include <string>
#include <vector>
#include <math.h>

static const char hex_chars[] = "0123456789abcdef";

inline bool is_unreserved_char(const char c)
{
    return (c >= 'A' && c<= 'Z') || (c >= 'a' && c<= 'z') || (c >= '0' && c <= '9') || ('-' == c) || ('_' == c) || ('.' == c) || ('~' == c);
}
// to hex
inline std::string to_hex(std::string const& s)
{
    std::string ret;
    for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
    {
        ret += hex_chars[((unsigned char)*i) >> 4];
        ret += hex_chars[((unsigned char)*i) & 0xf];
    }
    return ret;
}
// escape string to %hex
inline std::string escape_string(const std::string& s)
{
    std::string ret;
    std::string h;

    for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
    {
        h = *i;
        if (!is_unreserved_char(*i))
            h = "%" + to_hex(h);
        ret += h;
    }

    return ret;
}

/**
 *	hex to bin
 */ 
std::string hex2bin(std::string stx)
{
	std::string ret;
	unsigned int n1,n2;
	for (std::string::const_iterator i = stx.begin(); i != stx.end(); i=i+2)
	{
		for (unsigned int j = 0; j < sizeof(hex_chars); ++j)
		{
			if (*i == hex_chars[j])
			{
				n1 = j;
			}
			if (*(i+1) == hex_chars[j])
			{
				n2 = j;
			}
		}
		
		ret += ((unsigned char)n1 << 4) | ((unsigned char)n2);
	}
	return ret;
}

/**
 *	bin216
 */
inline std::string bin216(std::string& str)
{
    return to_hex(str);
}

/**
 *	encode32
 */
static const char keyStr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

inline std::string encode32(std::string& input)
{
    input = escape_string(input);

    std::string output = "";
    unsigned char chr1, chr2, chr3;
    unsigned int enc1, enc2, enc3, enc4 = 0;
    unsigned int i = 0;
    unsigned int len = input.length();
    do
    {
        chr1 = i<len?input[i++]:0;
        chr2 = i<len?input[i++]:0;
        chr3 = i<len?input[i++]:0;
        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;
        if (chr2 == 0)
        {
            enc3 = enc4 = 64;
        }
        else if (chr3 == 0)
        {
            enc4 = 64;
        }
        output = output + keyStr[enc1] + keyStr[enc2] + keyStr[enc3] + keyStr[enc4];
        chr1 = chr2 = chr3 = 0;
        enc1 = enc2 = enc3 = enc4 = 0;
    }
    while ( i < len);

    return output;
};

/**
 *	Base32.encrypt
 */
// base32.longArrayToString
inline std::string longArrayToString(std::vector<long> data)
{
    int length = data.size();

    int n = (length - 1) << 2;
    std::string str;

    for (int i = 0; i < length; i++)
    {
        str += (data[i] & 0xff);
        str += (data[i] >> 8 & 0xff);
        str += (data[i] >> 16 & 0xff);
        str += (data[i] >> 24 & 0xff);
    }

    return str;
};
// base32.stringToLongArray
inline std::vector<long> stringToLongArray(const char* str, bool includeLength)
{
    int len = strlen(str);

    std::vector<long> result;
    long result_tmp;

    for (int i = 0; i < len; i += 4)
    {
        //int num = i >> 2;
        result_tmp = str[i] | str[i + 1] << 8 | str[i + 2] << 16 | str[i + 3] << 24;
        result.push_back(result_tmp);
    }

    if (includeLength)
    {
        result.push_back(len);
    }

    return result;
};
// base32.encrypt
inline std::string base32_encrypt(std::string val, std::string key)
{
    std::vector<long> v = stringToLongArray(val.c_str(), true);
    std::vector<long> k = stringToLongArray(key.c_str(), false);

    int k_len = 0;
    if (k.size() < 4)
    {
        k_len = 4;
    }
    int n = v.size()-1;

    long z = v[n];
    long y = v[0];

    long mx, e, p, q = (long)floor(6 + 52 / (n + 1)),sum = 0;

    unsigned long delta = 0x9E3779B8;

    while (0 < q--)
    {
        sum = sum + delta & 0xffffffff;
        e = sum >> 2 & 3;
        for (p = 0; p < n; p++)
        {
            y = v[p + 1];

            long mx_p1 = (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y);

            mx = ((unsigned)z >> 5 ^ y << 2) + ((unsigned)y >> 3 ^ z << 4) ^ (sum ^ y)
                 + ( ((unsigned(p & 3 ^ e)<k.size())?k[p & 3 ^ e]:0) ^ z);

            z = v[p] + mx & 0xffffffff;
            v[p] = v[p] + mx & 0xffffffff;
        }
        y = v[0];

        mx = ((unsigned)z >> 5 ^ y << 2) + ((unsigned)y >> 3 ^ z << 4) ^ (sum ^ y)
             + ( ((unsigned(p & 3 ^ e)<k.size())?k[p & 3 ^ e]:0) ^ z);

        z =  v[n] + mx & 0xffffffff;
        v[n] = v[n] + mx & 0xffffffff;
    }

    return longArrayToString(v);
};


#endif

// unit_test.
//#include <iostream>
//int main(int argc, char* argv[])
//{
//    std::cout << encode32(bin216(base32_encrypt("1111", "NDQzNDg4"))) << std::endl;
//
//    return 0;
//}
