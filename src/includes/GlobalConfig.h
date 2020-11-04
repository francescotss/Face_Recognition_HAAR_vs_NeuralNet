#include "commons.h"
#include <map>
#ifndef OPENCV_GLOBALCONFIG_H
#define OPENCV_GLOBALCONFIG_H


class GlobalConfig {
private:
    static GlobalConfig* instance;

    map<string,string> data;

    GlobalConfig();

public:
    static void init(const string& config_path = "../config/config.cfg");
    static string get_string(const string& key);
    static int get_value(const string& key);
    static float get_float(const string& key);
};


#endif //OPENCV_GLOBALCONFIG_H
