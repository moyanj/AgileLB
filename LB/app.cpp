#include "app.h"
#include "utils.h"
#include "server.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <yaml-cpp/yaml.h>

#if defined(IS_WIN)
    #include <windows.h>
#else
    #include <cstdlib>
#endif

using namespace std;

YAML::Node Config;

// 跨平台获取环境变量的函数
string get_env_variable(const char* variableName) {
#if defined(IS_WIN)
    // Windows 下使用 GetEnvironmentVariable
    char buffer[1024];
    DWORD size = GetEnvironmentVariable(variableName, buffer, sizeof(buffer));
    if (size > 0) {
        return string(buffer, size);
    }
#else
    // UNIX 下使用 getenv
    const char* value = getenv(variableName);
    if (value != NULL) {
        return string(value);
    }
#endif
    return "";
}

YAML::Node get_config() {
    string configPath = get_env_variable("AL_CONFIG_PATH");
        
    // 如果环境变量未设置，则使用默认路径
    if (configPath.empty()) {
        configPath = "lb.yml";
    }
    try {
        YAML::Node config = YAML::LoadFile(configPath);
        return config;
    } catch (const exception& e) {
        cerr << "Error loading config file: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

int start() {
    Config = get_config();
    return 0;
}