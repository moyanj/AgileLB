#include "app.h"
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace std;
YAML::Node Config;

YAML::Node get_config() {
    try {
        // 加载 YAML 文件
        YAML::Node config = YAML::LoadFile("lb.yml");
        return config;
        
    } catch (const YAML::Exception& e) {
        cerr << "YAML Exception: " << e.what() << endl;
        exit(1);
    }
}

int start() {
    Config = get_config();
    
    return 0;
}