//
// Created by Francesco Riccardo Tassone on 28/10/2020.
//
#include "commons.h"
#include <map>
#ifndef OPENCV_GLOBALCONFIG_H
#define OPENCV_GLOBALCONFIG_H


class GlobalConfig {
private:
    map<string,string> data;
    string path = "../config/config.cfg";

public:
    GlobalConfig();
    GlobalConfig(const string& config_path);

    void init();
    string get_string(const string& key);
    int get_value(const string& key);
};


#endif //OPENCV_GLOBALCONFIG_H
