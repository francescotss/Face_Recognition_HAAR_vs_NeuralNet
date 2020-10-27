#ifndef OPENCV_DATASET_H
#define OPENCV_DATASET_H

#include "commons.h"

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif __cpp_lib_experimental_filesystem
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "no filesystem support: abort"
#endif


void create_dataset(const string& in_path, const string& out_path, const string& face_model_path, int min_images);
void create_csv(const string& in_dir_path,const string& out_dir_path, int img_for_training);

#endif //OPENCV_DATASET_H
