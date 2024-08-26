#ifndef APP_H
#define APP_H
#include <yaml-cpp/yaml.h>
#include "httplib.h"

YAML::Node get_config();
extern YAML::Node Config;
int start();
void handler(const httplib::Request &req, httplib::Response &res);
#endif