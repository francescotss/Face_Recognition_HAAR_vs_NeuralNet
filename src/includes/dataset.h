#ifndef OPENCV_DATASET_H
#define OPENCV_DATASET_H

#include "commons.h"
#include "GlobalConfig.h"

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "no filesystem support: abort"
#endif

void create_dataset();
void create_csv(short img_for_training);

#endif //OPENCV_DATASET_H
