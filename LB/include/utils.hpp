#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
using namespace std;

#if defined(_WIN32) || defined(_WIN64)
    #define IS_WIN
#else
    #if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) && defined(__MACH__)
        #define IS_UNIX
    #endif
#endif

void print(const string& text) {
    cout << text << endl;
}


#endif