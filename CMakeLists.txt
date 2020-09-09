cmake_minimum_required(VERSION 3.10)
project(OpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

include_directories(src/includes)

add_executable(facerec src/facerec.cpp src/utils.cpp src/includes/commons.h)
target_link_libraries(facerec ${OpenCV_LIBS})