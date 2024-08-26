#ifndef COMPILER_HPP
#define COMPILER_HPP
#include <string>
using namespace std;
string get_compiler() {
    // 检查预处理器宏来确定编译器
#if defined(__GNUC__) || defined(__GNUG__)
    // GCC编译器
    return "GCC " + to_string(__GNUC__) + "." + to_string(__GNUC_MINOR__);
#elif defined(_MSC_VER)
    // MSVC编译器
    return "MSVC " + to_string(_MSC_VER / 100) + "." + to_string(_MSC_VER % 100);
#elif defined(__clang__)
    // Clang编译器
    return "Clang " + to_string(__clang_major__) + "." + to_string(__clang_minor__) + "." + to_string(__clang_patchlevel__);
#else
    // 未知编译器
    return "Unknown Compiler";
#endif
}

// 定义一个宏，用于在编译时获取当前时间
#define COMPILE_TIME __DATE__ " " __TIME__

#endif