/*\
 * main.cpp
 *
 * Copyright (c) 2017 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <iostream>
#include <locale>
#include <string>
#ifdef WIN32
#include <codecvt>
#endif
#include <fstream>
#include <stdio.h>
#include <string.h> // for strdup(), ...
#include <string>
#include <algorithm> // for find_if
#include <cctype> // for isspace
#include <stdint.h>
#include "Utf8Ucs2Converter.h"

static const char *module = "main";

static const char *usr_input = 0;
static const char *out_file = "tempout.txt";
static int reverse = 0;
void give_help( char *name )
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    printf(" --out <file>  (-o) = Set output file. (def=%s)\n", out_file);
    printf(" --reverse     (-r) = Read in unicode, and output utf-8\n");
    printf("\n");
    printf(" Read user input file as utf-8, and output UCS-2 to output file.\n");
    printf(" And vice versa if '--reverse', (-r), given...\n");
    printf(" Exit(0) if help shown, or success. Exit(1) otherwise.\n");
}

int parse_args( int argc, char **argv )
{
    int i,i2,c;
    char *arg, *sarg;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        i2 = i + 1;
        if (*arg == '-') {
            sarg = &arg[1];
            while (*sarg == '-')
                sarg++;
            c = *sarg;
            switch (c) {
            case 'h':
            case '?':
                give_help(argv[0]);
                return 2;
                break;
            case 'o':
                if (i2 < argc) {
                    i++;
                    sarg = argv[i];
                    out_file = strdup(sarg);
                }
                else {
                    printf("%s: Expect output file to follow '%s'. Try -? for help...\n", module, arg);
                    return 1;
                }
                break;
            case 'r':
                reverse = 1;
                break;
            // TODO: Other arguments
            default:
                printf("%s: Unknown argument '%s'. Try -? for help...\n", module, arg);
                return 1;
            }
        } else {
            // bear argument
            if (usr_input) {
                printf("%s: Already have input '%s'! What is this '%s'?\n", module, usr_input, arg );
                return 1;
            }
            usr_input = strdup(arg);
        }
    }
    if (!usr_input) {
        printf("%s: No user input found in command!\n", module);
        return 1;
    }
    return 0;
}

#if 0 // 00000000000000000000000000000000000000000
int add_BOM_utf16(std::wofstream &fs)
{
    unsigned char smarker[2];
    smarker[0] = 0xFF;
    smarker[1] = 0xFE;
    fs << smarker;
    return 0;
}

int add_BOM_utf8(std::ofstream &fs)
{
    unsigned char smarker[3];
    smarker[0] = 0xEF;
    smarker[1] = 0xBB;
    smarker[2] = 0xBF;
    fs << smarker;
    return 0;
}
#endif // 0000000000000000000000000000000000000000

int process_in_file_UTF8(const char *file) // actions of app
{
    int iret = 0;
    int lines = 0;
    size_t in_len = 0;
    size_t out_len = 0;
    std::string str;
    std::ifstream inf(file);
    if (!inf) {
        std::cerr << "Error: FAILED to open '" << file << "'" << std::endl;
        return 1;
    }
    printf("%s: Reading input file '%s'...\n", module, file);
    //std::wofstream fs(out_file, std::ios::binary);
    std::wofstream fs;
#ifdef WIN32
    std::locale loc(std::locale(), new std::codecvt_utf16<wchar_t>);
    fs.imbue(loc);
#endif
    fs.open(out_file, std::ios::trunc | std::ios::binary);
    //fs.open(out_file, std::ios::trunc | std::ios::out);
    if (!fs) {
        std::cerr << "Error: FAILED to open out file '" << out_file << "'" << std::endl;
        inf.close();
        return 1;
    }
    printf("%s: Writting output file '%s' as UTF-16 (UCS-2)... (wcs=%u)\n", module, out_file, (int)sizeof(wchar_t));
    //add_BOM_utf16(fs);
    fs << (wchar_t)0xfeff; // UTS-16 BOM
    out_len += 2;
    while (std::getline(inf, str)) {
        lines++;
        in_len += str.length();
        std::wstring ws = utf8ToUcs2(str);
        //out_len += (ws.size() * sizeof(wchar_t));
        out_len += (ws.length() * 2);
        fs << ws;
        //fs << std::endl;
#ifdef WIN32
        fs << (wchar_t)0x0d;
        out_len += 2;
#endif // WIN32
        fs << (wchar_t)0x0a;
        out_len += 2;
    }
    printf("%s: Read, converted and wrote %u lines, %u bytes... (%u)\n", module, lines, (int)out_len, (int)fs.tellp());
    fs.close();
    inf.close();
    return iret;
}

// trim from end (in place) - THIS FAILED
#if 0 // 0000000000000000000000000000000000000000000000
static inline void rtrim(std::wstring &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
#endif // 000000000000000000000000000000000000000000000

static void trim_wCR(std::wstring &mystring)
{
    if (!mystring.empty()) {
        size_t len = mystring.size();
        if (len > 1) {
            int ch = mystring[len - 2];
            if (ch == '\r')
                mystring.erase(len - 2);
        }
    }
}

int process_in_file_UCS2(const char *file) // actions of app
{
    int iret = 0;
    int lines = 0;
    size_t out_len = 0;
    size_t len;
    std::wstring str, str2;
    std::wifstream ifs(file);
    if (!ifs) {
        std::cerr << "Error: FAILED to open '" << file << "'" << std::endl;
        return 1;
    }
    printf("%s: Reading input file '%s'... as UCS-2...\n", module, file);
    std::ofstream ofs;
    /* creat output file */
    ofs.open(out_file, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Error: FAILED to open out file '" << out_file << "'" << std::endl;
        ifs.close();
        return 1;
    }
    printf("%s: Writting output file '%s' as UTF-8...\n", module, out_file);
    while (std::getline(ifs, str)) {
        len = str.length();
        //rtrim(str); // FAILED
        trim_wCR(str);  /* seems to work... */
        len = str.length();
        if (lines == 0) {
            /* check and remove utf-16 BOM from wstring */
            unsigned char c1 = (unsigned char)str[0];
            unsigned char c2 = 0;
            if (len > 1)
                c2 = (unsigned char)str[1];
            /* is it the BOM */
            if (((c1 == 0xff) && (c2 == 0xfe)) || 
                ((c1 == 0xfe) && (c2 == 0xff))) {
                //if (((c1 & 0xff) == 0xfe) && ((c2 & 0xff) == 0xff)) {
                if (len >= 2) {
                    str2 = str.substr(2);
                    len = str2.length();
                    if (len == 0)
                        continue;
                    str = str2; // copy the balance
                }
                else {
                    continue;
                }
            }
        }
        lines++;
        std::string s = ucs2ToUtf8(str);
        out_len += s.length();
        ofs << s;
        ofs << std::endl;
#ifdef WIN32
        out_len += 2;
#else
        out_len++;
#endif
    }

    printf("%s: Read, converted and wrote %u lines, %u bytes... (%u)\n", module, lines, (int)out_len, (int)ofs.tellp());
    ifs.close();
    ofs.close();
    return iret;
}


// main() OS entry
int main( int argc, char **argv )
{
    int iret = 0;
    iret = parse_args(argc,argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }
    if (reverse)
        iret = process_in_file_UCS2(usr_input); // actions of app
    else
        iret = process_in_file_UTF8(usr_input); // actions of app

    return iret;
}


// eof = main.cpp
