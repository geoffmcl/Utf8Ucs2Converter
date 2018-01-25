/*\
 * test1.cxx
 *
 * Copyright (c) 2017 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
 * from : http://en.cppreference.com/w/cpp/locale/codecvt_utf16
 *
\*/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#ifdef WIN32
#include <codecvt>
#else
#include <endian.h>
#ifndef little_endian
#define little_endian __LITTLE_ENDIAN
#endif
#endif
// other includes

using namespace std;

// static const char *module = "test1";

static const char* test = "temptest.txt";

void prepare_file()
{
    // UTF-16le data (if host system is little-endian)
    char16_t utf16le[4] ={0x007a,          // latin small letter 'z' U+007a
                          0x6c34,          // CJK ideograph "water"  U+6c34
                          0xd834, 0xdd0b}; // musical sign segno U+1d10b
    // store in a file
    std::ofstream fout(test);
    fout.write( reinterpret_cast<char*>(utf16le), sizeof utf16le);
    fout.close();
}
 
int main()
{
    prepare_file();
    // open as a byte stream
    std::wifstream fin(test, std::ios::binary);
    // apply facet
    fin.imbue(std::locale(fin.getloc(),
       new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
 
    for (wchar_t c; fin.get(c); )
            std::cout << std::showbase << std::hex << c << '\n';
}

// eof = test1.cxx
