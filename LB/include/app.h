#ifndef APP_H
#define APP_H
#include <yaml-cpp/yaml.h>
#include <string>
std::string get_env_variable(const char* variableName);
YAML::Node get_config();
extern YAML::Node Config;
int start();
std::string get_env_variable(const char* variableName);
#endif