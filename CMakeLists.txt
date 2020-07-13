cmake_minimum_required(VERSION 3.16)
project(OpenCV)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS})