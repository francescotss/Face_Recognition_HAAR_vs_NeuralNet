#ifndef OPENCV_COMMONS_H
#define OPENCV_COMMONS_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <set>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/face.hpp"
#include "opencv2/imgproc.hpp"

#define DEBUG false
using namespace std;
using namespace cv;
using namespace cv::face;


#define fatal_error(msg) {cerr << msg << strerror(errno) << endl; exit(EXIT_FAILURE);}
#endif //OPENCV_COMMONS_H
