#include "GlobalConfig.h"

GlobalConfig::GlobalConfig() {
    init();
}

GlobalConfig::GlobalConfig(const string &config_path) {
    path = config_path;
    init();
}

void GlobalConfig::init() {
    ifstream file;
    file.open(path);
    if (!file.is_open())
        fatal_error("Configuration file not found");

    string line;
    while (getline(file,line)){
        if (line[0] == '#')
            continue;

        string key = line.substr(0,line.find('='));
        string value = line.substr(line.find('=')+1);
        data.insert({ key, value });
    }

    file.close();
}

string GlobalConfig::get_string(const string &key) {
    return data.find(key)->second;
}

int GlobalConfig::get_value(const string &key) {
    string temp = data.find(key)->second;
    return atoi(temp.c_str());
}




