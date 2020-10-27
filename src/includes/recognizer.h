#ifndef OPENCV_RECOGNIZER_H
#define OPENCV_RECOGNIZER_H

#include "commons.h"

void train_recognizer(const string& csv_path, const string& model_path);
void test_recognizer(const string& csv_path, const string& model_path);
void train_test_recognizer(const string& config_dir_path, const string& model_path);
#endif //OPENCV_RECOGNIZER_H
