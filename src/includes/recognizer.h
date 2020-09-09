#ifndef OPENCV_RECOGNIZER_H
#define OPENCV_RECOGNIZER_H

#include "commons.h"
#include "utils.h"

void train_recognizer(const string& csv_path, const string& model_path);
void test_recognizer(const string& csv_path, const string& model_path);
#endif //OPENCV_RECOGNIZER_H
