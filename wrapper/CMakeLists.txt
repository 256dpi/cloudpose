cmake_minimum_required(VERSION 3.13)

add_definitions(-std=c++11)

find_package(GFlags REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)
find_package(OpenPose REQUIRED)

add_executable(main main.cpp)

include_directories(${OpenPose_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${Caffe_INCLUDE_DIRS})
target_link_libraries(main caffe ${OpenPose_LIBS} ${OpenCV_LIBS} ${Caffe_LIBRARIES})
