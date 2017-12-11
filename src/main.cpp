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
#include <codecvt>
#include <fstream>
#include <stdio.h>
#include <string.h> // for strdup(), ...
#include <string>
#include <stdint.h>
#include "Utf8Ucs2Converter.h"

/* from : https://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support */
#if defined(_MSC_VER)
#   if _MSC_VER < 1800 
#       error This project needs atleast Visual Studio 2013
#   endif
#elif __cplusplus <= 199711L
#   error This project can only be compiled with a compiler that supports C++11
#endif

static const char *module = "main";

static const char *usr_input = 0;
static const char *out_file = "tempout.txt";

void give_help( char *name )
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    printf(" --out <file>  (-o) = Set output file. (def=%s)\n", out_file);
    // TODO: More help
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

int process_in_file(const char *file) // actions of app
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
    std::locale loc(std::locale(), new std::codecvt_utf16<wchar_t>);
    fs.imbue(loc);
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

    iret = process_in_file(usr_input); // actions of app

    return iret;
}


// eof = main.cpp
