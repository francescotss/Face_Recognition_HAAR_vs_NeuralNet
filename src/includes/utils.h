#ifndef OPENCV_UTILS_H
#define OPENCV_UTILS_H
#include "commons.h"

void handle_error(const std::string& msg);
bool is_picture(const std::string& path);
void create_csv(const string& dir, const string& train_name, const string& test_name);


#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "no filesystem support"
#endif


#endif //OPENCV_UTILS_H
