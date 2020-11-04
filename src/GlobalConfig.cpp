#include "GlobalConfig.h"

GlobalConfig* GlobalConfig::instance = nullptr;

GlobalConfig::GlobalConfig() = default;



void GlobalConfig::init(const string &config_path) {

    if (instance == nullptr)
        instance = new GlobalConfig();
    else
        instance->data.clear();


    ifstream file;
    file.open(config_path);
    if (!file.is_open())
        fatal_error("Configuration file not found");

    string line;
    while (getline(file,line)){
        if (line[0] == '#' || line[0] == '\n')
            continue;

        string key = line.substr(0,line.find('='));
        string value = line.substr(line.find('=')+1);
        instance->data.insert({ key, value });
    }

    file.close();
}


string GlobalConfig::get_string(const string &key) {
    return instance->data.find(key)->second;
}

int GlobalConfig::get_value(const string &key) {
    string temp = instance->data.find(key)->second;
    return stoi(temp);
}

float GlobalConfig::get_float(const string &key) {
    string temp = instance->data.find(key)->second;
    return stod(temp);
}









