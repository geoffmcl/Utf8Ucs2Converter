/* Utf8Ucs2Convert.h */
#ifndef _UTF8UCS2CONVERT_H_
#define _UTF8UCS2CONVERT_H_

/* from : https://stackoverflow.com/questions/5047971/how-do-i-check-for-c11-support */
#ifndef HAVE_C11_SUPPORT
#if defined(_MSC_VER)
#   if _MSC_VER < 1800 
// #       error This project needs atleast Visual Studio 2013
#       define HAVE_C11_SUPPORT 0
#   endif
#elif __cplusplus <= 199711L
// #   error This project can only be compiled with a compiler that supports C++11
#   define HAVE_C11_SUPPORT 0
#endif
#ifndef HAVE_C11_SUPPORT
#define HAVE_C11_SUPPORT 1
#endif
#endif

extern std::wstring utf8ToUcs2(const std::string& utf8Str);
extern std::string ucs2ToUtf8(const std::wstring& ucs2Str);

#endif /* _UTF8UCS2CONVERT_H_ */
/* eof */
