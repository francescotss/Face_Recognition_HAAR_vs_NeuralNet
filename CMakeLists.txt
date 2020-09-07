cmake_minimum_required(VERSION 3.10)
project(OpenCV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(facerec facerec.cpp)
target_link_libraries(facerec ${OpenCV_LIBS})